#include "8051.h"
#include "keypad.h"
#include "display.h"
#define TIMER_VAL 15536
#define cnt 8   /* 0.4sec */
#define pitch_down 12
#define pitch_up 13
#define start_record 14
#define play_record 15
#define pitch_max 6
#define pitch_min 2

int keypad();
void Timer_init();
void display(int target, int target2);
void delay(unsigned int count);
/* P0: keypad input, P2: 7's segment display, P1_0: speaker, P1_6 & P1_7 control display */
int control, shift, index, init, i, mode, counter, pause, pitch;
/* muusic frequency: keypad */ /* C2-C6 */
__code unsigned int scaleTable[][12] = {{57844, 58290, 58687, 59126, 59438, 59789, 60101, 60434, 60728, 60991, 61262, 61471}, 
                                        {61719, 61939, 62135, 62331, 62506, 62679, 62833, 62985, 63132, 63263, 63390, 63512}, 
                                        {63628, 63731, 63835, 63928, 64021, 64103, 64185, 64260, 64331, 64400, 64463, 64524}, 
                                        {64580, 64633, 64684, 64732, 64777, 64820, 64860, 64898, 64934, 64968, 64994, 65030}, 
                                        {65058, 65085, 65110, 65134, 65157, 65178, 65198, 65217, 65235, 65252, 65268, 65283}};
int __xdata record_note[500] = {0}, record_pitch[500] = {0};

int main() {
    Timer_init();
    _Bool mins = 0, plus = 0, recorded = 0, chord = 0;
    int num;
    while(1) {
        num = keypad();
        // button up
        if (num == -1) {
            if (!mode) {
                control = 0;
                TR0 = 0;
            }
            // record
            if (recorded && chord && pitch != 0 && i < sizeof(record_note)/sizeof(record_note[0])) {
                record_note[i] = pause;
                record_pitch[i] = pitch;
                i++;
                chord = 0;
            }
            // transposition (debounce)
            if (plus && !mins) {
                shift++;
                plus = 0;
            }
            if (mins && !plus) {
                shift--;
                mins = 0;
            }
        }
        else {
            if (num < pitch_down) {
                TR0 = 1;    /* start timer0 */
                control = 1;
                pause = num;
                pitch = shift;
                if (recorded)
                    chord = 1;
                // to display A, A#, B
                if (num > 8 && num < pitch_down)
                    index = shift + 1;
                else
                    index = shift;
                display(num, index);
            }
            /* transposition */
            if ((num == pitch_down || num == pitch_up) && shift <= pitch_max && shift >= pitch_min) {
                if (num == pitch_down && shift != pitch_min)
                    mins = 1;
                if (num == pitch_up && shift != pitch_max)
                    plus = 1;
            }
            /* start record */
            if (num == start_record)
                recorded = 1;
            /* play the record */
            if (num == play_record) {
                if (recorded) {
                    TR0 = 1;
                    control = 1;
                    TR1 = 1;    /* Start timer1 */ 
                    mode = 1;
                    recorded = 0;
                }
            }
        }
    }
}

void Timer_init() {
    TMOD = 0x11;    /* timer0, timer1 */
    IE = 0x8A;      /* EA = 1, ET0 = 1, ET1 = 1 */
    TH0 = 0x00 >> 8;
    TL0 = 0x00 & 0xFF;
    TH1 = TIMER_VAL >> 8;
    TL1 = TIMER_VAL & 0xFF;

    control = 0;
    index = 0;
    shift = 4;  /* C4 */
    i = 0;
    init = 0;
    mode = 0;
    pause = 0;
    pitch = 0;
    counter = 1;
}

// frequency
void Timer0_ISR(void) __interrupt(1) __using (1) {
    // mode 1: record mode, mode 0: general mode
    if (mode) {
        TH0 = scaleTable[record_pitch[init]-2][record_note[init]] >> 8;
        TL0 = scaleTable[record_pitch[init]-2][record_note[init]] & 0xFF;
    }
    else {
        TH0 = scaleTable[pitch-2][pause] >> 8;
        TL0 = scaleTable[pitch-2][pause] & 0xFF;
    }
    TF0 = 0;

    if (control) {
        P1_0 = 0;
        control = 0;
    }
    else {
        P1_0 = 1;
        control = 1;
    }
}

// tempo
void Timer1_ISR(void) __interrupt (3) __using (2) {
    TH1 = TIMER_VAL >> 8;
    TL1 = TIMER_VAL & 0xFF;
    TF1 = 0;

    counter++;
    if (counter == cnt) {
        init++;
        counter = 1;
    }
    if (init == i) {
        init = 0;
        i = 0;
        mode = 0;
        TR1 = 0;
        TR0 = 0;
        // clean the record buffer
        for (int k = 0; k < sizeof(record_note)/sizeof(record_note[0]); k++) {
            record_note[k] = 0;
            record_pitch[k] = 0;
        }
    }
}

#include "8051.h"

/* display the pitch: 7's segments */
// range(seg1): 0-6 (num: 7); range(seg2): C-B(include #) (num: 12)
unsigned char seg1[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8}, 
              seg2[] = {0b11000110, 0b01000110, 0b11000000, 0b01000000, 0b10000110, 0b10001110, 0b00001110, 
                        0b11000010, 0b01000010, 0b10001000, 0b00001000, 0b10000000};

void delay(unsigned int count) {
    unsigned int i;
    for (i = 0; i < count; i++);
}

/* P2: 7's segment display, P1_6 & P1_7 control display */
void display(int target, int target2) {
    /* target: button on/off; target2: frequency */
    if (target == -1) {
        P1_6 = 1;
        P1_7 = 1;
    }
    else {
        int n = 100;
        while(n--) {
            // the tens
            P1_6 = 0;
            P2 = seg1[target2];
            delay(200);
            P1_6 = 1;
            
            // the units
            P1_7 = 0;
            P2 = seg2[target];
            delay(200);
            P1_7 = 1;
        }
    }
}

#include "8051.h"

/* P0: keypad input */
int keypad(){
	unsigned char pad[] = {0xfe, 0xfd, 0xfb, 0xf7}, 
                column[] = {0x10, 0x20, 0x40, 0x80};
    int i, j, key = -1;
    for (i = 0; i < 4; i++){
        P0 = pad[i];
        for (j = 0; j < 4; j++){
            if ((P0 & column[j]) == 0){
                key = 4 * j + i;
                i = 4;
                j = 4;
            }
        }
    }
    return key;
}
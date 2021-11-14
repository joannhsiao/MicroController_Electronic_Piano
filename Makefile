FILE = *.hex *.rel *.mem *.map *.lst *.rst *.ihx *.lk *.sym *.asm

digit.o: 
	sdcc -c keypad.c
	sdcc -c display.c
	sdcc eOrgan-107321004.c keypad.rel display.rel
	packihx eOrgan-107321004.ihx > eOrgan-107321004.hex
myIO.lib:display.rel keypad.rel
	sdar -o myIO.lib -rc display.rel keypad.rel
clean:
	del $(FILE)
# MicroController_Electronic_Piano
- Hardware: 8051, keypad, 7-seg. LED
- Description:  
	- timer: 0.4sec
	- keypad:  
		(基準音: C4, minima: C2, maxima: B7)  
		- pitch down: 12  
		- pitch up: 13  
		- start record: 14  
		- play record: 15  
	- LED: used to display the pitch

## script
1. `eOrgan-107321004.c`: main program
2. `keypad.c`: input from keypad
3. `display.c`: display on the 7-seg LED

## run the program
```
$ make
```

## Demo
https://user-images.githubusercontent.com/59599987/177502356-ba776bee-cb79-45eb-9631-880a9eaef9ca.mp4


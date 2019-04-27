#include "../core.h"

uint8 pin = 0;
uint8 counter = 0;

uint16 soundCallback(){
	counter++;
	if (counter < 64){
		return 1 << pin;
	}
	return 0;
}

void displayPinName(){
	switch(pin){
		case 0:
			dDisplayText("GPIOB_PIN_5", 4, 10, 10);
			break;
			
		case 1:
			dDisplayText("GPIOB_PIN_6", 4, 10, 10);
			break;
			
		case 2:
			dDisplayText("GPIOB_PIN_7", 4, 10, 10);
			break;
			
		case 3:
			dDisplayText("GPIOA_PIN_8", 4, 10, 10);
			break;
			
		case 4:
			dDisplayText("GPIOA_PIN_9", 4, 10, 10);
			break;
			
		case 5:
			dDisplayText("GPIOA_PIN_10", 4, 10, 10);
			break;
			
		case 6:
			dDisplayText("GPIOA_PIN_11", 4, 10, 10);
			break;
			
		case 7:
			dDisplayText("GPIOA_PIN_12", 4, 10, 10);
			break;
	}
}

int32 main(){
	bool upPressed = false;
	bool downPressed = false;
	
	sndEnableSoundMono(4000, 8, soundCallback);
	
	while (1) {
		if (iGetState(INPUT_B) && !upPressed && pin < 7){
			pin++;
		}
		
		if (iGetState(INPUT_A) && !downPressed && pin > 0){
			pin--;
		}
		
		upPressed = iGetState(INPUT_B);
		downPressed = iGetState(INPUT_A);
		
		displayPinName();
		dSync();
	}
	return 0;
}
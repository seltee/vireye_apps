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
			displayText("GPIOB_PIN_4", 4, 10, 10);
			break;
			
		case 1:
			displayText("GPIOB_PIN_5", 4, 10, 10);
			break;
			
		case 2:
			displayText("GPIOB_PIN_6", 4, 10, 10);
			break;
			
		case 3:
			displayText("GPIOB_PIN_7", 4, 10, 10);
			break;
			
		case 4:
			displayText("GPIOB_PIN_8", 4, 10, 10);
			break;
			
		case 5:
			displayText("GPIOB_PIN_9", 4, 10, 10);
			break;
			
		case 6:
			displayText("GPIOB_PIN_10", 4, 10, 10);
			break;
			
		case 7:
			displayText("GPIOB_PIN_11", 4, 10, 10);
			break;
	}
}

int32 main(){
	bool upPressed = false;
	bool downPressed = false;
	
	enableSoundMono(4000, 8, soundCallback);
	
	while (1) {
		if (getButtonState(INPUT_B) && !upPressed && pin < 7){
			pin++;
		}
		
		if (getButtonState(INPUT_A) && !downPressed && pin > 0){
			pin--;
		}
		
		upPressed = getButtonState(INPUT_B);
		downPressed = getButtonState(INPUT_A);
		
		displayPinName();
		sync();
	}
	return 0;
}
#include "../core.h"

uint8 pin = 0;
uint8 counter = 0;

// Callback that will be called 4000 times per second
// Output based on selected pin. 
// Each bit of byte affects different pin with different resistance.
// Thats the main idea
uint16 soundCallback(){
	counter++;
	if (counter < 64){
		return 1 << pin;
	}
	return 0;
}

// Displays pin name
void displayPinName(){
	switch(pin){
		case 0:
			displayText("GPIOB_PIN_5", 4, 10, 10);
			break;
			
		case 1:
			displayText("GPIOB_PIN_6", 4, 10, 10);
			break;
			
		case 2:
			displayText("GPIOB_PIN_7", 4, 10, 10);
			break;
			
		case 3:
			displayText("GPIOB_PIN_8", 4, 10, 10);
			break;
			
		case 4:
			displayText("GPIOB_PIN_9", 4, 10, 10);
			break;
			
		case 5:
			displayText("GPIOB_PIN_10", 4, 10, 10);
			break;
			
		case 6:
			displayText("GPIOB_PIN_11", 4, 10, 10);
			break;
			
		case 7:
			displayText("GPIOB_PIN_12", 4, 10, 10);
			break;
	}
}

int32 main(){
	bool upPressed = false;
	bool downPressed = false;
	
	enableSoundMono(4000, 8, soundCallback);
	
	// Controls pins switching and displays it's name
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
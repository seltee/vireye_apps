#include "../core.h"
#include "config.h"
#include "utils.h"
#include "selector.h"

CoreConfig oldConfig;
uint8 menuSelector;
uint8 prevState = 0, move;
bool selectState = false;

uint8 configUpdate(){
	CoreConfig edConfig;
	getConfig(&edConfig, sizeof(CoreConfig));
	
	move = (getYAxis() > 1024) ? 2 : (getYAxis() < -1024) ? 1 : 0;
	if (move == 2 && move != prevState && menuSelector > 0) menuSelector--;
	if (move == 1 && move != prevState && menuSelector < 1) menuSelector++;
	prevState = move;
	
	displaySpriteBitMask(selectedSprite, 3, 0, menuSelector*15+5, 20, 6);
	
	displayText("X-AXIS", 2, 5, 5, false);
	displayText("Y-AXIS", 2, 5, 20, false);
	
	if (edConfig.invertAxisX){
		displayText("INVERTED", 7, 315-8*8, 5, false);
	}else{
		displayText("NORMAL", 2, 315-6*8, 5, false);
	}
	
	if (edConfig.invertAxisY){
		displayText("INVERTED", 7, 315-8*8, 20, false);
	}else{
		displayText("NORMAL", 2, 315-6*8, 20, false);
	}
	
	if (!selectState && getButtonState(INPUT_A)){
		switch(menuSelector){
			case 0:
				edConfig.invertAxisX = edConfig.invertAxisX ? 0 : 1;
				break;
				
			case 1:
				edConfig.invertAxisY = edConfig.invertAxisY ? 0 : 1;
				break;
		}
		applyConfig(&edConfig, sizeof(CoreConfig));
	}
	selectState = getButtonState(INPUT_A);
	
	if (getButtonState(INPUT_START)){
		saveConfig();
		return openSelector();
	}
	
	if (getButtonState(INPUT_Y)){
		applyConfig(&oldConfig, sizeof(CoreConfig));
		return openSelector();
	}
	
	displayText("START - SAVE&EXIT, Y - EXIT", 4, 5, 240-5-8, false);
	
	return MODE_CONFIG;
}

uint8 openConfig(){
	memset(&oldConfig, 0, sizeof(CoreConfig));
	getConfig(&oldConfig, sizeof(CoreConfig));
	menuSelector = 0;
	return MODE_CONFIG;
}
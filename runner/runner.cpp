#include "../core.h"
#include "selector.h"
#include "sound_player.h"
#include "utils.h"

unsigned char spriteBuffer[2*1024];
char mode;

int32 main(){
	// Init main systems
	dSetSpriteMemory(spriteBuffer, 2*1024);	
	if (!fsInit(readBuffer)){
		while(1){
			dDisplayText("SD is not available", 2, 5, 5, false);
			dSync();
		}
	}
	
	// Main cycle
	mode = openSelector();
	while (1) {
		switch(mode){
			case MODE_SELECTOR:
				mode = selectorUpdate();
				break;
				
			case MODE_SOUND_PLAYER:
				mode = soundPlayerUpdate();
				break;
		}
		dSync();
	}
	return 0;
}

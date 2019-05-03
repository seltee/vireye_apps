#include "../core.h"
#include "selector.h"
#include "sound_player.h"
#include "config.h"
#include "utils.h"


unsigned char spriteBuffer[2*1024];
char mode;

int32 main(){
	// Init main systems
	setSpriteMemory(spriteBuffer, 2*1024);	
	if (!setFSMemory(readBuffer)){
		while(1){
			displayText("SD is not available", 2, 5, 5, false);
			sync();
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
				
			case MODE_CONFIG:
				mode = configUpdate();
				break;
		}
		sync();
	}
	return 0;
}

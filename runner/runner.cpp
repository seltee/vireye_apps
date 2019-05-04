#include "../core.h"
#include "selector.h"
#include "sound_player.h"
#include "config.h"
#include "utils.h"

// 128 sprites buffer. Each sprite - 16 bytes
unsigned char spriteBuffer[2*1024];
char mode;

int32 main(){
	// Init sprite subsystem
	setSpriteMemory(spriteBuffer, 2*1024);	
	
	// Init SD subsystem
	if (!setFSMemory(readBuffer)){
		while(1){
			displayText("SD is not available", 2, 5, 5, false);
			sync();
		}
	}
	
	// Main cycle. Updates current "Page" and get's next page.
	// If page don't want to switch, then it just return self ID.
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

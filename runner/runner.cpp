#include "../core.h"
#include "selector.h"
#include "sound_player.h"
#include "config.h"
#include "utils.h"

char mode;

int32 main(){
	// Init sprite subsystem
	setSpriteLimit(128);

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

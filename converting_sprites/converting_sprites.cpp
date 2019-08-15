#include "../core.h"
#include "pallete.h"

int32 main(){
	// Init sprite subsystem
	setSpriteLimit(5);
	setFPS(30);
	setPalette(pallete);
	
	int frameCounter = 0;
	int walkX = -image_sprite_width;
	
	while (1) {
		frameCounter++;
		walkX += 3;
		
		if (walkX > 320){
			walkX = -image_sprite_width;
		}

		displaySprite(image_sprite[(frameCounter>>2)%10], walkX, 200-image_sprite_height, image_sprite_width, image_sprite_height, SPRITE_FLAG_NO, false);
		displaySprite(image_ground, 0, 200, image_ground_width, image_ground_height);
		
		
		sync();
	}
	return 0;
}
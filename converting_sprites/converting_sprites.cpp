#include "../core.h"
#include "pallete.h"

void callback(unsigned short *line, unsigned short number){
	unsigned short color = ((number >> 3) << 3) + (((240 - number) >> 3) << 8);
	for (int i = 0; i < 320; i++){
		line[i] = color;
	}
}

int32 main(){
	// Init sprite subsystem
	setSpriteLimit(5);
	setFPS(30);
	setPalette(pallete);
	setPreProcessCallback(callback);
	
	int frameCounter = 0;
	int walkX = -image_sprite_width;
	
	while (1) {
		frameCounter++;
		walkX += 3;
		
		if (walkX > 320){
			walkX = -image_sprite_width;
		}

		displaySprite(image_ground, 0, 240-(image_ground_height<<1), image_ground_width, image_ground_height);
		displaySprite(image_sprite[(frameCounter>>2)%10], walkX, 190-image_sprite_height, image_sprite_width, image_sprite_height, SPRITE_FLAG_NO, false);
		
		
		sync();
	}
	return 0;
}
#include "../core.h"
#include "pallete.h"
#include "comp_math.h"

#define WAVE_LENGTH 10
#define BOAT_MOVE 60
#define LIGHT_LINE 22

unsigned int matrixShiftTop = 0, matrixShiftBottom = 0;
unsigned int frameCounter = 0;
SpriteMatrix topTilesMatrix, bottomTilesMatrix;
const uint8 *spritesTop[4];
const uint8 *spritesBottom[8];

//fix, must work
const uint8 *sprites[4] = {
	image_tiles1[0],
	image_tiles1[1],
	image_tiles1[2],
	image_tiles1[3]
};

// Simple Square Root
int sqrt(int x) 
{ 
    if (x == 0 || x == 1) 
    return x; 
  
    int i = 1, result = 1; 
    while (result <= x) 
    { 
      i++; 
      result = i * i; 
    } 
    return i - 1; 
} 

// Callback before drawing sprites
void preCallback(unsigned short *line, unsigned short number){
	if (number >= 32 && number <= 240-32){
		unsigned short blue;
		int b = pre_sin[(number * W_PRECISION * 6) % (360 *  W_PRECISION)];
		int c = (b * WAVE_LENGTH)/M_PRECISION + WAVE_LENGTH;
		
		for (int i = 0; i < 320; i++){
			blue = ((c + (i>>1) - frameCounter*2)%32);
			
			if (blue == 31 && c < WAVE_LENGTH){
				line[i] = 0xffff;
			} else {
				line[i] = ((blue | 0x03) << 8) | 0x6018;
			}
			
			i++;
			if (blue == 31 && c < WAVE_LENGTH){
				line[i] = 0xffff;
			}else {
				line[i] = ((blue | 0x03) << 8) | 0x6018;
			}
		}
	} else {
		memset(line, 0, 640);
	}
}

// Callback after drawing sprites
void postCallback(unsigned short *line, unsigned short number){
	char r, g, b;
	if (number < LIGHT_LINE + 40){
		int dist = LIGHT_LINE - (int)number;
		dist = dist < 0 ? -dist : dist;
		if (dist <= 40){
			int size = sqrt(40*40 - dist*dist);
			
			for (int i = 0; i < 13; i++){
				if (topTilesMatrix.matrix[i] == 3){
					int start = i*32-size + matrixShiftTop - 48;
					int lim = i*32+size + matrixShiftTop - 48;
					if (start < 0) start = 0;
					if (lim >= 320) lim = 320;
					
					for (int ix = start; ix < lim; ix++){
						r = ((line[ix] >> 3) & 0x1f) + 12;
						g = ((line[ix] >> 13) + ((line[ix] & 0x07) << 3)) + 12;
						b = ((line[ix] >> 8) & 0x1f) + 2;
						if (r > 0x1f) r = 0x1f;
						if (g > 0x3f) g = 0x3f;
						if (b > 0x1f) b = 0x1f;
						
						line[ix] = (r << 3) + (g >> 3) + (g << 13) + (b << 8);
					}
				}
			}
		}
	}
}

int32 main(){
	// Init sprite subsystem
	setSpriteLimit(40);
	setFPS(30);
	setPalette(pallete);
	setPreProcessCallback(preCallback);
	setPostProcessCallback(postCallback);
	
	spritesTop[0] = image_tiles1[0];
	spritesTop[1] = image_tiles1[1];
	spritesTop[2] = image_tiles1[2];
	spritesTop[3] = image_tiles1[3];
	
	spritesBottom[0] = image_tiles2[0];
	spritesBottom[1] = image_tiles2[1];
	spritesBottom[2] = image_tiles2[2];
	spritesBottom[3] = image_tiles2[3];
	spritesBottom[4] = image_tiles2[0] + 1024;
	spritesBottom[5] = image_tiles2[1] + 1024;
	spritesBottom[6] = image_tiles2[2] + 1024;
	spritesBottom[7] = image_tiles2[3] + 1024;
	
	topTilesMatrix.sprites = sprites;
	topTilesMatrix.matrix = new uint8[13];

	bottomTilesMatrix.sprites = spritesBottom;
	bottomTilesMatrix.matrix = new uint8[26];
	
	for (int i = 0; i < 13; i++){
		topTilesMatrix.matrix[i] = rand()%4;
		bottomTilesMatrix.matrix[i] = rand()%4;
		bottomTilesMatrix.matrix[i+13] = (rand()%4) + 4;
	}
			
	while (1) {
		frameCounter++;
		matrixShiftTop+=5;
		matrixShiftBottom+=8;
		
		if (matrixShiftTop > 32){
			matrixShiftTop -= 32;
			for (int i = 12; i > 0; i--){
				topTilesMatrix.matrix[i] = topTilesMatrix.matrix[i-1];
			}
			topTilesMatrix.matrix[0] = rand()%4;
		}
		
		if (matrixShiftBottom > 32){
			matrixShiftBottom -= 32;
			for (int i = 12; i > 0; i--){
				bottomTilesMatrix.matrix[i] = bottomTilesMatrix.matrix[i-1];
				bottomTilesMatrix.matrix[i+12] = bottomTilesMatrix.matrix[i-1+12];
			}
			bottomTilesMatrix.matrix[0] = rand()%4;
			bottomTilesMatrix.matrix[13] = (rand()%4) + 4;
		}
		
		// Boat will move on Y by sin
		int bsin = pre_sin[(frameCounter << 2) % (360 *  W_PRECISION)];
		displaySprite(image_boat[(frameCounter >> 1)%2], 240, 25 + (bsin * BOAT_MOVE)/M_PRECISION + BOAT_MOVE, 64, 64, SPRITE_FLAG_NO, false); 
		
		displaySpriteMatrix(&topTilesMatrix, 32, -64+matrixShiftTop, 0, 13, 1, SPRITE_FLAG_NO, false);
		displaySpriteMatrix(&bottomTilesMatrix, 32, -64+matrixShiftBottom, 240-64, 13, 2, SPRITE_FLAG_NO, false);
				
		
				
		sync();
	}
	return 0;
}
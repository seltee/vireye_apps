#include "../core.h"
#include "field.h"
#include "colors.h"
#include "highScore.h"
#include "sound.h"

// Data for our field and figures. Third for "next figure".
uint8 mainFieldData[20*10];
uint8 figureData[4*4];
uint8 nextData[4*4];

// Other necessary data
int16 fPositionX, fPoisitionY;
uint8 go = 0, speed;
int16 move = 0;
bool prevStateA;
bool isGameOver;
uint32 score;
Field mainField, figure, next; 
uint8 level;
uint8 figuresInPlay;
int8 buffer[40];
uint32 highScore, lastHighScore;

// I will use this pallete instide of default
const uint16 colorPalleter[] = {
	REAL_BLACK,
	REAL_BLACK,
	BLACK,
	WHITE,
	DIRTY_WHITE,
	GREY,
	LIGHT_BLUE,
	DARK_BLUE,
	BLOODY_RED,
	DARK_RED,
	BRIGHT_GREEN,
	DARK_GREY,
	SUNNY,
	DARK_SUNNY,
	PURPLE,
	DARK_PURPLE
};

// This is edges of our game field. It's just a column sprite. 
// There will be better ways to do columns in the future, but now we can do such only this way.
const uint8 edge[] = {
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
};

// List of figures. 
// Actually we can use bit mask and reduce from 16 bytes to 2 bytes each figure, but with this style we have a lot better view.
const uint8 figures[][16] = {
	{
		1, 1, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	},
	{
		1, 0, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0,
	},
	{
		1, 1, 0, 0,
		1, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	},
	{
		0, 1, 0, 0,
		1, 1, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 0,
	},
	{
		1, 1, 0, 0,
		1, 0, 0, 0,
		1, 0, 0, 0,
		0, 0, 0, 0,
	},
	{
		1, 1, 1, 0,
		1, 0, 1, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	},
	{
		0, 1, 0, 0,
		1, 1, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
	},
	{
		1, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 1, 0,
		0, 0, 0, 0,
	},
	{
		0, 1, 0, 0,
		1, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 0, 0,
	},
	{
		1, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 0, 0,
		0, 1, 1, 0,
	},
	{
		0, 1, 1, 0,
		0, 1, 1, 0,
		0, 1, 1, 0,
		0, 1, 1, 0,
	},
	{
		1, 1, 1, 1,
		1, 0, 0, 1,
		1, 0, 0, 1,
		0, 0, 0, 0,
	},
};

// Makes new figure in next field
void makeNewFigure(){
	// View representation random selection
	uint8 sNum = (rand()%FIELD_SPRITE_COUNT)+1, i;
	
	// Random rotating of figure
	uint8 rotCount = rand()%4;
	
	// Namber of figure based on level
	uint8 figNum = rand() % figuresInPlay;
		
	memcpy(nextData, figures[figNum], 4*4);
	for (i = 0; i < 4*4; i++){
		nextData[i] = nextData[i] ? sNum : 0;
	}
	
	for(i = 0; i < rotCount; i++){
		next.rotate();
	}
}

// Copies next to current
void setNext(){
	memcpy(figureData, nextData, 4*4);
	makeNewFigure();
	figure.calcRealWidth();
	fPositionX = 5-figure.getRealWidth()/2;
	fPoisitionY = -2;
}

// Does what it says
void newGame(){
	score = 0;
	isGameOver = false;
	speed = 20;
	level = 1;
	figuresInPlay = 5;
	
	mainField.init(10, 20, mainFieldData);
	figure.init(4, 4, figureData);
	next.init(4, 4, nextData);
	
	makeNewFigure();
	setNext();
}

// Updates current level, speed and number of figures in play
void updateLevel(){
	level = (score/10000)+1;
	speed = level < 10 ? 20-level : 10;
	figuresInPlay = level + 4;
	figuresInPlay = figuresInPlay > sizeof(figures)/4 ? figuresInPlay = sizeof(figures)/4 : figuresInPlay;
}

int32 main(){
	setPalette(colorPalleter);
	setSpriteLimit(300);
	initSound();
	highScore = getHighScore();
	setFPS(30);
	newGame();
	
	while(1){
		if (!isGameOver){
			// Control of vertical moving
			go++;
			if (go >= speed || getYAxis() < -1800){
				go = 0;
				if (mainField.checkDown(fPositionX, fPoisitionY, &figure)){
					if (fPoisitionY >= 0){
						// Placing figure on the main field
						mainField.copy(fPositionX, fPoisitionY, &figure);
						setNext();
						
						// Cheking explosions and getting count of it
						uint8 counter = 0;
						while(1){
							int8 exp = mainField.explode();
							if (exp == 255) break;
							counter++;
						}
						
						// Adding score
						if (counter > 0){
							const uint16 scoreCount[] = { 0, 100, 300, 600, 1200 };
							score += scoreCount[counter];
							updateLevel();
							playNoize(0x30, -2, 2, 600, 4);
						}
					} else {
						// Game over
						playNoize(0x40, -1, 1, 2000, 2);
						isGameOver = true;
						lastHighScore = highScore;
						if (lastHighScore < score){
							// Saving new high score to SD
							setHighScore(score);
							highScore = score;
						}
						continue;
					}
					
					// Hit sound
					playNoize(0x04, 0, 4, 50, 6);
				}else{
					fPoisitionY++;
				}
			}		
			
			// Control of left and right moving
			int16 axisState = getXAxis();
			if (axisState > -200 && axisState < 200){
				move = 0;
			}else{
				move += axisState;
				if (move > 4000){
					move = 0;
					if (fPositionX < 10 - figure.getRealWidth() && !mainField.checkRight(fPositionX, fPoisitionY, &figure)){
						fPositionX++;
					}
				}
				if (move < -4000){
					move = 0;
					if (fPositionX > 0 && !mainField.checkLeft(fPositionX, fPoisitionY, &figure)){
						fPositionX--;
					}
				}
			}
			
			// Rotation
			if (getButtonState(INPUT_A) && !prevStateA){
				figure.rotate();
				if (mainField.check(fPositionX, fPoisitionY, &figure)){
					figure.revert();
				}
				figure.calcRealWidth();
				if (fPositionX >= 10 - figure.getRealWidth()){
					fPositionX = 10 - figure.getRealWidth();
				}
			}

			// Drawing
			mainField.draw(40, 0);
			figure.draw(fPositionX*12+40, fPoisitionY*12);
			next.draw(170, 96);
			
			// Statistics
			itoa(score, buffer);
			displayText("SCORE", 4, 170, 10, false);
			displayText(buffer, 4, 310-(strlen(buffer)*8-1), 10, false);
			itoa(level, buffer);
			displayText("LEVEL", 4, 170, 30, false);
			displayText(buffer, 4, 310-(strlen(buffer)*8-1), 30, false);
			displayText("NEXT:", 4, 170, 80, false);
		} else {
			// Screen of death
			if (getButtonState(INPUT_A) && !prevStateA){
				newGame();
			}
			
			displayText("GAME OVER", 4, 100-(9*8)/2, 80, false);
			displayText("SCORE", 4, 100-(5*8)/2, 100, false);

			itoa(score, buffer);
			displayText(buffer, 4, 100 - (strlen(buffer)*8)/2, 120, false);
			
			if (lastHighScore < score){
				displayText("HIGH SCORE!", 8, 100-(11*8)/2, 140, false);
			} else {
				displayText("HIGH SCORE", 4, 100-(11*8)/2, 140, false);
				
				itoa(highScore, buffer);
				displayText(buffer, 4, 100 - (strlen(buffer)*8)/2, 160, false);
			}
		}
		
		// Field columns
		displaySpriteBitMask(edge, 5, 40-16, 0, 1, 120);
		displaySpriteBitMask(edge, 5, 40+120, 0, 1, 120, SPRITE_FLAG_H_MIRROR);
		
		displayText("VIREYE TETRIS", 4, 170, 220, false);
		prevStateA = getButtonState(INPUT_A);
				
		sync();
	}
}
#include "../core.h"
#include "field.h"
#include "colors.h"
#include "highScore.h"
#include "sound.h"

//memory for 300 sprites
#define SPRITE_MEMORY_SIZE 16*300

uint8 spriteMemory[SPRITE_MEMORY_SIZE];
uint8 fsBuffer[512];

uint8 mainFieldData[20*10];
uint8 figureData[4*4];
uint8 nextData[4*4];
uint8 go = 0;
uint8 speed;
int16 fPositionX, fPoisitionY;
int16 move = 0;
bool prevStateA;
bool isGameOver;
uint32 score;
Field mainField, figure, next; 
uint8 level;
uint8 figuresInPlay;
int8 buffer[40];
uint32 highScore, lastHighScore;

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

void makeNewFigure(){
	uint8 sNum = (rand()%FIELD_SPRITE_COUNT)+1, i;
	uint8 rotCount = rand()%4;
	uint8 figNum = rand() % figuresInPlay;
	
	memcpy(nextData, figures[figNum], 4*4);
	for (i = 0; i < 4*4; i++){
		nextData[i] = nextData[i] ? sNum : 0;
	}
	
	for(i = 0; i < rotCount; i++){
		next.rotate();
	}
}

void setNext(){
	memcpy(figureData, nextData, 4*4);
	makeNewFigure();
	figure.calcRealWidth();
	fPositionX = 5-figure.getRealWidth()/2;
	fPoisitionY = -2;
}

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

void updateLevel(){
	level = (score/10000)+1;
	speed = level < 10 ? 20-level : 10;
	figuresInPlay = level + 4;
	figuresInPlay = figuresInPlay > sizeof(figures)/4 ? figuresInPlay = sizeof(figures)/4 : figuresInPlay;
}

int32 main(){
	setSpriteMemory(spriteMemory, SPRITE_MEMORY_SIZE);
	setFSMemory(fsBuffer);
	setPalette(colorPalleter);
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
						mainField.copy(fPositionX, fPoisitionY, &figure);
						setNext();
						uint8 counter = 0;
						while(1){
							int8 exp = mainField.explode();
							if (exp == 255) break;
							counter++;
						}
						const uint16 scoreCount[] = { 0, 100, 300, 600, 1200 };
						score += scoreCount[counter];
						updateLevel();
						if (counter > 0){
							playNoize(0x30, -2, 2, 600, 4);
						}
					} else {
						playNoize(0x40, -1, 1, 2000, 2);
						isGameOver = true;
						lastHighScore = highScore;
						if (lastHighScore < score){
							setHighScore(score);
							highScore = score;
						}
						continue;
					}
					
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

			mainField.draw(40, 0);
			figure.draw(fPositionX*12+40, fPoisitionY*12);
			next.draw(170, 96);
			
			itoa(score, buffer);
			displayText("SCORE", 4, 170, 10, false);
			displayText(buffer, 4, 310-(strlen(buffer)*8-1), 10, false);
			itoa(level, buffer);
			displayText("LEVEL", 4, 170, 30, false);
			displayText(buffer, 4, 310-(strlen(buffer)*8-1), 30, false);
			displayText("NEXT:", 4, 170, 80, false);
		} else {
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
		
		displaySpriteBitMask(edge, 5, 40-16, 0, 1, 120);
		displaySpriteBitMask(edge, 5, 40+120, 0, 1, 120, SPRITE_FLAG_H_MIRROR);
		
		displayText("VIREYE TETRIS", 4, 170, 220, false);
		prevStateA = getButtonState(INPUT_A);
		sync();
	}
}
#include "../core.h"

class Rect{
	public: 
	Rect();
	void update();
	
	int16 x, y, smX, smY;
	int8 width, height, color, r;
	Rect *next;
};

Rect::Rect(){
	width = (rand()%50) + 10;
	height = (rand()%50) + 10;
	
	x = rand() % (320 - width);
	y = rand() % (240 - height);
	
	color = rand()%8 + 2;
	
	smX = rand()%2 == 0 ? 1 : -1;
	smY = rand()%2 == 0 ? 1 : -1;
	
	next = 0;
}

void Rect::update(){
	x+=smX;
	y+=smY;
	if (x+width >= 320) smX = -1;
	if (x <= 0) smX = 1;
	if (y+height >= 240) smY = -1;
	if (y <= 0) smY = 1;
	
	displayFilledRect(color, x, y, width, height, false);
}

Rect *firstRect;

uint16 count(){
	uint16 counter = 0;
	Rect *rect = firstRect;
	while(rect){
		counter++;
		rect = rect->next;
	}
	return counter;
}

void addSprite(){
	Rect *rect = firstRect;
	while(rect->next){
		rect = rect->next;
	}
	
	Rect *newRect = new Rect();
	if (newRect){
		rect->next = newRect;
		setSpriteLimit(count() + 10);
	}
}

void removeSprite(){
	Rect *rect = firstRect;
	Rect *prevRect = 0;
	while(rect->next){
		prevRect = rect;
		rect = rect->next;
	}
	if (prevRect){
		prevRect->next = 0;
		delete rect;
		setSpriteLimit(count() + 10);
	}
}

void updateAndDisplaySprites(){
	Rect *rect = firstRect;
	while(rect){
		rect->update();
		rect = rect->next;
	}
}

int32 main(){
	// Init sprite subsystem
	setSpriteLimit(10);
	
	// First rect
	firstRect = new Rect();
	
	// For rendering number of sprites
	char buff[6];
	
	bool upPressed = false;
	bool downPressed = false;
	
	while (1) {
		if (getButtonState(INPUT_B) && !upPressed){
			addSprite();
		}
		
		if (getButtonState(INPUT_A) && !downPressed){
			removeSprite();
		}
		
		upPressed = getButtonState(INPUT_B);
		downPressed = getButtonState(INPUT_A);
		
		updateAndDisplaySprites();
		itoa(count(), buff);
		displayText(buff, 4, 5, 5);
		itoa(getFreeMem(), buff);
		displayText(buff, 3, 5, 21);
		sync();
	}
	return 0;
}
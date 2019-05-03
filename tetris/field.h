#pragma once
#include "../core.h"

#define FIELD_SPRITE_COUNT 5

class Field{
public:
	void init(int16 width, int16 height, uint8 *data);
	void draw(int16 x, int16 y);
	void set(int16 x, int16 y, uint8 sprite);
	uint8 get(int16 x, int16 y);
	int16 getRealWidth();
	int16 getWidth();
	int16 getHeight();
	
	bool check(int16 x, int16 y, Field *field);
	bool checkDown(int16 x, int16 y, Field *field);
	bool checkLeft(int16 x, int16 y, Field *field);
	bool checkRight(int16 x, int16 y, Field *field);
	void copy(int16 x, int16 y, Field *field);
	void rotate();
	void revert();
	void clearGups();
	bool hasSomeInColumn(uint8 column);
	bool hasSomeInRow(uint8 row);
	void calcRealWidth();
	int8 explode();
private:
	int16 width, height, realWidth;
	uint16 arrSize;
	uint8 *data;
	uint8 prev[16];
};

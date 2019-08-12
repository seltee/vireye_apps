#pragma once
#include "../core.h"

enum Modes{
	MODE_SELECTOR,
	MODE_SOUND_PLAYER,
	MODE_CONFIG
};

extern uint8 megaBuffer[8*1024];
extern uint8 drawBuffer[2400];
extern const uint8 selectedSprite[20*6];
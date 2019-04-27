#pragma once

enum Modes{
	MODE_SELECTOR,
	MODE_SOUND_PLAYER
};

extern unsigned char readBuffer[512];
extern unsigned char megaBuffer[6*1024];
extern unsigned char drawBuffer[2400];
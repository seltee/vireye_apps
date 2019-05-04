#include "sound.h"

#define MAIN_FREQ 22050

#define TYPE_FREQ 0x01
#define TYPE_NOIZE 0x02

// This stucture describes sound freq, power and changing of this values.
struct Channel{
	uint8 works;
	uint8 state;
	uint8 u1;
	uint8 u2;
	
	int16 change;
	int16 power;
	
	int16 waitCount;
	int16 counter;
	
	int16 changeCount;
	int16 changeCounter;
	
	int16 freqCount;
	int16 freqCounter;
	
	int16 out;
	int16 freqChange;
	
	uint32 length;
};

Channel channels[4];

bool c = false;

// Sound callback, as it said. Called MAIN_FREQ times per second, so, it must be fast, or at least not slow.
// There are 4 channels that affects output with their out.
uint16 soundCallback(){
	int16 line = 0;
	for (uint8 i = 0; i < 4; i++){
		if (channels[i].works){
			channels[i].length--;
			channels[i].counter--;
			channels[i].changeCounter--;
			channels[i].freqCounter--;
			
			if (channels[i].length == 0) channels[i].works = 0;
			if (channels[i].counter == 0){
				channels[i].counter = channels[i].waitCount;
				channels[i].out = (rand() % channels[i].power) - channels[i].power/2;
			}
			
			if (channels[i].changeCounter == 0){
				channels[i].changeCounter = channels[i].changeCount;
				channels[i].power += channels[i].change;
				if (channels[i].power < 0) channels[i].works = 0;
			}
					
			if (channels[i].freqCounter == 0){
				channels[i].freqCounter = channels[i].freqCount;
				channels[i].waitCount += channels[i].freqChange;
			}
			line += channels[i].out;
		}
	}
	
	return (uint16)line+0x7fff;
}

// Enables sound subsystem. It not needs external buffers.
void initSound(){
	memset(channels, 0, sizeof(Channel)*10);
	enableSoundMono(MAIN_FREQ, 8, soundCallback);
}

// Returns channel, that free
uint8 getFreeChannel(){
	for (uint8 i = 0; i < 4; i++){
		if (channels[i].works == 0){
			return i;
		}
	}
	return 255;
}

// Configures one of free channels
uint8 playNoize(int16 power, int16 powerChange, int16 freqChange, uint16 length, uint16 initialFreq){
	uint8 channel = getFreeChannel();
	if (channel != 255){
		channels[channel].change = powerChange;
		channels[channel].freqChange = freqChange;
		channels[channel].waitCount = initialFreq;
		channels[channel].counter = initialFreq;
		channels[channel].power = power;
		
		channels[channel].changeCounter = (MAIN_FREQ / 30);
		channels[channel].changeCount = (MAIN_FREQ / 30);
		
		channels[channel].freqCount = (MAIN_FREQ / 30);
		channels[channel].freqCounter = (MAIN_FREQ / 30);
		
		channels[channel].length = length * (MAIN_FREQ / 1000);
		channels[channel].state = 0;
		channels[channel].works = 1;
	}
	return channel;
}




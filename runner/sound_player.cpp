#include "../core.h"
#include "sound_player.h"
#include "utils.h"
#include "selector.h"

// This page playes sounds. 
// It just enables sound system on freq and bitdepth of the sound file.
// Then it sends information from SD to buffer in infinit loop.

struct WavStruct{
	uint8 sig[4];
	uint32 fileSize;
	uint8 wavSig[4];
	uint8 fmg[4];
	uint32 formatLength;
	uint16 typeOfFormat;
	uint16 numberOfChannels;
	uint32 sampleRate;
	uint32 fullSampleRate;
	uint16 bitsOnChannels;
	uint16 bitsPerSample;
	uint8 dataHeader[4];
	uint32 dataSize;
};

int lengthPlayed;
int prevSample;
char bytesPerSample;
int position;
bool rightPart;
int readed;
bool playing;

FileWorker fileWorker;
WavStruct wavStruct;

uint16 snd(){
	if (position < readed){
		position++;
		lengthPlayed += bytesPerSample;
		if (position == 6*1024) position = 0;
		if (wavStruct.bitsPerSample == 8)
			return megaBuffer[position];
		else
			return megaBuffer[position+1] + (megaBuffer[position] << 8);
	}
	return 0;
}

char soundPlayerUpdate(){
	if (playing){
		uint32 c = position, addition;
		uint16 tpx;
		uint8 sndSample, x, shift, pix;
		
		memset(drawBuffer, 0, 2400);
		
		for (uint32 c = position, i = 0; i < 160; i++){
			sndSample = megaBuffer[c] >> 1;
			if (sndSample >= 0 && sndSample < 120){
				x = i >> 3;
				shift = i % 8;
				tpx = x + sndSample*20;
				pix = drawBuffer[tpx];
				drawBuffer[tpx] = pix | (1 << shift);
			}
			c++;
			if (c >= 6*1024) c = 0;
		}
		displaySpriteBitMask(drawBuffer, 7, 0, 0, 20, 120);
			
		if (!rightPart && position < 3*1024){
			rightPart = true;
			addition = readFile(&fileWorker, megaBuffer+3*1024, 3*1024);
			if (addition){
				readed += addition;
			}else{
				stop();
			}
		}
		
		if (rightPart && position > 3*1024){
			rightPart = false;
			addition = readFile(&fileWorker, megaBuffer, 3*1024);
			if (addition){
				readed += addition;
			}else{
				stop();
			}
		}
	} else {
		displayText("Press Y to exit", 7, 5, 5, false);
	}
	
	if (getButtonState(INPUT_Y)){
		stop();
		return openSelector();
	}
	
	return MODE_SOUND_PLAYER;
}

char openSoundPlayer(char *file){
	if (!openToRead(file, &fileWorker)) return 0;

	readFile(&fileWorker, &wavStruct, 44);

	lengthPlayed = 0;
	prevSample = 0;
	position = 0;
	rightPart = false;
	bytesPerSample = wavStruct.numberOfChannels * (wavStruct.bitsPerSample >> 3);
	readed = readFile(&fileWorker, megaBuffer, 3*1024);
	playing = true;
	
	enableSoundMono(wavStruct.sampleRate, wavStruct.bitsPerSample, snd);
	return MODE_SOUND_PLAYER;
}

void stop(){
	disableSound();
	playing = false;
}


#include "../core.h"
#include "selector.h"
#include "utils.h"
#include "sound_player.h"
#include "config.h"

// This page allows you to run programs, go to play sound page and open configuration
enum{
	EXT_UNKNOWN = 0,
	EXT_WAV
};

struct FileData{
	char name[15];
	char type;
};

FileData files[64];
char fCount = 0;
char path[256];
unsigned char readBuffer[512];
char prevState = 0, move;
bool buttonPrevState = false;
bool confButtonPrevState = false;
int selected = 0;
	
// Update, input, draw
char selectorUpdate(){
	char ext;
	
	// Moving on the list
	move = (getYAxis() > 1024) ? 2 : (getYAxis() < -1024) ? 1 : 0;
	if (move == 2 && move != prevState && selected > 0) selected--;
	if (move == 1 && move != prevState && selected < fCount-1) selected++;
	prevState = move;
	
	// Selecting file
	if (!buttonPrevState && getButtonState(INPUT_A)){
		ext = getExt(files[selected].name);
		if (ext){
			switch(ext){
				case EXT_WAV:
					memcpy(path+strlen(path), files[selected].name, strlen(files[selected].name) + 1);
					return openSoundPlayer(path);
			}
		}else{
			if (files[selected].type == FILE_TYPE_RUNNABLE){
				memcpy(path+strlen(path), files[selected].name, strlen(files[selected].name) + 1);
				run(path, megaBuffer);
			}
			if (files[selected].type == FILE_TYPE_DIRECTORY){
				if (cmp("..", files[selected].name)){
					path[strlen(path)-1] = 0;
					while(path[strlen(path)-1] != '/') path[strlen(path)-1] = 0;
					updateFolder();
				} else {
					memcpy(path+strlen(path), files[selected].name, strlen(files[selected].name) + 1);
					char len = strlen(path);
					path[len] = '/';
					path[len+1] = 0;
					updateFolder();
				}
				selected = 0;
			}
		}
	}
	buttonPrevState = getButtonState(INPUT_A);
	
	// Configuration
	if (!confButtonPrevState && getButtonState(INPUT_SELECT)){
		return openConfig();
	}
	confButtonPrevState = getButtonState(INPUT_SELECT);
	
	// Selection Sprite
	displaySpriteBitMask(selectedSprite, 3, 0, selected*15+5, 20, 6);

	// Drawing list
	for (int i = 0; i < 14; i++){
		if (i < fCount){
			switch(files[i].type){
				case FILE_TYPE_RUNNABLE:
					displayText(files[i].name, 7, 5, 5+15*i, false);
					break;

				case FILE_TYPE_DIRECTORY:
					displayText(files[i].name, 4, 5, 5+15*i, false);
					break;

				default:
					ext = getExt(files[i].name);
					switch(ext){
						case EXT_WAV:
							displayText(files[i].name, 8, 5, 5+15*i, false);
							break;
							
						default:
							displayText(files[i].name, 6, 5, 5+15*i, false);
					}
			}
			
		}
	}
	displayText(path, 2, 5, 214, false);
	
	return MODE_SELECTOR;
}

// Builds file list in folder
void updateFolder(){
	DirectoryReader dirReader = readDir(path);
	if (dirReader){
		FileInfo *fileInfo;
		fCount = 0;
		while((fileInfo = readNextFile(dirReader))){
			if (fCount < 64 && !(fileInfo->flags & FILE_FLAG_HIDDEN) && !cmp(".", fileInfo->fileName)){
				files[fCount].type = fileInfo->fileType;
				memcpy(files[fCount].name, fileInfo->fileName, 15);
				files[fCount].name[14] = 0;
				fCount++;
			}
		}
		closeDir(dirReader);
	}
}

char getExt(char *name){
	uint16 length = strlen(name);
	if (length > 3){
		if (name[length-3] == 'W' && name[length-2] == 'A' && name[length-1] == 'V'){
			return EXT_WAV;
		}
	}
	return EXT_UNKNOWN;
}

char openSelector(){
	// Initial folder
	path[0] = '/';
	path[1] = 0;
	
	updateFolder();
	return MODE_SELECTOR;
}

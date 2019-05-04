#include "highScore.h"
#include "../core.h"

// Get's high score from file
uint32 getHighScore(){
	FileWorker fileWorker;
	if (openToRead("/t-score.tet", &fileWorker)){
		uint32 score;
		readFile(&fileWorker, &score, 4);
		return score;
	}
	return 0;
}

// Put's high score into file
void setHighScore(uint32 score){
	FileWorker fileWorker;
	if (openToWrite("/t-score.tet", &fileWorker)){
		writeFile(&fileWorker, &score, 4);
		closeFile(&fileWorker);
	}
}
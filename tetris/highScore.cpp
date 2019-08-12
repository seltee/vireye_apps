#include "highScore.h"
#include "../core.h"

// Get's high score from file
uint32 getHighScore(){
	File file = openToRead("/t-score.tet");
	if (file){
		uint32 score;
		readFile(file, &score, 4);
		closeFile(file);
		return score;
	}
	return 0;
}

// Put's high score into file
void setHighScore(uint32 score){
	File file = openToWrite("/t-score.tet");
	if (file){
		writeFile(file, &score, 4);
		closeFile(file);
	}
}
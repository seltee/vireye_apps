#pragma once

typedef char int8;
typedef short int16;
typedef int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define SYS_FN extern "C" __declspec(dllimport)
#define DirectoryReader void*
#define File void*

// Color format:
// RRRRRGGGGGGBBBBB
// Order of byte must be reversed
// r - max 31, g - max 63, b - max 31
#define COLOR(r, g, b)\
  ((unsigned short)(\
  (((g>>3)&0x07))|\
  ((r&0x1f) << 3)|\
  ((b&0x1f) << 8)|\
  ((g&0x07) << 13)))
  
enum FillColor{
	FILL_COLOR_BLACK = 0,
	FILL_COLOR_WHITE = 1
};

enum ButtonId {
	INPUT_SELECT = 0,
	INPUT_START,
	INPUT_A,
	INPUT_X,
	INPUT_B,
	INPUT_Y
};

enum SpriteFlags{
	SPRITE_FLAG_NO = 0x00,
	SPRITE_FLAG_V_MIRROR = 0x01,
	SPRITE_FLAG_H_MIRROR = 0x02
};

enum FileTypes{
	FILE_TYPE_UKNOWN = 0x00,
	FILE_TYPE_RUNNABLE = 0x01,
	FILE_TYPE_DIRECTORY = 0x02
};

enum FileFlags{
	FILE_FLAG_NONE = 0x00,
	FILE_FLAG_HIDDEN = 0x01,
	FILE_FLAG_SYSTEM = 0x02
};

struct SpriteMatrix{
	const uint8 **sprites;
	uint8 *matrix;
};

struct FileInfo{
	unsigned int fileSize;
	
	unsigned char modifiedSeconds;
	unsigned char modifiedMinutes;
	unsigned char modifiedHours;
	unsigned char modifiedMonth;
		
	unsigned char modifiedDay;
	unsigned short modifiedYear;
	unsigned char createSeconds;
	
	unsigned char createMinutes;
	unsigned char createHours;
	unsigned char createMonth;
	unsigned char flags;
	
	unsigned char fileType;
	unsigned char r1, r2, r3;	
	
	unsigned short createYear;
	unsigned short permissions;
	
	char *fileName;
};

struct CoreConfig{
	uint8 invertAxisX;
	uint8 invertAxisY;
	uint8 u[2];
};

// DISPLAY FUNCTIONS
// Enable or disable line clear, when rendering lines on display. May fasten rendering, when disabled.
SYS_FN void setLineClear(bool state);

// Fill color of the line.
SYS_FN void setFillColor(FillColor fillNum);

// Sets maximum amount of sprites on one frame. 
// Also this function allocates memory 16*count from heap. You may free it by disabling graphics.
SYS_FN bool setSpriteLimit(uint16 count);

// Get current maximum amount of sprites
SYS_FN uint16 getSpriteLimit();

// Get current amount of sprites in cash
SYS_FN uint16 getSpriteCash();

// Displays sprite on the screen. For all drawing sprite functions you need to set up sprite memory, or it will not work
SYS_FN void displaySprite(const uint8 *sprite, int16 x, int16 y, int16 width, int16 height, uint8 flags = SPRITE_FLAG_NO, bool upScale = true);

// Displays sprite with selected color. One byte contents eight pixels, each bit presents selected color.
SYS_FN void displaySpriteBitMask(const uint8 *sprite, uint8 color, int16 x, int16 y, uint8 bytesPerWidth, int16 height, uint8 flags = SPRITE_FLAG_NO, bool upScale = true);

// Displays sprite with selected color. One byte contents eight pixels, each bit presents selected color.
SYS_FN void displaySpriteByteMask(const uint8 *sprite, uint8 color, int16 x, int16 y, int16 width, int16 height, uint8 flags = SPRITE_FLAG_NO, bool upScale = true);

// Displays sprite matrix. Matrix in matrix structure contains numbers of the sprites in the structure field sprites. Sprites contain array of pointers to the sprite data. All of them need to be same with and height (sprites must be squares) defined by size.
SYS_FN void displaySpriteMatrix(const SpriteMatrix *matrix, int8 size, int16 x, int16 y, int16 width, int16 height, uint8 flags = SPRITE_FLAG_NO, bool upScale = true);

// Display filled rect
SYS_FN void displayFilledRect(uint8 color, int16 x, int16 y, int16 width, int16 height, bool upScale = true);

// Display rect
SYS_FN void displayRect(uint8 color, int16 x, int16 y, int16 width, int16 height, bool upScale = true);

// Displays text. Note - each letter is independent sprite, so, it will take sprite memory.
SYS_FN void displayText(const int8 *string, uint8 color, uint16 x, uint16 y, bool upscale = true);

// Presents all of your sprites on the display, then clears sprite memory counter (you need to draw all again)
SYS_FN void sync();

// Set fps limit
SYS_FN void setFPS(unsigned short limit);

// Set color pallete. Each color is 2 bytes in the following model - rrrrrggggggbbbbb. as format use COLOR_TYPE_RGB565.
// Note: call it only after you arranged sprite memory
SYS_FN void setPalette(const uint16 *colors);

// By disabling graphics you may get extra memory
SYS_FN void disableGraphics();

// Sets callback, that will be called on drawing each string before main drawing. 
// First parameter will be color array, second - number of string
SYS_FN void setPreProcessCallback(void (*callback)(unsigned short *, unsigned short));

// Sets callback, that will be called on drawing each string after main drawing. 
// First parameter will be color array, second - number of string
SYS_FN void setPostProcessCallback(void (*callback)(unsigned short *, unsigned short));

// MEMORY
// Allocate memory from heap. Returns pointer to first byte. Allocated size will be alligned to 16 bytes blocks
SYS_FN int8* malloc(uint32 size);

// Free allocated memory from heap by pointer to first byte
SYS_FN void free(void *memory);

// Returns number of free memory in heap. 
// Keep in mind, that this memory may be fragmented and no garanty, that you will be able to get it all in one big block.
SYS_FN uint32 getFreeMem();


// INPUT
// Get state of button
SYS_FN bool getButtonState(uint8 buttonId);

// Get state of X axis of analog.
SYS_FN int16 getXAxis();
	
// Get state of Y axis of analog.
SYS_FN int16 getYAxis();


// FILE SYSTEM
// Open folder to read
SYS_FN DirectoryReader readDir(int8 *path);

// Read next file. Buffer for name must be at least 14 bytes
SYS_FN FileInfo *readNextFile(DirectoryReader reader);

// Close directory after reading and free memory
SYS_FN void closeDir(DirectoryReader reader);

// Open file for reading
SYS_FN File openToRead(int8 *filePath);

// Open file for writing
SYS_FN File openToWrite(int8 *filePath);

// Read data from file to buffer. In return you will get bytes actually readed
SYS_FN uint32 readFile(File file, void *dst, uint32 length);

// Write data to file from buffer. In return you will get bytes actually writed
SYS_FN uint32 writeFile(File file, void *src, uint32 length);

// Close file after read
SYS_FN void closeFile(File file);

// SOUND
// Enables mono sound output. First parameter is frequency in wich samples will go to DAC. 
// The second was is bit count in each sample. 
// If, for examples, your samples will be 16 bit and there is only 8 bit dac on board - samples will be reduced to this number. 
// The third parameter - is the function that will be called to get next sample from your program.
SYS_FN bool enableSoundMono(uint16 freq, uint8 bitPerSample, uint16 (*callback)(void));

// Disables sound output
SYS_FN void disableSound();

// CONFIG
// Get the copy of current configuration. In cfgSize you need to provide sizeof(CoreConfig).
SYS_FN bool getConfig(CoreConfig *config, unsigned int cfgSize);

// Applies configuration, but not saves it.
SYS_FN bool applyConfig(CoreConfig *config, unsigned int cfgSize);

// Saves applied configuration
SYS_FN bool saveConfig();

// HELPERS
// Compares two strings 
SYS_FN bool cmp(const int8 *str1, const int8 *str2);

// Converts integer to string
SYS_FN void itoa(int32 n, int8 str1[]);

// SYSTEM
// You need to pass buffer with at least 9 kb free to run the programm
SYS_FN void run(int8 *path, uint8 *ramBuffer);

// Default
SYS_FN void memcpy(void *dest, const void *src, unsigned int count);
SYS_FN void memset(void *s, int value, int count);
SYS_FN int32 strlen(const int8 *string);
SYS_FN uint32 rand();
#ifndef _SPRITE_H
#define _SPRITE_H

//SDL and custom headers
#include <SDL.h>
#include <string>
#include <SDL_image.h>
#include "LogManager.h"

//Sprite class
class Sprite
{
public:
	//Defualt contructor
	Sprite();

	//Overloaded constructor
	Sprite(int X, int Y, int frameWidthPix, int frameHeightPix, SDL_Renderer* r, unsigned int updateTime, float sFactor);
	
	//Defualt destructor
	~Sprite();

	//Sprite game loop functions
	void render(int x, int y, double angle);
	void update();
	void load(const char* path);
	void setNumberOfFrames(unsigned int amount);
	void clean();

	//Sprite position and dimensions
	int x, y;
	int frameWidthPixels, frameHeightPixels;

private:
	//Renderer
	SDL_Renderer* renderer = NULL;
	
	//Texture
	SDL_Texture* texture = NULL;

	//Surface to load to
	SDL_Surface* loadedSurface = NULL;
	
	//Sprite animation attributes
	unsigned int numberOfFrames = 1;
	unsigned int frameIndex = 0;
	unsigned int changeTimeMS = 10;
	float scaleFactor = 0;
};
#endif 
#include "Sprite.h"


//Defualt contructor
Sprite::Sprite()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Sprite constructed]");
}

//Defualt destructor
Sprite::~Sprite()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destroying sprite]");
}

//Overloaded constructor
Sprite::Sprite(int X, int Y, int frameWidthPix, int frameHeightPix, SDL_Renderer* r, unsigned int updateTime, float sFactor)
{
	x = X; y = Y; frameWidthPixels = frameWidthPix; frameHeightPixels = frameHeightPix; renderer = r; changeTimeMS = updateTime; scaleFactor = sFactor;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Sprite constructed]");
}

//Render function
void Sprite::render(int x, int y, double angle)
{
	//The calculate X within the sprite sheet to pull from (this
	//assumes there is only one row and many columns)
	unsigned int calcX = (frameWidthPixels * frameIndex);

	//The src rect starts at (cX, 0)
	SDL_Rect src = { (int)calcX, 0, frameWidthPixels, frameHeightPixels };

	//The dest rect (where to show it) is just the coords we want
	//to render it to
	SDL_Rect dest = { x, y, (int)(frameWidthPixels * scaleFactor), (int)(frameHeightPixels * scaleFactor) };

	//Render the sprite
	SDL_RenderCopyEx(renderer, texture, &src, &dest, angle, 0, SDL_FLIP_NONE);
}

//Update functions
void Sprite::update()
{
	//Get the number of ticks.. we'll use this to calculate what frame number we should be at
	unsigned int ticks = SDL_GetTicks();

	//The delay between changing frames -- this will change the texture every 200ms.
	changeTimeMS = 50;

	//Calculate frame index -- this is just (time / change) mod frameCount
	frameIndex = (ticks / changeTimeMS) % numberOfFrames;
}

//Load function
void Sprite::load(const char* path)
{
	//Load image at specified path
	loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Unable to load image %s! SDL_image Error: %s\n]", path, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		//If texture doesn't exsist
		if (texture == NULL)
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Unable to create texture from %s! SDL Error: %s\n]", path, IMG_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}
}

//Animation number of frames setter function
void Sprite::setNumberOfFrames(unsigned int amount)
{
	numberOfFrames = amount;
}

//Clean function
void Sprite::clean()
{
	//Free loaded image
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	//Free surface
	if (loadedSurface != NULL)
	{
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Sprite destroyed]");
}
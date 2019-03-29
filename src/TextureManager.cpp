#include "TextureManager.h"

//Static instance of the Game Manager class
TextureManager* TextureManager::tm_instance = NULL;

//Game Manager Singleton
TextureManager* TextureManager::instance()
{
	//If an instance of the game class dosen't exsist create a new game
	if (tm_instance == NULL)
		tm_instance = new TextureManager();

	//Return the instance of the game that exsists
	return tm_instance;
}

bool TextureManager::init()
{
	if (IMG_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[An error occurred when initialising images];[ERROR:%s]", IMG_GetError());

		//Images failed to initialise
		return false;
	}
	return true;
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

bool TextureManager::loadImage(std::string fileName, std::string id, SDL_Renderer * pRenderer)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0)
	{
		return false;
	}

	SDL_Texture* pTexture =
		SDL_CreateTextureFromSurface(pRenderer, pTempSurface);

	SDL_FreeSurface(pTempSurface);

	// everything went ok, add the texture to our list
	if (pTexture != 0)
	{
		m_textureMap[id] = pTexture;
		return true;
	}

	return false;
}


void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int scale, int currentRow, int currentFrame, SDL_Renderer * pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentFrame;
	srcRect.y = height * (currentRow - 1);
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect,
		&destRect, 0, 0, flip);
}

void TextureManager::clean()
{
	//Quit SDL2_Image
	IMG_Quit();
}

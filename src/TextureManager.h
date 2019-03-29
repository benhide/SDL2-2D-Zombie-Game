#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_h

//
#include <string>
#include <map>

//
#include <SDL.h>
#include <SDL_image.h>
#include "LogManager.h"

//TextureManager class
class TextureManager
{
public:
	//Game Manager singleton
	static TextureManager* instance();

	bool init();

	bool loadImage(std::string fileName, std::string id, SDL_Renderer* renderer);

	void drawFrame(std::string id, int x, int y, int width, int height, int scale, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	void clean();

	std::map<std::string, SDL_Texture*> m_textureMap;


private:
	//Static instance of the texture manager Class
	static TextureManager* tm_instance;

	//
	TextureManager();
	
	//
	~TextureManager();
};

#endif
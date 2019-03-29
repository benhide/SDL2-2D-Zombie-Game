#ifndef _KEYS_H
#define _KEYS_H

//C++ headers
#include <fstream>
#include <sstream>
#include <string>

//SDL and custom headers
#include <SDL.h>
#include "json.hpp"

//Using declaration
using json = nlohmann::json;

//Keys class
class Keys
{
public:
	Keys(void);
	~Keys();

	//Player keys
	SDL_Keycode shoot;
	SDL_Keycode left;
	SDL_Keycode right;
	SDL_Keycode up;
	SDL_Keycode down;

	//The function to load in the key values
	void loadKeys(const std::string& path);
};
#endif
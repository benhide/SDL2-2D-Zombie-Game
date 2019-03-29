#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H

//C++ Headers
#include <fstream>

//SDL and custom headers
#include <SDL.h>
#include "LogManager.h"
#include "GameManager.h"
#include "Vector2.h"
#include "json.hpp"
#include "Keys.h"

//Forward class declaration
class GameManager;

//Using declaration
using json = nlohmann::json;

//Input manager class
class InputManager
{
public:
	//Game Manager singleton
	static InputManager* instance();

	//Initialisation function
	bool init(GameManager* gm);

	//Input handler functions
	void handleInputMenu(GameManager* gm, SDL_Event& e);
	void handleInputOptions(GameManager* gm, SDL_Event& e);
	void handleInputPlay(GameManager* gm, SDL_Event& e);
	void handleInputPause(GameManager* gm, SDL_Event& e);

	//Clean function
	void clean();

	//joystickConnected boolean
	bool joystickConnected = false;

	//SDL joystick
	SDL_Joystick* gameController = NULL;

private:
	//Static instance of the Game Class
	static InputManager* im_instance;

	//Create a Keys instance
	Keys keysP1;
	Keys keysP2;

	//Joystick motion vector2
	Vector2 joyStick;

	//Defualt contructor
	InputManager();

	//Defualt destructor
	~InputManager();
};
#endif
#include "InputManager.h"

//Static instance of the InputManager class
InputManager* InputManager::im_instance = NULL;

//InputManager Singleton
InputManager* InputManager::instance()
{
	//If an instance of the InputManager class dosen't exsist create a new InputManager
	if (im_instance == NULL)
		im_instance = new InputManager();

	//Return the instance of the InputManager that exsists
	return im_instance;
}

//Defualt constructor
InputManager::InputManager()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[InputManager constructed]");
}

//Default destructor
InputManager::~InputManager()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Destorying InputManager]");
}

//Initialisation function
bool InputManager::init(GameManager* gm)
{
	//Initialise gamepad
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[SDL couldn't access Joystick!]");
	}

	//Check for gamepad, if present, load first gamepad connected.
	if (SDL_NumJoysticks() < 1)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Warning: No GameControllers connected!]");
	}
	else
	{
		//Open the joystick
		gameController = SDL_JoystickOpen(0);

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[GameController connected!]");

		//Setthe joystickConnected bool
		joystickConnected = true;

		//If the game controller doesn't exsist
		if (gameController == NULL)
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Game controller not present]");
		}
	}

	//Load in key bindings from json file
	keysP1.loadKeys("./content/p1keys.json");

	//Load in key bindings from json file
	keysP2.loadKeys("./content/p2keys.json");

	//Return success
	return true;
}

//Handle menu input function
void InputManager::handleInputMenu(GameManager* gm, SDL_Event& e)
{
	//Set the joystick values
	joyStick.X = SDL_JoystickGetAxis(gameController, 0) / 3276.70f;
	joyStick.Y = SDL_JoystickGetAxis(gameController, 1) / 3276.70f;

	//If a key is pressed down (no repeat presses)
	if (e.type == SDL_KEYDOWN && e.key.repeat == NULL)
	{
		//Switch on key pressed
		switch (e.key.keysym.sym)
		{
			//Toggle fullscreen
		case SDLK_F1:
			gm->toggleFullscreen();
			break;

			//Volume up
		case SDLK_F2:
			gm->soundManager->setVolume(-10);
			break;

			//Volume up
		case SDLK_F3:
			gm->soundManager->setVolume(10);
			break;

			//Move through menu items (down)
		case SDLK_DOWN:
			gm->currentMenuIndex = (gm->currentMenuIndex + 1) % gm->menuItems.size();
			gm->soundManager->playSound(1, gm->sfx);
			break;

			//Move through menu items (up)
		case SDLK_UP:
			if ((gm->currentMenuIndex - 1) < 0) gm->currentMenuIndex = (unsigned int)gm->menuItems.size() - 1;
			else gm->currentMenuIndex--;
			gm->soundManager->playSound(1, gm->sfx);
			break;

			//Select highlighted menu item
		case SDLK_RETURN:

			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Start the game
			if (gm->currentMenuIndex == 0)
			{
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 1;
				gm->initPlay();
				break;
			}

			//Start the game
			if (gm->currentMenuIndex == 1)
			{
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 2;
				gm->initPlay();
				break;
			}

			//Go to options menu
			else if (gm->currentMenuIndex == 2)
			{
				gm->gameState = gm->options;
				break;
			}

			//Load game
			else if (gm->currentMenuIndex == 3)
			{
				gm->loadPlayer = true;
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 1;
				gm->initPlay();
				break;
			}

			//Quit the game
			else if (gm->currentMenuIndex == 4)
			{
				gm->quit();
				break;
			}
		}
	}

	//If the event is a joystick motion
	if (e.type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//X axis motion
			if (e.jaxis.axis == 1)
			{
				//If the x axis motion is greater than 8
				if (joyStick.Y > 8.0f)
				{
					//Move the menu index
					gm->currentMenuIndex = (gm->currentMenuIndex + 1) % gm->menuItems.size();
					gm->soundManager->playSound(1, gm->sfx);
				}

				//If the x axis motion is less than 8
				else if (joyStick.Y < -8.0f)
				{
					//Move the menu index
					if ((gm->currentMenuIndex - 1) < 0)
						gm->currentMenuIndex = (unsigned int)gm->menuItems.size() - 1;
					else
					{
						gm->currentMenuIndex--;
						gm->soundManager->playSound(1, gm->sfx);
					}
				}
			}
		}
	}

	//If the event is a joystick button press
	if (e.type == SDL_JOYBUTTONDOWN)
	{
		//If its button a
		if (e.jbutton.button == 0)
		{
			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Start the game
			if (gm->currentMenuIndex == 0)
			{
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 1;
				gm->initPlay();
			}

			//Start the game
			if (gm->currentMenuIndex == 1)
			{
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 2;
				gm->initPlay();
			}

			//Go to options menu
			else if (gm->currentMenuIndex == 2)
				gm->gameState = gm->options;

			//Load game
			else if (gm->currentMenuIndex == 3)
			{
				gm->loadPlayer = true;
				gm->gameState = gm->play;
				gm->paused = false;
				gm->playerNumb = 1;
				gm->initPlay();
			}

			//Quit the game
			else if (gm->currentMenuIndex == 4)
			{
				gm->quit();
			}
		}
	}

	//If the event is the d-pad
	if (e.type == SDL_JOYHATMOTION)
	{
		//If the d-pad is pressed down
		if (e.jhat.value == SDL_HAT_DOWN)
		{
			//Move the menu index
			gm->currentMenuIndex = (gm->currentMenuIndex + 1) % gm->menuItems.size(); gm->soundManager->playSound(1, gm->sfx);
		}

		//If the x axis motion is less than 8
		else if (e.jhat.value == SDL_HAT_UP)
		{
			//Move the menu index
			if ((gm->currentMenuIndex - 1) < 0)
				gm->currentMenuIndex = (unsigned int)gm->menuItems.size() - 1;
			else
			{
				gm->currentMenuIndex--;
				gm->soundManager->playSound(1, gm->sfx);
			}
		}
	}
}

//Handle options menu input function
void InputManager::handleInputOptions(GameManager* gm, SDL_Event& e)
{
	//Set the joystick values
	joyStick.X = SDL_JoystickGetAxis(gameController, 0) / 3276.70f;
	joyStick.Y = SDL_JoystickGetAxis(gameController, 1) / 3276.70f;

	//Log info
	//LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Joystick x: f% - Joysticky: f%]", joyStick.X, joyStick.Y);

	//If a key is pressed down (no repeat presses)
	if (e.type == SDL_KEYDOWN && e.key.repeat == NULL)
	{
		//Switch on key pressed
		switch (e.key.keysym.sym)
		{
			//Toggle fullscreen
		case SDLK_F1:
			gm->toggleFullscreen();
			break;

			//Move through menu items (down)
		case SDLK_DOWN:
			gm->currentOptionsIndex = (gm->currentOptionsIndex + 1) % gm->optionsItems.size();
			gm->soundManager->playSound(1, gm->sfx);
			break;

			//Move through menu items (up)
		case SDLK_UP:
			if ((gm->currentOptionsIndex - 1) < 0) gm->currentOptionsIndex = (unsigned int)gm->optionsItems.size() - 1;
			else gm->currentOptionsIndex--;
			gm->soundManager->playSound(1, gm->sfx);
			break;

			//Select highlighted menu item
		case SDLK_RETURN:

			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Mute and unmute audio
			if (gm->currentOptionsIndex == 0)
			{
				//Mute audio
				if (gm->audio)
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("UNMUTE AUDIO");
					gm->pauseItems[1]->setText("UNMUTE AUDIO");
					gm->audio = false;
				}

				//Unmute audio
				else
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("MUTE AUDIO");
					gm->pauseItems[1]->setText("MUTE AUDIO");
					gm->audio = true;
				}
				gm->soundManager->playMusic(gm->audio);
			}

			//Mute and unmute SFX
			else if (gm->currentOptionsIndex == 1)
			{
				//Mute SFX
				if (gm->sfx)
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("UNMUTE SFX");
					gm->pauseItems[2]->setText("UNMUTE SFX");
					gm->sfx = false;
				}

				//Unmute SFX
				else
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("MUTE SFX");
					gm->pauseItems[2]->setText("MUTE SFX");
					gm->sfx = true;
				}
			}

			//Volume up
			else if (gm->currentOptionsIndex == 2)
				gm->soundManager->setVolume(10);

			//Volume down
			else if (gm->currentOptionsIndex == 3) 
				gm->soundManager->setVolume(-10);

			//Back to main menu
			else if (gm->currentOptionsIndex == 4)
				gm->gameState = gm->menu;
		}
	}

	//If the event is a joystick motion
	if (e.type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//X axis motion
			if (e.jaxis.axis == 1)
			{
				//If the x axis motion is greater than 8
				if (joyStick.Y > 8.0f)
				{
					//Move the menu index
					gm->currentOptionsIndex = (gm->currentOptionsIndex + 1) % gm->optionsItems.size(); gm->soundManager->playSound(1, gm->sfx);
				}

				//If the x axis motion is less than 8
				else if (joyStick.Y < -8.0f)
				{
					//Move the menu index
					if ((gm->currentOptionsIndex - 1) < 0)
						gm->currentOptionsIndex = (unsigned int)gm->optionsItems.size() - 1;
					else
					{ 
						gm->currentOptionsIndex--;
						gm->soundManager->playSound(1, gm->sfx);
					}
				}
			}
		}
	}

	//If the event is a joystick button press
	if (e.type == SDL_JOYBUTTONDOWN)
	{
		//If its button a
		if (e.jbutton.button == 0)
		{
			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Mute and unmute audio
			if (gm->currentOptionsIndex == 0)
			{
				//Mute audio
				if (gm->audio)
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("UNMUTE AUDIO");
					gm->pauseItems[1]->setText("UNMUTE AUDIO");
					gm->audio = false;
				}

				//Unmute audio
				else
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("MUTE AUDIO");
					gm->pauseItems[1]->setText("MUTE AUDIO");
					gm->audio = true;
				}
				gm->soundManager->playMusic(gm->audio);
			}

			//Mute and unmute SFX
			else if (gm->currentOptionsIndex == 1)
			{
				//Mute SFX
				if (gm->sfx)
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("UNMUTE SFX");
					gm->pauseItems[2]->setText("UNMUTE SFX");
					gm->sfx = false;
				}

				//Unmute SFX
				else
				{
					gm->optionsItems[gm->currentOptionsIndex]->setText("MUTE SFX");
					gm->pauseItems[2]->setText("MUTE SFX");
					gm->sfx = true;
				}
			}

			//Volume up
			else if (gm->currentOptionsIndex == 2) 
				gm->soundManager->setVolume(10);

			//Volume down
			else if (gm->currentOptionsIndex == 3) 
				gm->soundManager->setVolume(-10);

			//Back to main menu
			else if (gm->currentOptionsIndex == 4)
				gm->gameState = gm->menu;
		}
	}

	//If the event is the d-pad
	if (e.type == SDL_JOYHATMOTION)
	{
		//If the d-pad is pressed down
		if (e.jhat.value == SDL_HAT_DOWN)
		{
			//Move the menu index
			gm->currentOptionsIndex = (gm->currentOptionsIndex + 1) % gm->optionsItems.size();
			gm->soundManager->playSound(1, gm->sfx);
		}

		//If the x axis motion is less than 8
		else if (e.jhat.value == SDL_HAT_UP)
		{
			//Move the menu index
			if ((gm->currentOptionsIndex - 1) < 0)
				gm->currentOptionsIndex = (unsigned int)gm->optionsItems.size() - 1;
			else 
			{
				gm->currentOptionsIndex--;
				gm->soundManager->playSound(1, gm->sfx);
			}
		}
	}
}

//Handle game play input function
void InputManager::handleInputPlay(GameManager* gm, SDL_Event& e)
{
	//Set the joystick values
	joyStick.X = SDL_JoystickGetAxis(gameController, 0) / 3276.70f;
	joyStick.Y = SDL_JoystickGetAxis(gameController, 1) / 3276.70f;

	//Log info
	//LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Joystick x: f% - Joysticky: f%]", joyStick.X, joyStick.Y);

	//If a key is pressed down (no repeat presses)
	if (e.type == SDL_KEYDOWN && e.key.repeat == NULL)
	{
		//Switch on key
		switch (e.key.keysym.sym)
		{
			//Change game state on escape
		case SDLK_ESCAPE:
			gm->gameState = gm->pause;
			gm->paused = true;
			break;

			//Toggle fullscreen
		case SDLK_F1:
			gm->toggleFullscreen();
			break;

			//Volume up
		case SDLK_F2: 
			gm->soundManager->setVolume(-10);
			break;

			//Volume up
		case SDLK_F3:
			gm->soundManager->setVolume(10);
			break;
		}

		//Player 1 keys and controls
		if (e.key.keysym.sym == keysP1.left) gm->player1->KEY_LEFT = true;
		if (e.key.keysym.sym == keysP1.right) gm->player1->KEY_RIGHT = true;
		if (e.key.keysym.sym == keysP1.up) gm->player1->KEY_UP = true;
		if (e.key.keysym.sym == keysP1.down) gm->player1->KEY_DOWN = true;
		if (e.key.keysym.sym == keysP1.shoot) gm->player1->bulletSpawner->shoot(gm->player1->velocity, gm->sfx);

		//Player 2 keys and controls
		if (e.key.keysym.sym == keysP2.left) if (gm->player2 != NULL) gm->player2->KEY_LEFT = true;
		if (e.key.keysym.sym == keysP2.right) if (gm->player2 != NULL) gm->player2->KEY_RIGHT = true;
		if (e.key.keysym.sym == keysP2.up) if (gm->player2 != NULL) gm->player2->KEY_UP = true;
		if (e.key.keysym.sym == keysP2.down) if (gm->player2 != NULL) gm->player2->KEY_DOWN = true;
		if (e.key.keysym.sym == keysP2.shoot) if (gm->player2 != NULL) gm->player2->bulletSpawner->shoot(gm->player2->velocity, gm->sfx);
	}

	//If a key is released
	if (e.type == SDL_KEYUP && e.key.repeat == NULL)
	{
		//Player 1 keys and controls
		if (e.key.keysym.sym == keysP1.left) gm->player1->KEY_LEFT = false;
		if (e.key.keysym.sym == keysP1.right) gm->player1->KEY_RIGHT = false;
		if (e.key.keysym.sym == keysP1.up) gm->player1->KEY_UP = false;
		if (e.key.keysym.sym == keysP1.down) gm->player1->KEY_DOWN = false;

		//Player 2 keys and controls
		if (e.key.keysym.sym == keysP2.left) if (gm->player2 != NULL) gm->player2->KEY_LEFT = false;
		if (e.key.keysym.sym == keysP2.right) if (gm->player2 != NULL) gm->player2->KEY_RIGHT = false;
		if (e.key.keysym.sym == keysP2.up) if (gm->player2 != NULL) gm->player2->KEY_UP = false;
		if (e.key.keysym.sym == keysP2.down) if (gm->player2 != NULL) gm->player2->KEY_DOWN = false;
	}

	//If the event is a joystick motion
	if (e.type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//If the player is alive
			if (gm->player1->health > 0)
			{
				//X axis motion
				if (e.jaxis.axis == 1)
				{
					//gm->joystickInUse = true;
					gm->player1->velocity.Y = joyStick.Y;
					gm->player1->velocity.X = joyStick.X;

					gm->player1Speed = abs(gm->player1->velocity.length());
				}
			}
		}
		if (gm->player1Speed < 1.5f) gm->player1Speed = 0.0f;
	}

	//If the event is a joystick button press
	if (e.type == SDL_JOYBUTTONDOWN)
	{
		//If the player is alive
		if (gm->player1->health > 0)
		{
			//If its button a
			if (e.jbutton.button == 0)
			{
				gm->player1->bulletSpawner->shoot(gm->player1->velocity, gm->sfx);
			}
		}

		//If start is pressed
		if (e.jbutton.button == 7)
		{
			//Change to pause state
			gm->gameState = gm->pause;
			gm->paused = true;
		}
	}

	//If the event is the d-pad
	if (e.type == SDL_JOYHATMOTION)
	{
		//Set the players speed
		gm->player1Speed = 10.0f;

		//If the player is alive
		if (gm->player1->health > 0)
		{
			//Switch on the d-pad
			switch (e.jhat.value)
			{
				//Player1 movement controls
			case SDL_HAT_LEFT: gm->player1->velocity.X = -1.0f; break;
			case SDL_HAT_LEFTUP: gm->player1->velocity.X = -1.0f; gm->player1->velocity.Y = -1.0f; break;
			case SDL_HAT_LEFTDOWN: gm->player1->velocity.X = -1.0f; gm->player1->velocity.Y = 1.0f; break;
			case SDL_HAT_RIGHT: gm->player1->velocity.X = 1.0f; break;
			case SDL_HAT_RIGHTUP: gm->player1->velocity.X = 1.0f; gm->player1->velocity.Y = -1.0f; break;
			case SDL_HAT_RIGHTDOWN: gm->player1->velocity.X = 1.0f; gm->player1->velocity.Y = 1.0f; break;
			case SDL_HAT_UP: gm->player1->velocity.Y = -1.0f; break;
			case SDL_HAT_DOWN: gm->player1->velocity.Y = 1.0f; break;
			case SDL_HAT_CENTERED: gm->player1->velocity.X = 0.0f; gm->player1->velocity.Y = 0.0f; gm->player1Speed = 0.0f; break;
			}
		}
		else
		{
			gm->player1->velocity.X = 0.0f; gm->player1->velocity.Y = 0.0f; gm->player1Speed = 0.0f;
		}
	}
}

//Handle pause menu input function
void InputManager::handleInputPause(GameManager* gm, SDL_Event& e)
{
	//Set the joystick values
	joyStick.X = SDL_JoystickGetAxis(gameController, 0) / 3276.70f;
	joyStick.Y = SDL_JoystickGetAxis(gameController, 1) / 3276.70f;

	//Log info
	//LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Joystick x: f% - Joysticky: f%]", joyStick.X, joyStick.Y);

	//If a key is pressed down (no repeat presses)
	if (e.type == SDL_KEYDOWN && e.key.repeat == NULL)
	{
		//Switch on key pressed
		switch (e.key.keysym.sym)
		{
			//Toggle fullscreen
		case SDLK_F1: 
			gm->toggleFullscreen(); 
			break;

			//Unpause the game
		case SDLK_ESCAPE: 
			gm->gameState = gm->play;
			gm->paused = false; 
			break;

			//Move through menu items (down)
		case SDLK_DOWN:	
			gm->currentPauseIndex = (gm->currentPauseIndex + 1) % gm->pauseItems.size();
			gm->soundManager->playSound(1, gm->sfx);
			break;

			//Move through menu items (up)
		case SDLK_UP:
			if ((gm->currentPauseIndex - 1) < 0)
				gm->currentPauseIndex = (unsigned int)gm->pauseItems.size() - 1;
			else
				gm->currentPauseIndex--; gm->soundManager->playSound(1, gm->sfx);
			break;

			//Select highlighted menu item
		case SDLK_RETURN:

			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Start the game
			if (gm->currentPauseIndex == 0)
			{ 
				gm->pauseItems[6]->setText("SAVE GAME");
				gm->gameState = gm->play;
				gm->paused = false;
			}

			//Mute and unmute audio
			else if (gm->currentPauseIndex == 1)
			{
				//Mute audio
				if (gm->audio)
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("UNMUTE AUDIO");
					gm->optionsItems[0]->setText("UNMUTE AUDIO");
					gm->audio = false;
				}

				//Unmute audio
				else
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("MUTE AUDIO");
					gm->optionsItems[0]->setText("MUTE AUDIO");
					gm->audio = true;
				}
				gm->soundManager->playMusic(gm->audio);
			}

			//Mute and unmute SFX
			else if (gm->currentPauseIndex == 2)
			{
				//Mute SFX
				if (gm->sfx)
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("UNMUTE SFX");
					gm->optionsItems[1]->setText("UNMUTE SFX");
					gm->sfx = false;
				}

				//Unmute SFX
				else
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("MUTE SFX");
					gm->optionsItems[1]->setText("MUTE SFX");
					gm->sfx = true;
				}
			}

			//Volume up
			else if (gm->currentPauseIndex == 3)
				gm->soundManager->setVolume(10);

			//Volume down
			else if (gm->currentPauseIndex == 4)
				gm->soundManager->setVolume(-10);

			//Return to menu
			else if (gm->currentPauseIndex == 5)
			{
				gm->pauseItems[6]->setText("SAVE GAME");
				gm->gameState = gm->menu;
				gm->resetPlay();
			}

			//Save Game
			else if (gm->currentPauseIndex == 6)
			{
				gm->player1->serialize("./content/player.json");
				gm->pauseItems[gm->currentPauseIndex]->setText("GAME SAVED");
			}

			//Quit the game
			else if (gm->currentPauseIndex == 7)
				gm->quit();
		}
	}

	//If the event is a joystick motion
	if (e.type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//X axis motion
			if (e.jaxis.axis == 1)
			{
				//If the x axis motion is greater than 8
				if (joyStick.Y > 8.0f)
				{
					//Move the menu index
					gm->currentPauseIndex = (gm->currentPauseIndex + 1) % gm->pauseItems.size(); 
					gm->soundManager->playSound(1, gm->sfx);
				}

				//If the x axis motion is less than 8
				else if (joyStick.Y < -8.0f)
				{
					//Move the menu index
					if ((gm->currentPauseIndex - 1) < 0)
						gm->currentPauseIndex = (unsigned int)gm->pauseItems.size() - 1;
					else
					{ 
						gm->currentPauseIndex--;
						gm->soundManager->playSound(1, gm->sfx);
					}
				}
			}
		}
	}

	//If the event is a joystick button press
	if (e.type == SDL_JOYBUTTONDOWN)
	{
		//If its button a
		if (e.jbutton.button == 0)
		{
			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Start the game
			if (gm->currentPauseIndex == 0) 
			{ 
				gm->pauseItems[6]->setText("SAVE GAME");
				gm->gameState = gm->play;
				gm->paused = false;
			}

			//Mute and unmute audio
			else if (gm->currentPauseIndex == 1)
			{
				//Mute audio
				if (gm->audio)
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("UNMUTE AUDIO");
					gm->optionsItems[0]->setText("UNMUTE AUDIO");
					gm->audio = false;
				}

				//Unmute audio
				else
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("MUTE AUDIO");
					gm->optionsItems[0]->setText("MUTE AUDIO");
					gm->audio = true;
				}
				gm->soundManager->playMusic(gm->audio);
			}

			//Mute and unmute SFX
			else if (gm->currentPauseIndex == 2)
			{
				//Mute SFX
				if (gm->sfx)
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("UNMUTE SFX");
					gm->optionsItems[1]->setText("UNMUTE SFX");
					gm->sfx = false;
				}

				//Unmute SFX
				else
				{
					gm->pauseItems[gm->currentPauseIndex]->setText("MUTE SFX");
					gm->optionsItems[1]->setText("MUTE SFX");
					gm->sfx = true;
				}
			}

			//Volume up
			else if (gm->currentPauseIndex == 3) 
				gm->soundManager->setVolume(10);

			//Volume down
			else if (gm->currentPauseIndex == 4)
				gm->soundManager->setVolume(-10);

			//Return to menu
			else if (gm->currentPauseIndex == 5)
			{
				gm->pauseItems[6]->setText("SAVE GAME");
				gm->gameState = gm->menu;
				gm->resetPlay();
			}

			//Save Game
			else if (gm->currentPauseIndex == 6)
			{
				gm->player1->serialize("../content/player.json");
				gm->pauseItems[gm->currentPauseIndex]->setText("GAME SAVED");
			}

			//Quit the game
			else if (gm->currentPauseIndex == 6)
				gm->quit();
		}

		//If start is pressed
		if (e.jbutton.button == 7)
		{
			//Menu sound
			gm->soundManager->playSound(2, gm->sfx);

			//Change to pause state
			gm->gameState = gm->play; gm->paused = false;
		}
	}

	//If the event is the d-pad
	if (e.type == SDL_JOYHATMOTION)
	{
		//If the d-pad is pressed down
		if (e.jhat.value == SDL_HAT_DOWN)
		{
			//Move the menu index
			gm->currentPauseIndex = (gm->currentPauseIndex + 1) % gm->pauseItems.size();
			gm->soundManager->playSound(1, gm->sfx);
		}

		//If the x axis motion is less than 8
		else if (e.jhat.value == SDL_HAT_UP)
		{
			//Move the menu index
			if ((gm->currentPauseIndex - 1) < 0) 
				gm->currentPauseIndex = (unsigned int)gm->pauseItems.size() - 1;
			else 
			{
				gm->currentPauseIndex--;
				gm->soundManager->playSound(1, gm->sfx); }
		}
	}
}

//Clean function
void InputManager::clean()
{
	//Clean up controller
	SDL_JoystickClose(gameController);
	gameController = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Game controllers destroyed]");

	//Delete the input manager
	delete im_instance;
	im_instance = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_INPUT, SDL_LOG_PRIORITY_INFO, "[Input manager destroyed]");
}

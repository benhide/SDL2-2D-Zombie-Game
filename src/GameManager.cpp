#include "GameManager.h"

//Static instance of the Game Manager class
GameManager* GameManager::gm_instance = NULL;

//Game Manager Singleton
GameManager* GameManager::instance()
{
	//If an instance of the game class dosen't exsist create a new game
	if (gm_instance == NULL)
		gm_instance = new GameManager();

	//Return the instance of the game that exsists
	return gm_instance;
}

//Game constructor
GameManager::GameManager()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Game constructed]");
}

//Game destructor
GameManager::~GameManager()
{
	//Destroy the window, renderer
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Game renderer and window destoryed]");
}

//Initalisation function
bool GameManager::init(const char * title, int xPos, int yPos, int width, int height, bool fullscreen, bool debug)
{
	//Ste debug mode
	this->debug = debug;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Setting the window size and fullscreen mode]");

	//Initialise the game manager member variables
	winPosX = xPos, winPosY = yPos, winHeight = height, winWidth = width;

	//Local variable for the SDL window flags enum
	int flags = 0;

	//If the screen is set to fullscreen view set the flags variable to that of the fullscreen sdl window enum
	if (fullscreen)	flags = SDL_WINDOW_FULLSCREEN;

	//Set the fullscreen game memeber variable
	fullScreen = fullscreen;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialising SDL]");

	//Initialise SDL - if init successful ...
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Creating Window]");

		//Create a simple window
		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags | SDL_WINDOW_RESIZABLE);

		//Get the ID of the window
		winID = SDL_GetWindowID(window);

		//Initialise the aspect ratio
		aspectRatio = (float)width / (float)height;

		//Set the mininum window size
		SDL_SetWindowMinimumSize(window, width, height);

		//Get the current display mode
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);

		//Set the mininum window size
		SDL_SetWindowMaximumSize(window, dm.h, dm.h);

		//If the window was created
		if (window != 0)
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_INFO, "[Creating Renderer]");

			//Create the renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			//If the renderer was created
			if (renderer != 0)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_VIDEO, SDL_LOG_PRIORITY_INFO, "[Setting draw colour]");

				//Sets the renderer draw colour
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

				//SoundManager instance
				soundManager = SoundManager::instance();

				//Initialise sound manager
				if (soundManager->init())
				{
					//Log info
					LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Audio initialisation success]");

					//If images initialises
					if (IMG_Init(SDL_INIT_EVERYTHING) != 0)
					{
						//Log info
						LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Images initialisation success]");

						//Assign the input manager
						inputManager = InputManager::instance();

						//Initialise the input manager
						if (inputManager->init(this))
						{
							//Check to see if a controller is connected
							connectedJoystick = inputManager->joystickConnected;

							//Log info
							LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Game controller initialisation success]");
						}
					}
					else
					{
						//Log info
						LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Images initialisation unsuccessful]");

						//Images failed to initialise
						return false;
					}
				}
				else
				{
					//Log info
					LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Audio initialisation unsuccessful]");

					//Audio failed to initialise
					return false;
				}

			}
			//Renderer failed to initialise
			else
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[An error occurred when initialising Renderer];[ERROR:%s]", SDL_GetError());

				//Renderer failed to initialise
				return false;
			}
		}
		else
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[An error occurred when initialising Window];[ERROR:%s]", SDL_GetError());

			//Window failed to initialise
			return false;
		}
	}
	else
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[An error occurred when initialising SDL];[ERROR:%s]", SDL_GetError());

		//SDL failed to initialise
		return false;
	}


	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialisation success]");

	//Start the game loop
	gameRunning = true;

	//Initialise the gameState
	gameState = 0;

	//Play music
	soundManager->playMusic(audio);

	//Load the background sprite
	backgroundSprites("./content/splash.png");

	//Assign the font file path
	font = "./content/SpecialElite-Regular.ttf";

	//Initialise the game states
	initMenu();
	initOptions();
	initPause();

	//SDL initialised
	return true;
}









//Handle events function
void GameManager::input()
{
	//SDL event variable
	SDL_Event event;

	//While there are events to be processed
	while (SDL_PollEvent(&event))
	{
		//Keycode for pressed key
		SDL_Keycode keyPressed = event.key.keysym.sym;

		//Log key press
		if (event.type == SDL_KEYDOWN && !event.key.repeat)
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[KEY DOWN];[TIMESTAMP:%d];[CODE:%d];[CHARACTER:%s]", event.key.timestamp, keyPressed, SDL_GetKeyName(keyPressed));

		//Log key release
		if (event.type == SDL_KEYUP)
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[KEY UP];[TIMESTAMP:%d];[CODE:%d];[CHARACTER:%s]", event.key.timestamp, keyPressed, SDL_GetKeyName(keyPressed));

		//If the 'X' is pressed quit the game
		if (event.type == SDL_QUIT)	quit();

		//If there is a window event
		if (event.type == SDL_WINDOWEVENT)
		{
			if (event.window.windowID == winID)
			{
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					maintainAspectRatio(event);
				}
			}
		}

		//Switch on gamestate
		switch (gameState)
		{

		//If the game is in menu state
		case menu:
			inputManager->handleInputMenu(this, event);
			break;

		//If the game is in options state
		case options:
			inputManager->handleInputOptions(this, event);
			break;

		//If the game is in play state
		case play:
			inputManager->handleInputPlay(this, event);
			break;

		//If the game is in pause state
		case pause:
			inputManager->handleInputPause(this, event);
			break;
		}
	}
}

//Update function
void GameManager::update()
{
	//Switch on gameState
	switch (gameState)
	{

	//Update the menu
	case menu:
		break;

	//Update otions
	case options:
		break;

	//Update play
	case play:
		//If the player is alive and the game is not paused
		if (player1->health > 0 && !paused)
		{
			//Check for player collisions
			playerCollisions(player1);

			//Check for enemy collisions
			enemyCollisions();

			//Check for bullet collisions
			bulletCollisions(player1);

			//Update player
			player1->update(winWidth, winHeight, player1Speed, connectedJoystick);

			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating player 1]");
		}
		else player1->health = 0;

		//If the second player is active, alive and the game is not paused
		if (player2 != NULL && player2->health > 0 && !paused)
		{
			//Check for player collisions
			playerCollisions(player2);

			//Check for bullet collisions
			bulletCollisions(player2);

			//Update player
			player2->update(winWidth, winHeight, 0, false);

			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating player 2]");
		}
		else if (player2 != NULL) player2->health = 0;

		//Update enemies
		if (playerNumb == 1)
		{
			//If the player is alive and the game is not paused
			if (player1->health > 0 && !paused)
			{
				//Update the enemies
				enemySpawner->update(player1->position, player1->position);

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating enemies]");
			}
		}
		else if (playerNumb == 2)
		{
			if (player1->health > 0 && player2->health > 0 && !paused)
			{
				//Update the enemies check for enemy collisions
				enemySpawner->update(player1->position, player2->position);

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating enemies]");
			}
			else if (player1->health <= 0 && player2->health > 0 && !paused)
			{
				//Update the enemies
				enemySpawner->update(player2->position, player2->position);

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating enemies]");
			}
			else if (player1->health > 0 && player2->health <= 0 && !paused)
			{
				//Update the enemies
				enemySpawner->update(player1->position, player1->position);

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating enemies]");
			}
		}
		break;

	//Update pause
	case pause:
		break;
	default:
		break;
	}
}

//Render function
void GameManager::render()
{
	//Render the background
	renderBackgrounds();

	//Switch on gameState
	switch (gameState)
	{

	//Render menu
	case menu:
		//Loop through the menu items and render
		for (unsigned int i = 0; i < menuItems.size(); i++)
		{
			//Highlight current selected item
			if (i == currentMenuIndex) menuItems[i]->setColor(255, 0, 0);

			//Set unselected item as un-highlighted
			else menuItems[i]->setColor(255, 255, 255);

			//Render every menu item
			menuItems[i]->render();
		}

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering menu]");
		break;

	//Render options
	case options:
		//Loop through the menu items and render
		for (unsigned int i = 0; i < optionsItems.size(); i++)
		{
			//Highlight current selected item
			if (i == currentOptionsIndex) optionsItems[i]->setColor(255, 0, 0);

			//Set unselected item as un-highlighted
			else optionsItems[i]->setColor(255, 255, 255);

			//Render every menu item
			optionsItems[i]->render();
		}

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering options]");
		break;

	//Render play
	case play:
		//Render enemies
		enemySpawner->render(renderer);

		//Render player
		player1->render(renderer, player1->health > 0 ? false : true);

		//Render the player2 
		if (player2 != NULL) player2->render(renderer, player2->health > 0 ? false : true);

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering play]");
		break;

	//Render pause
	case pause:
		//Render enemies
		enemySpawner->render(renderer);

		//Render player
		player1->render(renderer, player1->health > 0 ? false : true);

		//Render the player2 
		if (player2 != NULL) player2->render(renderer, player2->health > 0 ? false : true);

		//Loop through the menu items and render
		for (unsigned int i = 0; i < pauseItems.size(); i++)
		{
			//Highlight current selected item
			if (i == currentPauseIndex) pauseItems[i]->setColor(255, 0, 0);

			//Set unselected item as un-highlighted
			else pauseItems[i]->setColor(255, 255, 255);

			//Render every menu item
			pauseItems[i]->render();
		}

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering pause]");
		break;

	//Render splash
	case splash:

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering splash]");
		break;
	}

	//Present the renderer
	SDL_RenderPresent(renderer);
}

//Quit Function
void GameManager::quit()
{
	gameRunning = false;
}

//Function to access private gameRunning variable
bool GameManager::running()
{
	return gameRunning;
}

//Clean function
void GameManager::clean()
{
	//Free the surface
	SDL_FreeSurface(loadedSurface);
	loadedSurface = NULL;

	//Clean up the game objects
	if (player1 != NULL)
	{
		player1->clean();
		delete player1;
		player1 = NULL;
	}

	//Clean up the game objects
	if (player2 != NULL)
	{
		player2->clean();
		delete player2;
		player2 = NULL;
	}

	//Delete the enemies
	if (enemySpawner != NULL)
	{
		enemySpawner->clean();
		delete enemySpawner;
		enemySpawner = NULL;
	}

	//Delete the menu items
	for (auto& item : menuItems)
	{
		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
	}

	//Delete the option items
	for (auto& item : optionsItems)
	{
		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
	}

	//Delete the pause items
	for (auto& item : pauseItems)
	{
		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
	}

	//Free loaded image
	SDL_DestroyTexture(texture);
	texture = NULL;

	//Close subsystems
	IMG_Quit();

	//Clean the input and sound manager
	soundManager->clean();
	inputManager->clean();

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Play items destroyed]");

	//Delete the Game Manager
	delete gm_instance;
	gm_instance = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Game manager destroyed]");
}













//Menu state initalise function
void GameManager::initMenu()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialising Menu]");

	//Initialise the current menu index
	currentMenuIndex = 0;

	//Initialize both menu items load in fonts
	ScreenText* start1p = new ScreenText(window, font);
	ScreenText* start2p = new ScreenText(window, font);
	ScreenText* optionsText = new ScreenText(window, font);
	ScreenText* loadGame = new ScreenText(window, font);
	ScreenText* quitText = new ScreenText(window, font);

	//Set up text, position and size
	start1p->setText("START 1 PLAYER", 100, 50, 28);
	start2p->setText("START 2 PLAYER", 100, 100, 28);
	optionsText->setText("OPTIONS", 100, 150, 28);
	loadGame->setText("LOAD GAME", 100, 200, 28);
	quitText->setText("QUIT", 100, 250, 28);

	//Add to the list of menu items
	menuItems.push_back(start1p);
	menuItems.push_back(start2p);
	menuItems.push_back(optionsText);
	menuItems.push_back(loadGame);
	menuItems.push_back(quitText);
}

//Options state initalise function
void GameManager::initOptions()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialising options]");

	//Initialise the current menu index
	currentOptionsIndex = 0;

	//Initialize both menu items load in fonts
	ScreenText* menuAudioText = new ScreenText(window, font);
	ScreenText* menuSfxText = new ScreenText(window, font);
	ScreenText* volUp = new ScreenText(window, font);
	ScreenText* volDown = new ScreenText(window, font);
	ScreenText* backText = new ScreenText(window, font);

	//Set up text, position and size
	menuAudioText->setText("MUTE AUDIO", 100, 50, 28);
	menuSfxText->setText("MUTE SFX", 100, 100, 28);
	volUp->setText("VOLUME UP", 100, 150, 28);
	volDown->setText("VOLUME DOWN", 100, 200, 28);
	backText->setText("BACK", 100, 250, 28);

	//Add to the list of menu items
	optionsItems.push_back(menuAudioText);
	optionsItems.push_back(menuSfxText);
	optionsItems.push_back(volUp);
	optionsItems.push_back(volDown);
	optionsItems.push_back(backText);
}

//Play state initalise function
void GameManager::initPlay()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialising play]");

	//Change to the menu game state
	backgroundSprites("./content/ground.png");

	//Initialise game objects and conditions
	player1 = new Player(Vector2(400, 600), 30, 30, debug);

	//If loading the player - load the player
	if (loadPlayer)	player1->deserialize("./content/player.json");

	//Initialise game objects and conditions
	player1->init(window, font, 1, loadPlayer);
	player2 = NULL;

	//If there are 2 players
	if (playerNumb == 2)
	{
		//Initialise game objects and conditions
		player2 = new Player(Vector2(400, 200), 30, 30, debug);
		player2->init(window, font, 2, false);
	}

	//Initialise the enemy spawner object and conditions
	enemySpawner = new EnemyController(Vector2(0, 0), 10 * playerNumb, debug);
	enemySpawner->init(window, winWidth, winHeight);
	soundManager->playSound(3, sfx);

	//Reset the loa player boolean
	loadPlayer = false;
}

//Pause state initalise function
void GameManager::initPause()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Initialising pause]");

	//Initialise the current menu index
	currentPauseIndex = 0;

	//Initialize both menu items load in fonts
	ScreenText* continueText = new ScreenText(window, font);
	ScreenText* pauseAudioText = new ScreenText(window, font);
	ScreenText* pauseSfxText = new ScreenText(window, font);
	ScreenText* pauseVolUp = new ScreenText(window, font);
	ScreenText* pauseVolDown = new ScreenText(window, font);
	ScreenText* menuText = new ScreenText(window, font);
	ScreenText* saveText = new ScreenText(window, font);
	ScreenText* pauseQuitText = new ScreenText(window, font);

	//Set up text, position and size
	continueText->setText("CONTINUE", 100, 50, 28);
	pauseAudioText->setText("MUTE AUDIO", 100, 100, 28);
	pauseSfxText->setText("MUTE SFX", 100, 150, 28);
	pauseVolUp->setText("VOLUME UP", 100, 200, 28);
	pauseVolDown->setText("VOLUME DOWN", 100, 250, 28);
	menuText->setText("RETURN TO MENU", 100, 300, 28);
	saveText->setText("SAVE GAME", 100, 350, 28);
	pauseQuitText->setText("QUIT", 100, 400, 28);

	//Add to the list of menu items
	pauseItems.push_back(continueText);
	pauseItems.push_back(pauseAudioText);
	pauseItems.push_back(pauseSfxText);
	pauseItems.push_back(pauseVolUp);
	pauseItems.push_back(pauseVolDown);
	pauseItems.push_back(menuText);
	pauseItems.push_back(saveText);
	pauseItems.push_back(pauseQuitText);
}

//Reset play state
void GameManager::resetPlay()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Resetting play]");

	//Change to the menu game state
	backgroundSprites("./content/menu.png");

	//Clean up the game objects
	if (player1 != NULL)
	{
		player1->clean();
		delete player1;
		player1 = NULL;
	}

	//Clean up the game objects
	if (player2 != NULL)
	{
		player2->clean();
		delete player2;
		player2 = NULL;
	}

	//Clean up the game objects
	if (enemySpawner != NULL)
	{
		enemySpawner->clean();
		delete enemySpawner;
		enemySpawner = NULL;
	}
}

//Accessor to the gameState variable
void GameManager::state(unsigned int s)
{
	gameState = s;
}








//Load background sprites
void GameManager::backgroundSprites(const char* path)
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

		if (texture == NULL)
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Unable to create texture from %s! SDL Error: %s\n]", path, IMG_GetError());
		}
	}
}

//Render background sprites
void GameManager::renderBackgrounds()
{
	//The src rect starts at (0, 0)
	SDL_Rect src = { 0, 0, loadedSurface->w, loadedSurface->h };

	//The dest rect (where to show it)
	SDL_Rect dest = { 0, 0, winWidth, winHeight };

	//Render the sprite
	SDL_RenderCopy(renderer, texture, &src, &dest);
}

//Colision detection function
bool GameManager::checkCollision(SDL_Rect col1, SDL_Rect col2)
{
	//The sides of the colliders
	int left1, left2, right1, right2, top1, top2, bottom1, bottom2;

	//Calculate the sides of collider 1
	left1 = col1.x; right1 = col1.x + col1.w; top1 = col1.y; bottom1 = col1.y + col1.h;

	//Calculate the sides of collider 2
	left2 = col2.x; right2 = col2.x + col2.w; top2 = col2.y; bottom2 = col2.y + col2.h;

	//If any of the sides from collider 1 are outside of collider 2
	if (bottom1 <= top2) return false;
	if (top1 >= bottom2) return false;
	if (right1 <= left2) return false;
	if (left1 >= right2) return false;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected]");

	//If none of the sides from collider 1 are outside collider 2
	return true;
}

//Bullet - enemy collision detection response
void GameManager::bulletCollisions(Player* p)
{
	//For each enemy in the enemy controller
	for (auto &enemy : enemySpawner->enemies)
	{
		//For each bullet in the bullet controller
		for (auto &bullet : p->bulletSpawner->bullets)
		{
			//If the bullet isn't moving continue
			if (bullet->velocity.Y == 0 && bullet->velocity.X == 0)
				continue;

			//Check for collisions between the bullet and enemy
			if (checkCollision(bullet->boxCollider, enemy->boxCollider))
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected between bullet and enemy]");

				//Reset the enemy position and play sfx
				enemy->death->x = enemy->position.X;
				enemy->death->y = enemy->position.Y;
				enemy->position = enemySpawner->randomSpawn(winWidth, winHeight);
				soundManager->playSound(3, sfx);
				soundManager->playSound(7, sfx);

				//Increase player score and reset bullet
				p->score += 10;
				bullet->velocity = Vector2(0, 0);
				p->scoreText->setText(std::to_string(p->playerNum) + "P Score: " + std::to_string(p->score));
			}
		}
	}
}

//Player - enemy collision detection response
void GameManager::playerCollisions(Player* p)
{
	//For each enemy in enemy controller
	for (auto &enemy : enemySpawner->enemies)
	{
		//Check for collisions between enemy and player
		if (checkCollision(enemy->boxCollider, p->boxCollider))
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected between player and enemy]");

			//Decrease player health
			p->health--;
			soundManager->playSound(5, sfx);
			soundManager->playSound(6, sfx);
			
			//Set player position and enemy velocity
			p->position += enemy->velocity * 10.0f;
			enemy->velocity = enemy->velocity * -1.0f;
			p->healthText->setText(std::to_string(p->playerNum) + "P Health: " + std::to_string(p->health) + " / 100");
		}
	}
}

//Enemy - enemy collision detection response
void GameManager::enemyCollisions()
{
	//For each enemy in enemy controller
	for (auto &enemy1 : enemySpawner->enemies)
	{
		//For each enemy in enemy controller
		for (auto &enemy2 : enemySpawner->enemies)
		{
			//If the enemies are the same enemy continue
			if (enemy1 == enemy2)
				continue;
			
			//Check for collisions between enemy and and other enemy
			if (checkCollision(enemy1->boxCollider, enemy2->boxCollider))
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected between enemy and enemy]");

				//Set enemies position / velocity
				enemy1->position += enemy2->velocity;
				enemy2->velocity = enemy1->velocity * -1.0f;
			}
		}
	}
}







//Function to get the window properties
void GameManager::windowProperties(int &x, int &y, int &w, int &h)
{
	w = winWidth; h = winHeight; x = winPosX; y = winPosY;
}

//Function to get the window dimensions
void GameManager::windowDimensions(int &w, int &h)
{
	w = winWidth; h = winHeight;
}

//Function to return game play renderer
SDL_Renderer* GameManager::getRenderer() const
{ 
	return renderer;
}

//Function to return game play window
SDL_Window* GameManager::getWindow() const
{ 
	return window;
}

//Function formoving the window
bool GameManager::moveWindow()
{
	//Get the window position
	SDL_GetWindowPosition(window, &winPosX, &winPosY);
	return true;
}

//Function for if the window has been moved
bool GameManager::windowMoved()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Window moving]");

	//Set the window position
	SDL_SetWindowPosition(window, winPosX, winPosY);

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Window position: x = %i, y = %i]", winPosX, winPosY);

	//The window has been resized
	return false;
}

//Function to maintain the aspect ratio of the window
bool GameManager::maintainAspectRatio(SDL_Event& e)
{
	//Store the temp values for the window width and height
	float tempWidth = e.window.data1;
	float tempHeight = e.window.data2;

	//Store the temp aspect ratio
	float tempRatio = tempWidth / tempHeight;

	//If the temp aspect ratio is not the same as the desired aspect ratio
	if (tempRatio != aspectRatio)
	{
		if (tempHeight > tempWidth)	tempWidth = tempHeight;
		else tempHeight = tempWidth;
	}

	//Set the new window size 
	winWidth = (int)tempWidth;
	winHeight = (int)tempHeight;

	return true;
}

//Function for if the window has been resized
bool GameManager::windowResized()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Resizing window]");

	//Set the window size
	SDL_SetWindowSize(window, winWidth, winHeight);

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Window resized: width = %i, height = %i, aspect ratio = %f]",
			winWidth, winHeight, (float)winWidth / (float)winHeight);

	return false;
}

//Function for toggle fullscreen
void GameManager::toggleFullscreen()
{
	//If in full screen
	if (fullScreen)
	{
		//Set the window not to full screen
		SDL_SetWindowFullscreen(window, false);
		fullScreen = false;
		SDL_GetWindowSize(window, &winWidth, &winHeight);
	}
	else
	{
		//Else set the window to full screen
		SDL_SetWindowFullscreen(window, true);
		fullScreen = true;
		SDL_GetWindowSize(window, &winWidth, &winHeight);
	}
}
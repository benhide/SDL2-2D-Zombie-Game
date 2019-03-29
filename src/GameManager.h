#ifndef _GAME_MANAGER_H
#define _GAME_MANAGER_H

//SDL and custom headers
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "ScreenText.h"
#include "Player.h"
#include "EnemyController.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "Keys.h"

//Forward class declaration
class InputManager;


//Game Manager class
class GameManager
{
public:
	//Game Manager singleton
	static GameManager* instance();

	//Game Manager initialisation function
	bool init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, bool debug);

	//Game manager game loop functions
	void input();
	void update();
	void render();
	void quit();

	//Game loop running and clean function
	bool running();
	void clean();

	//Game state initialisation function
	void initMenu();
	void initOptions();
	void initPlay();
	void initPause();
	void resetPlay();
	void state(unsigned int s);

	//Function to render background images
	void backgroundSprites(const char* path);
	void renderBackgrounds();

	//Collision detection functions
	bool checkCollision(SDL_Rect col1, SDL_Rect col2);
	void playerCollisions(Player* p);
	void enemyCollisions();
	void bulletCollisions(Player* p);

	//Function to return game window and renderer
	SDL_Renderer* getRenderer() const;
	SDL_Window* getWindow() const;

	//Function to get the window properties and dimensions
	void windowProperties(int &x, int &y, int &w, int &h);
	void windowDimensions(int &w, int &h);

	//Window position, scale fullscreen functions
	bool moveWindow();
	bool windowMoved();
	bool maintainAspectRatio(SDL_Event& e);
	bool windowResized();
	void toggleFullscreen();

	//Sound manager reference
	SoundManager* soundManager = NULL;

	//Input manager reference
	InputManager* inputManager = NULL;

	//Game state enum
	enum state { menu, options, pause, play, splash	};

	//Pointers to the SDL window and renderer objects
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	//
	SDL_Surface* loadedSurface = NULL;
	SDL_Texture* texture = NULL;

	//Font file path
	std::string font;

	//The list of states text items and variables
	std::vector<ScreenText*> menuItems;
	std::vector<ScreenText*> optionsItems;
	std::vector<ScreenText*> pauseItems;
	ScreenText* logScreen = NULL;

	//The list of states text variables
	int currentMenuIndex;
	int currentOptionsIndex;
	int currentPauseIndex;

	//Number of players
	int playerNumb;

	//Player1 game object
	Player* player1 = NULL;
	Player* player2 = NULL;

	//EnemyController game object
	EnemyController* enemySpawner = NULL;

	//The game state
	int gameState;

	//Game sound booleans
	bool audio = true, sfx = true;

	//Game paused boolean
	bool paused = false;

	//Joystick connected
	bool connectedJoystick = false;

	//Loading of player
	bool loadPlayer = false;

	//Player speed variable
	float player1Speed = 0;

private:
	//Static instance of the Game Class
	static GameManager* gm_instance;

	//Defualt constructor
	GameManager();

	//Defualt destructor
	~GameManager();

	//Game loop, fullscreen, resized window and debug booleans
	bool gameRunning = false, fullScreen = false, debug = false;

	//Window size and position booleans
	bool winResized = false, winMoved = false;

	//Game width, height, x pos, y pos
	int winWidth, winHeight, winPosX, winPosY;

	//ID of the window
	Uint32 winID;

	//Aspect ratio of the window
	float aspectRatio = 1;
};
#endif
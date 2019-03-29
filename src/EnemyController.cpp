#include "EnemyController.h"

//Defualt constructor
EnemyController::EnemyController(Vector2 pos, int number, bool db) : position(pos), numberOfObjects(number), debug(db)
{
	//Random seed
	srand((unsigned)time(NULL));

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy controller GameObject constructed!]");
}

//Defualt destructor
EnemyController::~EnemyController()
{
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destroying Enemy controller GameObject]");
}

//Add enemy function
void EnemyController::add(SDL_Window* window, Vector2 pos, int w, int h)
{
	//Creat a new enemy and add to the enemies vector, then initialise the enemy
	Enemy* enemy = new Enemy(pos, w, h, debug);
	this->enemies.push_back(enemy);
	this->enemies.back()->init(window);
}

//Initialisation function
void EnemyController::init(SDL_Window* window, int winW, int winH)
{
	//Loop through the number of enemies to create
	for (unsigned int i = 0; i < numberOfObjects; i++)
	{
		//Set the enemies spawn point
		Vector2 pos = randomSpawn(winW, winH);
		
		//Add a new enemy
		add(window, pos, 35, 35);
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy controller GameObject initialised]");
}

//Render function
void EnemyController::render(SDL_Renderer* renderer)
{
	//Loop through each enemy
	for (auto& enemy : enemies)
	{
		//Render the enemy
		enemy->render(renderer);
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering enemies]");
}

//Update function
void EnemyController::update(Vector2 target1, Vector2 target2)
{
	//Loop through each enemy
	for (auto& enemy : enemies)
	{
		//Update the enemy
		enemy->update(target1, target2);
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating enemies]");
}

//Clean function
void EnemyController::clean()
{
	//While there are enemies to clean
	while (!enemies.empty())
	{
		//Clean up the enemy
		enemies.back()->clean();
		delete enemies.back();
		enemies.back() = NULL;
		enemies.pop_back();
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy controller GameObject destroyed]");
}

//Random spawn function
Vector2 EnemyController::randomSpawn(int winW, int winH)
{
	//Set the x and y position
	int xpos = 0, ypos = 0;

	//Choose a random spawn location
	int spawn = rand() % 4;

	//Set a random spawn position for the location
	if (spawn == 0) { xpos = -50; ypos = rand() % winH; }
	else if (spawn == 1) { xpos = winW + 50; ypos = rand() % winH; }
	else if (spawn == 2) { ypos = -50; xpos = rand() % winW; }
	else if (spawn == 3) { ypos = winH + 50; xpos = rand() % winW;}

	//Return the random spawn position
	return Vector2((float)xpos, (float)ypos);
}

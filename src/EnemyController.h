#ifndef _ENEMY_CONTROLLER_H
#define _ENEMY_CONTROLLER_H

//SDL and custom headers
#include <SDL.h>
#include <vector>
#include <stdlib.h>
#include <time.h> 
#include "Enemy.h"
#include "Vector2.h"

//Enemey controller class
class EnemyController
{
public:
	//Defualt constructor
	EnemyController(Vector2 pos, int number, bool db);

	//Defualt destructor
	~EnemyController();

	//Add enemy function
	void add(SDL_Window* window, Vector2 pos, int w, int h);

	//Game loop functions
	void init(SDL_Window* window, int winW, int winH);
	void render(SDL_Renderer* renderer);
	void update(Vector2 target1, Vector2 target2);
	void clean();

	//Random spawn postion function
	Vector2 randomSpawn(int winW, int winH);

	//Number of enemies
	unsigned int numberOfObjects;

	//Vector of enemies
	std::vector<Enemy*> enemies;

	//Position of controller
	Vector2 position;

	//Debug mode
	bool debug = false;
};
#endif 
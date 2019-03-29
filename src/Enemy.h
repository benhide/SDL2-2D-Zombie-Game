#ifndef _ENEMY_H
#define _ENEMY_H

//Define pi
#define PI 3.14159265

//C++ Headers
#include <math.h>

//SDL and custom headers
#include <SDL.h>
#include "Vector2.h"
#include "Sprite.h"

//Enemy class 
class Enemy 
{
public:
	//Defualt constructor
	Enemy(Vector2 pos, int w, int h, bool db, int r = 0, int g = 0, int b = 0, int a = 255);

	//Defualt destructor
	~Enemy();

	//Game loop functions
	void init(SDL_Window* window);
	void update(Vector2 target1, Vector2 target2);
	void render(SDL_Renderer* renderer);
	void clean();

	//SDL rect
	SDL_Rect boxCollider;

	//Enemey sprite
	Sprite* sprite = NULL;
	Sprite* death = NULL;

	//Motion vectors and floats
	Vector2 position;
	Vector2 velocity;
	float speed;

	//Game object width and height
	int width, height;

	//RGB of game object
	int red, green, blue, alpha;

	//Debug mode active
	bool debug = false;
};
#endif
#ifndef _BULLET_H
#define _BULLET_H

//SDL and custom headers
#include <SDL.h>
#include "Vector2.h"

//Bullet class
class Bullet
{
public:
	//Defualt constructor
	Bullet(Vector2 pos, int w, int h, int r = 0, int g = 0, int b = 0, int a = 255);

	//Defualt destructor
	~Bullet();

	//Game loop functions
	void init();
	void update();
	void render(SDL_Renderer* renderer);
	void clean();

	//Sdl rect 
	SDL_Rect boxCollider;

	//Motion vectors and floats
	Vector2 position;
	Vector2 velocity;
	float speed;

	//Game object width and height
	int width, height;

	//RGB of game object
	int red, green, blue, alpha;

	//Bullet timer/counter
	unsigned int counter;
};
#endif
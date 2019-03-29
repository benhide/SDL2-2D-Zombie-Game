#ifndef _BULLET_SPAWNER_H
#define _BULLET_SPAWNER_H

//SDL and custom headers
#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "LogManager.h"
#include "SoundManager.h"

//Bullet spawner class
class BulletSpawner
{
public:
	//Defualt constructor
	BulletSpawner(Vector2 pos, int number);

	//Defualt destructor
	~BulletSpawner();

	//Add bullet function
	void add(Vector2 pos, int w, int h);

	//Game loop functions
	void init();
	void render(SDL_Renderer* renderer, bool dead);
	void update();
	void shoot(Vector2 vec, bool sfx);
	void clean();

	//Number of bullets
	unsigned int numberOfObjects;

	//Vector of bullets
	std::vector<Bullet*> bullets;

	//Position of controller
	Vector2 position;

	//Last vector bullet was fired
	Vector2 lastvec;

	//Soundmanager reference
	SoundManager* soundManager = NULL;
};
#endif 
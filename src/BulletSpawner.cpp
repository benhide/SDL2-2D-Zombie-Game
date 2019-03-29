#include "BulletSpawner.h"


//Defualt constructor
BulletSpawner::BulletSpawner(Vector2 pos, int number) : position(pos), numberOfObjects(number)
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Bullet spawner constructed]");
}

//Defualt destructor
BulletSpawner::~BulletSpawner()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destorying bullet spawner]");
}

//Add bullet function
void BulletSpawner::add(Vector2 pos, int w, int h)
{
	Bullet* bullet = new Bullet(pos, w, h);
	this->bullets.push_back(bullet);
	this->bullets.back()->init();
}

//Initialisation function
void BulletSpawner::init()
{
	//Loop through the number of bullets to create
	for (unsigned int i = 0; i < numberOfObjects; i++)
	{
		//Add a new bullet
		add(Vector2(position.X, position.Y), 6, 6);
	}

	//Locate the sound manager
	soundManager = SoundManager::instance();
	
	//Assign the lastvec
	lastvec = Vector2(0, 1);
}

//Render function
void BulletSpawner::render(SDL_Renderer* renderer, bool dead)
{
	//Loop through each bullet
	for (auto& bullet : bullets)
	{
		//If the bullets moving render it
		if (bullet->velocity.Y != 0 || bullet->velocity.X != 0)
		{
 			if (!dead)
			{
				bullet->render(renderer);
			}
		}
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_INFO, "[Rendering bullet GameObjects]");
}

//Update function
void BulletSpawner::update()
{
	//Loop through the bullets
	for (auto& bullet : bullets)
	{
		//Update the bullets
		bullet->update();

		//If the bullet is moving increase the counter/timer
		if (bullet->velocity.Y != 0 || bullet->velocity.X != 0)
			bullet->counter++;
		
		//Else set the bullet position to the controller position
		else if (bullet->velocity.Y == 0 && bullet->velocity.X == 0)
		{
			bullet->position.X = position.X;
			bullet->position.Y = position.Y;
		}

		//Ifth ecounter is greater than 50
		if (bullet->counter > 50)
		{
			//Reset the counter and the bullet
			bullet->counter = 0;
			bullet->position.X = position.X;
			bullet->position.Y = position.Y;
			bullet->velocity.Y = 0;
			bullet->velocity.X = 0;
		}
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating bullet GameObjects]");
}

//Shoot function
void BulletSpawner::shoot(Vector2 vec, bool sfx)
{
	int i = 0;
	bool notFound = true;

	while ((i < bullets.size()) && (notFound))
	{
		if (bullets[i]->velocity.Y == 0 && bullets[i]->velocity.X == 0)
		{
			bullets[i]->counter = 0;

			if (vec.X == 0 && vec.Y == 0) vec = lastvec;
			else lastvec = vec;

			bullets[i]->velocity = vec;
			soundManager->playSound(4, sfx);
			notFound = false;
		}
		i++;
	}
}

//Clean function
void BulletSpawner::clean()
{
	//Clean up bullets game objects
	while (!bullets.empty())
	{
		bullets.back()->clean();
		delete bullets.back();
		bullets.back() = NULL;
		bullets.pop_back();
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Bullet spawner destroyed]");
}

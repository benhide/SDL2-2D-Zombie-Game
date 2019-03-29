#include "Enemy.h"

//Defualt constructor
Enemy::Enemy(Vector2 pos, int w, int h, bool db, int r, int g, int b, int a) : position(pos), width(w), height(h), debug(db), red(r), green(g), blue(b), alpha(a)
{
	//Create a new rect
	boxCollider.w = width; boxCollider.h = height; boxCollider.x = (int)position.X; boxCollider.y = (int)position.Y;

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy GameObject constructed!]");
}

//Defualt destructor
Enemy::~Enemy()
{
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destroying Enemy GameObject]");
}

//Initialisation function
void Enemy::init(SDL_Window* window)
{
	//Init the velocity and acceleration
	velocity.X = 0;	velocity.Y = 0.0f;
	speed = 0.85f;

	//Colour of enemy
	red = 255;

	//Load in sprites
	sprite = new Sprite((int)position.X, (int)position.Y, 288, 311, SDL_GetRenderer(window), 75, 0.25f);
	sprite->setNumberOfFrames(8);
	sprite->load("./content/zombie.png");

	//Load in sprites
	death = new Sprite(-200, -200, 152, 152, SDL_GetRenderer(window), 1, 0.35f);
	death->setNumberOfFrames(1);
	death->load("./content/bloodsplatter.png");

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy GameObject initialised]");
}

//Update function
void Enemy::update(Vector2 target1, Vector2 target2)
{
	//Temp vector2 target
	Vector2 target;

	//Select the target
	if (Vector2(abs(target1.X - position.X), abs(target1.Y - position.Y)).length() <= Vector2(abs(target2.X - position.X), abs(target2.Y - position.Y)).length())
		target = target1;
	else
		target = target2;

	//Lerp the enemy towards the player by the percentage
	velocity = velocity.lerp(position, target, 50.0f);

	//Calculate the enemy position
	velocity.normalize();
	velocity *= speed;
	position += velocity;

	//Assign the rect position / dimensions
	boxCollider.x = (int)position.X;
	boxCollider.y = (int)position.Y;

	//If the enemy is moving update the sprite
	if (velocity.X != 0 || velocity.Y != 0)	sprite->update();
}

//Render Function
void Enemy::render(SDL_Renderer * renderer)
{
	//If in debug mode render collider
	if (debug)
	{
		//Set the draw colour
		SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

		//Fill the rect
		SDL_RenderFillRect(renderer, &boxCollider);
	}

	//Render the sprites
	death->render(death->x, death->y, 0.0f);
	sprite->render((int)position.X - (boxCollider.w / 2), (int)position.Y - (boxCollider.h / 2), (atan2(velocity.X, -velocity.Y) * 180 / PI) - 90);
}

//Clean function
void Enemy::clean()
{

	//Delete the player objects
	if (sprite != NULL)
	{
		sprite->clean();
		delete sprite;
		sprite = NULL;
	}

	//Delete the player objects
	if (death != NULL)
	{
		death->clean();
		delete death;
		death = NULL;
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Enemy destroyed!]");
}
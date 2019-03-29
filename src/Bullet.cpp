#include "Bullet.h"


//Defualt constructor
Bullet::Bullet(Vector2 pos, int w, int h, int r, int g, int b, int a) : position(pos), width(w), height(h), red(r), green(g), blue(b), alpha(a)
{
	//Create a new rect
	boxCollider.w = width; boxCollider.h = height; boxCollider.x = (int)position.X; boxCollider.y = (int)position.Y;

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Bullet GameObject constructed!]");
}

//Defualt destructor
Bullet::~Bullet()
{
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destroying bullet GameObject]");
}

//Initialisation function
void Bullet::init()
{
	//Init the velocity
	velocity.X = 0;	velocity.Y = 0;
	speed = 20.0f;
	red = 255; green = 255;

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Bullet GameObject initialised]");

}

//Update function
void Bullet::update()
{
	//Calculate the bullet position
	velocity.normalize();
	velocity *= speed;
	position += velocity;

	//Assign the rect position
	boxCollider.x = (int)position.X;
	boxCollider.y = (int)position.Y;
}

//Render function
void Bullet::render(SDL_Renderer * renderer)
{
	//Set the draw colour
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

	//Fill the rect
	SDL_RenderFillRect(renderer, &boxCollider);
}

//Clean function
void Bullet::clean()
{
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Bullet destroyed!]");
}

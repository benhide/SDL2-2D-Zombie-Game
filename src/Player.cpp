#include "Player.h"

//Defualt constructor
Player::Player(Vector2 pos, int w, int h, bool db, int r, int g, int b, int a) : position(pos), width(w), height(h), debug(db), red(r), green(g), blue(b), alpha(a)
{
	//Create a new rect
	boxCollider.w = width; boxCollider.h = height; boxCollider.x = (int)position.X; boxCollider.y = (int)position.Y;

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Player GameObject constructed!]");
}

//Defualt destructor
Player::~Player()
{
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Destroying Player GameObject]");
}

//Initialisation function
void Player::init(SDL_Window* window, std::string font, int numb, bool loading)
{
	//Initialise the bullet spawner object and conditions
	bulletSpawner = new BulletSpawner(position, 10);
	bulletSpawner->init();

	//Player health
	playerNum = numb;
	if (!loading)
	{
		health = 100;
		score = 0;
	}

	//Initialize both menu items load in fonts
	healthText = new ScreenText(window, font);
	healthText->setText(std::to_string(playerNum) + "P Health: " + std::to_string(health) + " / 100", (numb*numb*numb) * 25, 25, 16);

	//Initialize both menu items load in fonts
	scoreText = new ScreenText(window, font);
	scoreText->setText(std::to_string(playerNum) + "P Score: " + std::to_string(score), (numb*numb*numb) * 25, 50, 16);

	//Initialize both menu items load in fonts
	playerTag = new ScreenText(window, font);
	playerTag->setText("Player " + std::to_string(playerNum), (int)(position.X - (width / 2)), (int)(position.Y + height), 16);

	//Init the velocity and acceleration
	velocity.X = 0;	velocity.Y = 0;
	speed = 5.0f;

	//Colour of the player
	red = 255, green = 255, blue = 255, alpha = 255;

	//Load in sprites
	sprite = new Sprite((int)position.X, (int)position.Y, 258, 220, SDL_GetRenderer(window), 5, 0.25f);
	sprite->setNumberOfFrames(20);
	sprite->load("./content/survivor.png");

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Player GameObject initialised]");
}

//Update function
void Player::update(int w, int h, float s, bool joyStick)
{
	if (!joyStick)
	{
		//Udate player position
		if (KEY_UP) velocity.Y = -1.0f;
		if (KEY_DOWN) velocity.Y = 1.0f;
		
		if (!KEY_UP && velocity.Y < 0) velocity.Y = 0;
		if (!KEY_DOWN && velocity.Y > 0) velocity.Y = 0;
		
		if (KEY_LEFT) velocity.X = -1.0f;
		if (KEY_RIGHT) velocity.X = 1.0f;
		
		if (!KEY_LEFT && velocity.X < 0) velocity.X = 0;
		if (!KEY_RIGHT && velocity.X > 0) velocity.X = 0;

		//Set the player speed
		speed = 5.0f;
	}
	else
	{
		//Set the players speed
		speed = s / 2.0f;
	}

	//Calculate the player position
	velocity.normalize();
	velocity *= speed;
	position += velocity;

	//Update the bullet spawner position
	bulletSpawner->position.X = position.X + ((width / 2) - 5);
	bulletSpawner->position.Y = position.Y + ((height / 2) - 5);

	//Update the bullets 
	bulletSpawner->update();

	//Wall collision dectetion and response with player
	if ((position.X <= 0) || position.X + boxCollider.w >= w)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected between player and game boundaries]");

		position.X -= velocity.X * 5.0f;
	}

	//Wall collision detection and response with player
	if ((position.Y <= 0) || position.Y + boxCollider.h >= h)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Collision dectected between player and game boundaries]");

		position.Y -= velocity.Y * 5.0f;
	}

	//Assign the rect position
	boxCollider.x = (int)position.X;
	boxCollider.y = (int)position.Y;

	//Assign the playerTag position
	playerTag->move((int)position.X - (width / 2), (int)position.Y + height);

	//If the player isn't moving don't animate the player
	if (velocity.X != 0 || velocity.Y != 0)
	{
		//Don't update sprite
		sprite->update();

		//Set the bullet spawner to shoot in the last facing direction of fire
		lastVec = bulletSpawner->lastvec;
	}
	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Updating Player GameObject]");

}

//Render function
void Player::render(SDL_Renderer* renderer, bool dead)
{
	//Render the bullet spawner
	bulletSpawner->render(renderer, dead);

	//Render score text
	scoreText->render();
	healthText->render();
	playerTag->render();

	//If in debug mode render collider
	if (debug)
	{
		//Set the draw colour
		SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

		//Fill the rect
		SDL_RenderFillRect(renderer, &boxCollider);
	}


	//Render the player
	if (velocity.X == 0 && velocity.Y == 0) sprite->render((int)position.X - (boxCollider.w / 2), (int)position.Y - (boxCollider.h / 2), (atan2(lastVec.X, -lastVec.Y) * 180 / PI) - 90.0);
	else sprite->render((int)position.X - (boxCollider.w / 2), (int)position.Y - (boxCollider.h / 2), (atan2(velocity.X, -velocity.Y) * 180 / PI) - 90);

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Rendering Player GameObject]");

}

//Serialisation function
void Player::serialize(const std::string& path) const
{
	//The output stream (to a file)
	std::ofstream outStream;

	//Open the file for writing
	outStream.open(path, std::ios::out);

	//Create an empty json object that we're going to save
	json outObject;

	//Set up fields to save
	outObject["health"] = this->health;
	outObject["score"] = this->score;
	outObject["position"]["x"] = this->position.X;
	outObject["position"]["y"] = this->position.Y;

	//Write to the file -- use dump to get the JSON text
	outStream << outObject.dump(4);

	//And close the file! Very important!
	outStream.close();
}

//De-serialisation function
void Player::deserialize(const std::string& path)
{
	//The input stream (to a file)
	std::ifstream inStream;

	//Open the file for reading
	inStream.open(path, std::ios::in);

	//Build an empty JSON object 
	json j;

	//Read all JSON data into object
	inStream >> j;

	//Set the instance variables of the player to what is parsed by the JSON object
	this->health = j["health"].get<int>();
	this->score = j["score"].get<int>();
	this->position.X = j["position"]["x"].get<float>();
	this->position.Y = j["position"]["y"].get<float>();


	//And close the file
	inStream.close();
}

//Clean function
void Player::clean()
{
	//Delete the player objects
	if (bulletSpawner != NULL)
	{
		bulletSpawner->clean();
		delete bulletSpawner;
		bulletSpawner = NULL;
	}

	//Delete the player objects
	if (scoreText != NULL)
	{
		delete scoreText;
		scoreText = NULL;
	}

	//Delete the player objects
	if (healthText != NULL)
	{
		delete healthText;
		healthText = NULL;
	}

	//Delete the player objects
	if (playerTag != NULL)
	{
		delete playerTag;
		playerTag = NULL;
	}

	//Delete the player objects
	if (sprite != NULL)
	{
		sprite->clean();
		delete sprite;
		sprite = NULL;
	}

	//Log info
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Player destroyed!]");
}
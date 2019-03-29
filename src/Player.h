#ifndef _PLAYER_H
#define _PLAYER_H

//Define pi
#define PI 3.14159265

//C++ Headers
#include <math.h>
#include <fstream>

//SDL and custom headers
#include <SDL.h>
#include "Vector2.h"
#include "BulletSpawner.h"
#include "ScreenText.h"
#include "Sprite.h"
#include "json.hpp"

//Using declaration
using json = nlohmann::json;

//Player class
class Player
{
public:
	//Defualt constructor
	Player(Vector2 pos, int w, int h, bool db, int r = 0, int g = 0, int b = 0, int a = 255);

	//Defualt destructor
	~Player();

	//Initialisation function
	void init(SDL_Window* window, std::string font, int numb, bool loading);
	void update(int w, int h, float s, bool joyStick);
	void render(SDL_Renderer* renderer, bool dead);
	void clean();

	//Serialisation function
	void serialize(const std::string& path) const;
	void deserialize(const std::string& path);

	//SDL rect
	SDL_Rect boxCollider;

	//Key presses
	bool KEY_UP = false;
	bool KEY_DOWN = false;
	bool KEY_LEFT = false;
	bool KEY_RIGHT = false;
	bool DASH = false;

	//BulletSpawner game object
	BulletSpawner* bulletSpawner = NULL;

	//Player health and score text
	ScreenText* healthText = NULL;
	ScreenText* scoreText = NULL;
	ScreenText* playerTag = NULL;

	//PLayer sprite
	Sprite* sprite = NULL;

	//Motion vectors and floats
	Vector2 position;
	Vector2 velocity;
	Vector2 lastVec;
	float speed;

	//Game object width and height
	int width, height;

	//RGB of game object
	int red, green, blue, alpha;

	//Player health
	int health;
	int score;
	int playerNum;

	//Debug mode active
	bool debug = false;
};
#endif 
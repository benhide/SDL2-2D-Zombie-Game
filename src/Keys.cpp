#include "Keys.h"


//Defualt constructor
Keys::Keys(){}

//Default Destructor
Keys::~Keys(){}

//Load keys functon
void Keys::loadKeys(const std::string& path)
{
	//The input stream (to a file)
	std::ifstream inStream;

	//Open the file for reading
	inStream.open(path, std::ios::in);

	//Build an empty JSON object that we're going to save into
	json j;

	//Read all JSON data into this object..
	//json.hpp will do the parsing!
	inStream >> j;

	//Get the fire and jump keys.. set them for this instance
	this->shoot = SDL_GetKeyFromName(j["shoot"].get<std::string>().c_str());
	this->left = SDL_GetKeyFromName(j["left"].get<std::string>().c_str());
	this->right = SDL_GetKeyFromName(j["right"].get<std::string>().c_str());
	this->up = SDL_GetKeyFromName(j["up"].get<std::string>().c_str());
	this->down = SDL_GetKeyFromName(j["down"].get<std::string>().c_str());

	//And close the file
	inStream.close();
}

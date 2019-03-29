/**
* @file   main.cpp
* @author Ben Hide (16605093@students.lincoln.ac.uk / ben.m.hide@googlemail.com)
* @brief  The main entry point for "THE STALKING DEAD!!" game.
*/

//ScreenText header and definition
#define STB_TRUETYPE_IMPLEMENTATION
#include "ScreenText.h"

//C++ Headers
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>

//SDL and custom headers
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "GameManager.h"
#include "LogManager.h"

//Std namespace
using namespace std;

//Window width and window height
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

//Set the game fps and frame delay time
const unsigned int fps = 60;
const unsigned int frameDelay = 1000 / fps;

//Function prototype for cleaning the game objects
void cleanGame(GameManager* gm);

//Entry point for the program
int main(int argc, char *argv[])
{
	//Set the debug mode
	bool debug = false;

	//Set the log output function
	SDL_LogSetOutputFunction(&LogManager::logData, NULL);

	//Game instance
	GameManager* gameManager = GameManager::instance();

	//Use debug mode
	printf("Would you like to use debug mode? y = yes / n = no\n");

	//If the game is in debug mode initialise the log manager
	cin.get() == 'y' ? debug = true : debug = false;
	if (LogManager::init(debug))
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,"[Game initialisation attempted]");

		//If creating a new game is successful
		if (gameManager->init("THE STALKING DEAD!! - Games Programming (CGP2011M-2-1718) Level 2 - Ben Hide - ID: 16605093", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, false, debug))
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Game initialisation Successful]");

			//Number of frames
			Uint32 frameNumber = 0, frameStart, frameTime, inputTime, updateTime, renderTime;

			//Change to the splash screen game state
			gameManager->state(4);
			gameManager->render();

			//Delay the menu screen
			SDL_Delay(2000);
			
			//Change to the menu game state
			gameManager->backgroundSprites("./content/menu.png");
			gameManager->state(0);

			//Game loop - while the game is running
			while (gameManager->running())
			{
				//Set the frame start time
				frameStart = inputTime = updateTime = renderTime = SDL_GetTicks();

				//Handle game events
				gameManager->input();

				//Calculate the input time
				inputTime = SDL_GetTicks() - frameStart;

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Frame number:%i - Input time:%i ms]", frameNumber, inputTime);

				//Update the game
				gameManager->update();

				//Calculate the update time
				updateTime = SDL_GetTicks() - frameStart - inputTime;

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Frame number:%i - Update time:%i ms]", frameNumber, updateTime);

				//Render to the screen
				gameManager->render();

				//Calculate the update time
				renderTime = SDL_GetTicks() - frameStart - inputTime - updateTime;

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Frame number:%i - Render time:%i ms]", frameNumber, renderTime);

				//Calculate the frame time
				frameTime = SDL_GetTicks() - frameStart -  inputTime - updateTime - renderTime;

				//If the frame time is less than the dealy time - add a delay
				if (frameTime < frameDelay)
					SDL_Delay((int)(frameDelay - frameTime));
				
				//Increase number of frames
				frameNumber++;	

				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Frame number:%i - Frame time:%i ms]", frameNumber, frameTime);
			}
		}
		else
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Game initialisation error, quiting game];[Error:%s]", SDL_GetError());

			//Game initialisation error
			return -1;
		}
	}
	else
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Log initialisation error, quiting game];[Error:%s]", SDL_GetError());

		//Log initialisation error
		return -1;
	}

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Quiting the Game]");

	//Clean up the game objects
	cleanGame(gameManager);

	//Clean up SDL
	SDL_Quit();
	IMG_Quit();
	Mix_Quit();

	//Exit
	return 0;
}

//Function to clean the game objects
void cleanGame(GameManager* gm)
{
	//Clean up the game objects
	gm->clean();
	LogManager::clean();

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Cleaning the Game]");
}
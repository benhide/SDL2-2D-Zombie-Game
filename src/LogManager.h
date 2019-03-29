#ifndef _LOG_MANAGER_H
#define _LOG_MANAGER_H

//C++ Headers
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>

//SDL and custom headers
#include <SDL.h>
#include "ScreenText.h"

//Log Manager class
class LogManager
{
public:
	//Log Manager functions
	static bool init(bool debug);
	static void logMessage(unsigned int category, unsigned int priority, const char* format, ...);
	static void logData(void* userdata, int category, SDL_LogPriority priority, const char* message);
	static bool debugging();
	static std::string logText();

	//Return function for current time
	static char* getTime();

	//Clean function
	static void clean();

private:
	//Logging boolean
	static bool isDebugging;

	//Log catergory and priority variable
	static unsigned int category;
	static unsigned int priority;

	//Log output files
	static std::ofstream logFiles[8];

	//Log file names
	static std::string fileNames[8];

	//Log screen text string
	static std::string logScreenText;
};
#endif
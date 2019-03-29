#include "LogManager.h"

//Initialise the static members
unsigned int LogManager::category = NULL;
unsigned int LogManager::priority = NULL;
bool LogManager::isDebugging = NULL;

//Initialise the log arrays
std::ofstream LogManager::logFiles[8] {};
std::string LogManager::fileNames[8] = { "app-log.txt", "error-log.txt", "assert-log.txt", "system-log.txt", "audio-log.txt", "video-log.txt", "render-log.txt", "input-log.txt" };
std::string LogManager::logScreenText = "HOLDER";


//Open file function
bool LogManager::init(bool debug)
{
	//Set the debug mode
	isDebugging = debug;

	//If the game is i debug mode
	if (isDebugging)
	{
		for (unsigned int i = 0; i < 8; i++)
		{
			//Open up log file
			LogManager::logFiles[i].open("log/" + fileNames[i]);
			LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Opening log file %s]", fileNames[i].c_str());

			//If the log file couldn't be opened
			if (!logFiles[i].is_open())
			{
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Couldn't open log file:%s];[ERROR:%s]", fileNames[i], SDL_GetError());
				return false;
			}
		}
	}
	return true;
}

//Custom Log Message function
void LogManager::logMessage(unsigned int category, unsigned int priority, const char* format, ...)
{
	//Set the category and priority for the logger
	LogManager::category = category;
	LogManager::priority = priority;

	//The list of arguments to save into (the ...)
	va_list list;

	//Get all the arguments (after format) and put them into list
	va_start(list, format);

	//Format a message using the parameters into the variable "msg"
	char msg[128]; vsprintf_s(msg, format, list);

	//Stop with arguments
	va_end(list);

	//Use SDL log to print out the formatted message
	SDL_Log(msg);
}

//Log data function
void LogManager::logData(void* userdata, int category, SDL_LogPriority priority, const char* message)
{
	//Holders for the log catergory name and priority
	const char* categoryName;
	const char* priorityName;

	//If debugging is enabled
	if (isDebugging)
	{
		//Switch on the log category (write to specific log file and console)
		switch (LogManager::category)
		{
		case SDL_LOG_CATEGORY_APPLICATION: categoryName = "APPLICATION";break;
		case SDL_LOG_CATEGORY_ERROR: categoryName = "ERROR"; break;
		case SDL_LOG_CATEGORY_ASSERT: categoryName = "ASSERT"; break;
		case SDL_LOG_CATEGORY_SYSTEM: categoryName = "SYSTEM"; break;
		case SDL_LOG_CATEGORY_AUDIO: categoryName = "AUDIO"; break;
		case SDL_LOG_CATEGORY_VIDEO: categoryName = "VIDEO"; break;
		case SDL_LOG_CATEGORY_RENDER: categoryName = "RENDER"; break;
		case SDL_LOG_CATEGORY_INPUT: categoryName = "INPUT"; break;
		}

		//Switch on the log priority and set the priority
		switch (LogManager::priority)
		{
		case SDL_LOG_PRIORITY_VERBOSE: priorityName = "VERBOSE"; break;
		case SDL_LOG_PRIORITY_DEBUG: priorityName = "DEBUG"; break;
		case SDL_LOG_PRIORITY_INFO: priorityName = "INFO"; break;
		case SDL_LOG_PRIORITY_WARN: priorityName = "WARN"; break;
		case SDL_LOG_PRIORITY_ERROR: priorityName = "ERROR"; break;
		case SDL_LOG_PRIORITY_CRITICAL:	priorityName = "CRITICAL"; break;
		}

		//Log string
		std::string logger = "[CATEGORY:" + std::string(categoryName) + "];[TIME:" + getTime() + "];[PRIORITY:" + std::string(priorityName) + "];" + std::string(message);

		//Write to the console and the log file
		std::cout << logger << std::endl;
		logFiles[LogManager::category] << logger << std::endl;
		logScreenText = logger;
	}
}

//Function to return if the log manager is in debug mode
bool LogManager::debugging()
{
	return isDebugging;
}

//Accessor to the log screen text
std::string LogManager::logText()
{
	return logScreenText;
}

//Return the current time
char* LogManager::getTime()
{
	//Get the current time
	time_t timeNow = std::time(0);

	//Get time information from current time -- secs, mins, etc. and save into a struct
	struct tm info; localtime_s(&info, &timeNow);

	//Allocate 32 characters in memory to store our date message
	char* timeString = new char[32]();

	//Format the time to (day_num/month_num/year_num time)
	strftime(timeString, 32, "%d/%m/%y %T", &info);

	//Return the time
	return timeString;
}

//Clean function
void LogManager::clean()
{
	LogManager::logMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "[Closing log files]");

	//Loop through the log files, if the log file is open, close the file
	for (unsigned int i = 0; i < 8; i++) if (logFiles[i].is_open())	logFiles[i].close();
}
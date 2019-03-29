#include "SoundManager.h"

//Static instance of the SoundManager class
SoundManager* SoundManager::sm_instance = NULL;

//SoundManager Singleton
SoundManager* SoundManager::instance()
{
	//If an instance of the game class dosen't exsist create a new game
	if (sm_instance == NULL)
		sm_instance = new SoundManager();

	//Return the instance of the game that exsists
	return sm_instance;
}

//SoundManager constructor
SoundManager::SoundManager()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[SoundManager constructed]");
}

//SoundManager destructor
SoundManager::~SoundManager()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Destorying sound manager]");
}

//Initialisation function
bool SoundManager::init()
{
	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Initialising SDL audio]");

	//Initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[SDL audio could not initialize];[SDL Error: %s]", SDL_GetError());
		return false;
	}
	else
	{
		//Initialize SDL_mixer
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[SDL mixer could not initialize];[SDL mixer Error: %s\n]", Mix_GetError());
			return false;
		}

		else
		{
			//Load music
			music = Mix_LoadMUS("./content/theme.mp3");
			if (music == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load music];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeMusic(35);

			//Load sound effect
			menuSound = Mix_LoadWAV("./content/205542__everheat__menusound1.wav");
			if (menuSound == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load menuSound sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(menuSound, 255);

			//Load sound effect
			menuSelect = Mix_LoadWAV("./content/menuselect.wav");
			if (menuSelect == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load menuSelect sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(menuSelect, 255);

			//Load sound effect
			zombieIdle = Mix_LoadWAV("./content/zombieidle.wav");
			if (zombieIdle == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load zombieIdle sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(zombieIdle, 25);

			//Load sound effect
			gunShot = Mix_LoadWAV("./content/163456__lemudcrab__pistol-shot.wav");
			if (gunShot == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load gunShot sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(gunShot, 205);

			//Load sound effect
			playerDamage = Mix_LoadWAV("./content/playerdamage.wav");
			if (playerDamage == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load playerDamage sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(playerDamage, 255);

			//Load sound effect
			zombieAttack = Mix_LoadWAV("./content/zombieattack.wav");
			if (zombieAttack == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load zombieAttack sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(zombieAttack, 55);

			//Load sound effect
			zombieDead = Mix_LoadWAV("./content/103527__tschapajew__pain-scream-hard-2.wav");
			if (zombieDead == NULL)
			{
				//Log info
				LogManager::logMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR, "[Failed to load zombieDeath sound effect];[SDL mixer Error: %s]", Mix_GetError());
				return false;
			}
			else Mix_VolumeChunk(zombieAttack, 55);

		}

		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[All sound effects and music loaded successfully]");
	}

	//Set the volume
	volume = 128;

	//Set the volume
	Mix_Volume(-1, volume);
	Mix_VolumeMusic(volume);

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Initialised SDL audio]");

	//Initialisation success
	return true;
}

//Play game sounds function
void SoundManager::playSound(int soundNum, bool sfx)
{
	//if sound effects are on
	if (sfx)
	{
		//Log info
		LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Playing sound effect]");

		//Switch on sound
		switch (soundNum)
		{
		case 1:
			Mix_PlayChannel(-1, menuSound, 0);
			break;
		case 2:
			Mix_PlayChannel(-1, menuSelect, 0);
			break;
		case 3:
			Mix_PlayChannel(-1, zombieIdle, 0);
			break;
		case 4:
			Mix_PlayChannel(-1, gunShot, 0);
			break;
		case 5:
			Mix_PlayChannel(-1, playerDamage, 0);
			break;
		case 6:
			Mix_PlayChannel(-1, zombieAttack, 0);
			break;
		case 7:
			Mix_PlayChannel(-1, zombieDead, 0);
			break;
		}
	}
}

//Play music function
void SoundManager::playMusic(bool audio)
{
	//If there is no music playing
	if (Mix_PlayingMusic() == 0)
	{
		if (audio)
		{
			//Play the music
			Mix_PlayMusic(music, -1);

			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Playing music]");
		}
	}
	else
	{
		if (!audio)
		{
			//Halt music
			Mix_HaltMusic();

			//Log info
			LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Halting music]");
		}
	}
}

//Set volume of game sounds
void SoundManager::setVolume(int vol)
{
	//Volume control
	int v = volume + vol;

	//Clamp the volume
	if (v <= 0)	v = 0;
	else if (v > 128) v = 128;

	//Control the volume
	Mix_Volume(-1, v);
	Mix_VolumeMusic(v);

	//Store the volume
	volume = v;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Audio volume changed]");
}

//Clean function
void SoundManager::clean()
{
	//Free the sound effects
	Mix_FreeChunk(menuSelect); menuSelect = NULL;
	Mix_FreeChunk(menuSound); menuSound = NULL;
	Mix_FreeChunk(zombieIdle); zombieIdle = NULL;
	Mix_FreeChunk(gunShot);	gunShot = NULL;
	Mix_FreeChunk(playerDamage); playerDamage = NULL;
	Mix_FreeChunk(zombieAttack); zombieAttack = NULL;
	Mix_FreeChunk(zombieDead); zombieDead = NULL;

	//Free the music
	Mix_FreeMusic(music); music = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Sound items destroyed]");

	//Quit SDL subsystems
	Mix_Quit();

	//Delete the sound Manager
	delete sm_instance;
	sm_instance = NULL;

	//Log info
	LogManager::logMessage(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_INFO, "[Sound manager destroyed]");
}

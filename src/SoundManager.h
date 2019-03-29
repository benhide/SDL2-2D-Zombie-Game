#ifndef _SOUND_MANAGER_H
#define _SOUND_MANAGER_H

//SDL and custom headers
#include <SDL_mixer.h>
#include "LogManager.h"

//Sound manager class
class SoundManager
{
public:
	//SoundManager singleton
	static SoundManager* instance();

	//SoundManager initialisation function
	bool init();

	//SoundManager functions
	void playSound(int soundNum, bool sfx);
	void playMusic(bool audio);
	void setVolume(int vol);
	void clean();

private:
	//Static instance of the SoundManager Class
	static SoundManager* sm_instance;

	//Defualt constructor
	SoundManager();

	//Defualt destructor
	~SoundManager();

	//The music that will be played
	Mix_Music* music = NULL;

	//The sound effects that will be used
	Mix_Chunk* menuSound = NULL;
	Mix_Chunk* menuSelect = NULL;
	Mix_Chunk* zombieIdle = NULL;
	Mix_Chunk* gunShot = NULL;
	Mix_Chunk* playerDamage = NULL;
	Mix_Chunk* zombieAttack = NULL;
	Mix_Chunk* zombieDead = NULL;

	//The sound volume
	int volume;
};
#endif 
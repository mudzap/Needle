#ifndef _AUDIO_
#define _AUDIO_

#define CHANNELS 8

#include <unordered_map>
#include <vector>
#include <string>

#include <SDL2/SDL_mixer.h>

#include "util/Log.h"

enum SampleIDs {
	SFX_EXPLOSION = 0,
	SFX_SHOT_SMALL = 1,
	SFX_SHOT_MEDIUM = 2,
	SFX_SHOT_LARGE = 3,
	SFX_SHOT_HUGE = 4,
	SFX_SHOT_LASER = 5,
	SFX_PLAYER_SHOT = 6,
	SFX_PLAYER_POWERUP = 7,
	SFX_PLAYER_GRAZE = 8,
	SFX_PLAYER_HIT = 9,
	SFX_PLAYER_DEATH = 10,
	SFX_ENEMY_DEATH = 11,
	SFX_BOMB = 12,
	SFX_MENU_GO = 13,
	SFX_MENU_BACK = 14,
	SFX_MENU_CHANGE = 15
	//AND MUCH MORE!
};

const std::vector<std::string> sampleDirectories = {
	"assets/audio/explosion.wav",
	"assets/audio/shot_16wav",
	"assets/audio/shot_32.wav",
	"assets/audio/shot_64.wav",
	"assets/audio/shot_128.wav",
	"assets/audio/shot_laser.wav",
	"assets/audio/player_shot.wav",
	"assets/audio/player_powerup.wav",
	"assets/audio/player_graze.wav",
	"assets/audio/player_hit.wav",
	"assets/audio/player_death.wav",
	"assets/audio/enemy_death.wav",
	"assets/audio/bomb.wav",
	"assets/audio/menu_go.wav",
	"assets/audio/menu_back.wav",
	"assets/audio/menu_change.wav"
};


class Audio {

	public:

		Audio();
		~Audio();

		static int LoadSample(const std::string& filepath);
		static std::vector<int> LoadSamples(const std::vector<std::string>& directories);
		static void PlaySample(int index);
		static void PlaySample(int index, int loops);
		static void QueueSample(int newIndex);
		static void HandleAudio();

		static int LoadMusic(const std::string& filepath);
		static void PlayMusic();
		static void PlayMusic(int fadeInMillis);
		static void StopMusic();
		static void StopMusic(int fadeInMillis);
		static void PauseMusic();
		static void ResumeMusic();

		static bool IsPlayingMusic();
		static void SetMusicVolume();

	private:

		static std::vector<int> sfxIndices;
		static std::vector<Mix_Chunk*> samples;
		static Mix_Music* music;

		static unsigned int currentChannel;

};

#endif
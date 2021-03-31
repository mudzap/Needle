#include "Audio.h"

Audio::Audio() {
	printf("Allocating audio sample resources.\n");
	//Mix_AllocateChannels(CHANNELS);
	Audio::sfxIndices.reserve(16);
	Audio::samples.reserve(16);
}

Audio::~Audio() {

	printf("Stopping audio.\n");
	Mix_HaltMusic();
	//Mix_HaltChannel(-1); //CHECK THIS OUT

	printf("Freeing up audio sample resources.\n");
	for (Mix_Chunk* sample : samples)
		Mix_FreeChunk(sample);
	
	Mix_FreeMusic(music);

}

int Audio::LoadSample(const std::string& filepath){

	samples.emplace_back(Mix_LoadWAV(filepath.c_str()));

	if (samples.back() == NULL) {
		printf("Couldn't load WAV file %s.\n", filepath.c_str());
		return -1;
	}

	return(samples.size() - 1);
}

std::vector<int> Audio::LoadSamples(const std::vector<std::string>& directories) {

	std::vector<int> sampleIndices;
	sampleIndices.reserve(directories.size());

	for (std::string filepath : directories)
		sampleIndices.emplace_back(Audio::LoadSample(filepath));
	
	return sampleIndices;

}

void Audio::PlaySample(int index) {

	Mix_HaltChannel(Audio::currentChannel);

	int channel;

	if (samples.size()) {
		channel = Mix_PlayChannel(Audio::currentChannel, samples[index], 0);

		if (channel == -1)
			printf("Failed to play sample at index %i.\n", index);
	} 
	else {
		printf("No samples loaded at index %i!\n", index);
	}

	Audio::currentChannel = (Audio::currentChannel + 1) % CHANNELS;

}

void Audio::PlaySample(int index, int loops) {

	if (loops <= 0)
		loops = 1;

	Mix_HaltChannel(Audio::currentChannel);

	int channel;

	if (samples.size()) {
		channel = Mix_PlayChannel(Audio::currentChannel, samples[index], loops - 1);

		if (channel == -1)
			printf("Failed to play sample at index %i.\n", index);
	}
	else {
		printf("No samples loaded at index %i!\n", index);
	}

	Audio::currentChannel = (Audio::currentChannel + 1) % CHANNELS;

}

int Audio::LoadMusic(const std::string& filepath) {

	Mix_HaltMusic();

	music = (Mix_LoadMUS(filepath.c_str()));

	if (music == NULL) {
		printf("Couldn't load music file %s.\n", filepath.c_str());
		return -1;
	}

	return 0;

}

void Audio::PlayMusic() {

	if (music == NULL) {
		printf("No music loaded! Call Audio::LoadMusic first. \n");
	}
	else {
		if (Mix_PlayMusic(music, -1) == -1) {
			printf("Couldn't play music. Error: %s.\n", Mix_GetError());
		}
	}

}

void Audio::PlayMusic(int fadeInMillis) {

	if (music == NULL) {
		printf("No music loaded! Call Audio::LoadMusic first. \n");
	}
	else {
		if (Mix_FadeInMusic(music, -1, fadeInMillis) == -1) {
			printf("Couldn't play music. Error: %s.\n", Mix_GetError());
		}
	}

}

void Audio::QueueSample(int newIndex) {

	if (sfxIndices.size() > CHANNELS)
		return;

	for (int storedIndex : Audio::sfxIndices)
		if (storedIndex == newIndex)
			return;

	sfxIndices.emplace_back(newIndex);

}

void Audio::HandleAudio() {

	for (int storedIndex : Audio::sfxIndices)
		Audio::PlaySample(storedIndex);

	sfxIndices.clear();

}

void Audio::StopMusic() {
	Mix_HaltMusic();
}

void Audio::StopMusic(int fadeInMillis) {
	Mix_FadeOutMusic(fadeInMillis);
}

void Audio::PauseMusic() {
	Mix_PauseMusic();
}

void Audio::ResumeMusic() {
	Mix_ResumeMusic();
}

bool Audio::IsPlayingMusic()
{
	return Mix_PlayingMusic();
}

std::vector<int> Audio::sfxIndices;
std::vector<Mix_Chunk*> Audio::samples;
Mix_Music* Audio::music = NULL;
unsigned int Audio::currentChannel = 0;
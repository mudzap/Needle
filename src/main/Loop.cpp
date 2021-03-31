#include "Game.h"
#include "Timer.h"

void Game::OnLoop() {
	Timer::lifeTime = SDL_GetTicks();
	//Timer::lifeTimeFrames += 16.66666666666666f;
	Timer::lifeTimeFrames += 16;
	Timer::countedFrames++;
}
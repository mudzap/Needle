#include "Game.h"
#include "util/Timer.h"

void Game::OnLoop() {

	SHMY_LOGV("Keeping track of time\n");
	Timer::lifeTime = SDL_GetTicks();
	//Timer::lifeTimeFrames += 16.66666666666666f;
	Timer::lifeTimeFrames += 16;
	Timer::countedFrames++;

	
	//Audio::HandleAudio();
	SHMY_LOGV("Keeping timed events\n");
	timer.Process();

	SHMY_LOGV("Creating camera thread\n");
	std::thread cameraThread([&] {

		//MOVE TO SPAWNER
		//myLaser.HandleNodes();

		camera.HandleCamera();

		stage.GetDrawables(camera);
	
	});

	SHMY_LOGV("Processing enemy and players\n");
	/*for (Enemy enemies : enemyPool.enemies) {
		
		player.HandlePlayerCollision(enemies);
		enemies.Handle(player);
		
	}*/

	SHMY_LOGV("Handling player movements and bullets\n");
	player.HandlePlayerMovement();
	player.HandleSpawners();

	SHMY_LOGV("Wait for camera thread\n");
	cameraThread.join();
}
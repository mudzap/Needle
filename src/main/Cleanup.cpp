#include "Game.h"

void Game::OnCleanup () {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//Unbind program
	glUseProgram(NULL);

	timer.~Timer();

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_GL_DeleteContext(context);
	context = 0;

	SDL_Quit();

	// quit SDL_mixer
	Mix_CloseAudio();
}
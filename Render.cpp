#include "Game.h"

void Game::OnRender() {
}

void Game::Render(Renderer& renderer)
{
    SDL_GL_SwapWindow(window);
}
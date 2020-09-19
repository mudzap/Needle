#include "Game.h"

void Game::OnEvent(SDL_Event& event, Player& player, sol::state& lua) {

    player.transform.velocity = Complex(0, 0);
    camera.velocity = { 0, 0, 0 };
    camera.rotationDelta = { 0, 0 };

    const uint8_t* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_UP])
        player.transform.velocity.y += player.playerArgs.speed;

    if (keystate[SDL_SCANCODE_DOWN])
        player.transform.velocity.y -= player.playerArgs.speed;

    if (keystate[SDL_SCANCODE_LEFT])
        player.transform.velocity.x -= player.playerArgs.speed;

    if (keystate[SDL_SCANCODE_RIGHT])
        player.transform.velocity.x += player.playerArgs.speed;

    if (keystate[SDL_SCANCODE_KP_1])
        lua.script_file("scripts/stage1.lua");

    if (keystate[SDL_SCANCODE_W])
        camera.velocity.z += 0.4f;

    if (keystate[SDL_SCANCODE_A])
        camera.velocity.x -= 0.4f;

    if (keystate[SDL_SCANCODE_S])
        camera.velocity.z -= 0.4f;

    if (keystate[SDL_SCANCODE_D])
        camera.velocity.x += 0.4f;

    if (keystate[SDL_SCANCODE_I])
        camera.rotationDelta.y += 1.5f;

    if (keystate[SDL_SCANCODE_J])
        camera.rotationDelta.x -= 1.5f;

    if (keystate[SDL_SCANCODE_K])
        camera.rotationDelta.y -= 1.5f;

    if (keystate[SDL_SCANCODE_L])
        camera.rotationDelta.x += 1.5f;

    if (keystate[SDL_SCANCODE_SPACE])
        camera.velocity.y += 0.4f;

    if (keystate[SDL_SCANCODE_LSHIFT])
        camera.velocity.y -= 0.4f;

    while (SDL_PollEvent(&event)) {

        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            isRunning = false;
        if (event.type == SDL_QUIT)
            isRunning = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(window)) {

            const float ar = DEFAULT_W / DEFAULT_H;
            const float newAr = (float)event.window.data1 / (float)event.window.data2;

            if (newAr < ar) {
                newScale = event.window.data1 / DEFAULT_W;
                newXOffset = 0;
                newYOffset = (float)event.window.data1 * (ar - newAr) / 2;
                newXPlayOffset = newScale * DEFAULT_X_OFFSET;
                newYPlayOffset = newScale * DEFAULT_Y_OFFSET + newYOffset;

            }
            else {
                newScale = event.window.data2 / DEFAULT_H;
                newXOffset = (float)event.window.data2 * (newAr - ar) / 2;
                newYOffset = 0;
                newXPlayOffset = newScale * DEFAULT_X_OFFSET + newXOffset;
                newYPlayOffset = newScale * DEFAULT_Y_OFFSET;
            }

            newWidth = newScale * DEFAULT_W;
            newHeight = newScale * DEFAULT_H;

            newPlayWidth = newScale * DEFAULT_PLAY_W;
            newPlayHeight = newScale * DEFAULT_PLAY_H;

            orthoPlay = glm::ortho(-newPlayWidth / 2, newPlayWidth / 2, -newPlayHeight / 2, newPlayHeight / 2, 0.0f, 1.0f);
            orthoFull = glm::ortho(-newWidth / 2, newWidth / 2, -newHeight / 2, newHeight / 2, 0.f, 1.0f);

            shader.Bind();
            shader.SetUniformMat4f("u_Projection", orthoPlay);
            shader.SetUniform1f("scale", newScale);

            textShader.Bind();
            textShader.SetUniformMat4f("u_Projection", orthoFull);
            textShader.SetUniform4f("v_Color", 0.5f, 1.f, 0.5f, 1.f);
            shader.SetUniform1f("scale", newScale);

            shader3D.Bind();
            shader3D.SetUniformMat4f("u_VP", camera.projection * camera.view);
            shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);
        }
    }

}
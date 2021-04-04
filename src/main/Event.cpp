#include "Game.h"

void Game::OnEvent() {

    player.shooting = false;
    player.transform.velocity = Complex(0, 0);
    player.currentSpeed = player.playerArgs.speed;
    camera.velocity = { 0, 0, 0 };
    camera.rotationDelta = { 0, 0 };

    const uint8_t* keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_LSHIFT]) {
        camera.velocity.y -= 0.4f;
        player.currentSpeed = player.playerArgs.halfspeed;
    }

    if (keystate[SDL_SCANCODE_UP])
        player.transform.velocity.y += player.currentSpeed;

    if (keystate[SDL_SCANCODE_DOWN])
        player.transform.velocity.y -= player.currentSpeed;

    if (keystate[SDL_SCANCODE_LEFT])
        player.transform.velocity.x -= player.currentSpeed;

    if (keystate[SDL_SCANCODE_RIGHT])
        player.transform.velocity.x += player.currentSpeed;

    if (keystate[SDL_SCANCODE_KP_1]) {
        SHMY_LOGD("Starting lua script\n");
        sol::error e = lua.script_file("scripts/stage1.lua");
        printf("%s\n", e.what());
    }

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

    if (keystate[SDL_SCANCODE_Z])
        player.shooting = true;

    if (keystate[SDL_SCANCODE_SPACE])
        camera.velocity.y += 0.4f;
        


    while (SDL_PollEvent(&event)) {

        ImGui_ImplSDL2_ProcessEvent(&event);

        bool backSignal;

        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            if(isInMenu) {
                backSignal = true;
            }

            if (isInGame && !isPaused) {
                isPaused = !isPaused;
                isInMenu = !isInMenu;
            }
            
            if (backSignal && isPaused && isInGame) {
                isPaused = !isPaused;
                isInMenu = !isInMenu;
            }
        }

        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            isRunning = false;
        if (event.type == SDL_QUIT)
            isRunning = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && event.window.windowID == SDL_GetWindowID(window)) {

            const float ar = DEFAULT_W / DEFAULT_H;
            const float newAr = (float)event.window.data1 / (float)event.window.data2;
            float oldScaleComp = 1/newScale;
            ImVec2 oldOffset = ImVec2(newXOffset, newYOffset);
            

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

            float oldScale = oldScaleComp * newScale;
            ImVec2 newOffset = ImVec2(newXOffset, newYOffset);

            for (const auto &viewport : ImGui::GetCurrentContext()->Viewports)
            {
                ImGui::TranslateWindowsInViewport(viewport, oldOffset, newOffset);
                ImGui::ScaleWindowsInViewport(viewport, oldScale);
            }
            ImGui::GetStyle().ScaleAllSizes(newScale);
            ImGui::GetIO().FontGlobalScale = newScale;


            //orthoPlay = glm::ortho(-newPlayWidth / 2, newPlayWidth / 2, -newPlayHeight / 2, newPlayHeight / 2, 0.0f, 1.0f);
            //orthoFull = glm::ortho(-newWidth / 2, newWidth / 2, -newHeight / 2, newHeight / 2, 0.f, 1.0f);

            orthoPlay = glm::ortho(-DEFAULT_PLAY_W / 2, DEFAULT_PLAY_W / 2, -DEFAULT_PLAY_H / 2, DEFAULT_PLAY_H / 2, 0.0f, 1.0f);
            orthoFull = glm::ortho(-DEFAULT_W / 2, DEFAULT_W / 2, -DEFAULT_H / 2, DEFAULT_H / 2, 0.f, 1.0f);
            camera.projection = glm::perspective(glm::radians(60.f), DEFAULT_PLAY_W / DEFAULT_PLAY_H, 1.f, 750.f);

#ifdef _MAT4_INSTANCING_
            shader.Bind();
            shader.SetUniform1i("u_Texture", 0);
            shader.SetUniformMat4f("u_Projection", orthoPlay);
            shader.SetUniform1f("scale", 1);
#else
            shader.Bind();
            shader.SetUniform1i("u_Texture", 0);
            shader.SetUniformMat4f("u_Projection", orthoPlay);
#endif

            fboShader.Bind();
            fboShader.SetUniform1i("scene", 4);
            fboShader.SetUniform1i("bloomBlur", 5);
            fboShader.SetUniform1f("exposure", 0.7f);

            uiShader.Bind();
            uiShader.SetUniform1i("u_Texture", 7);
            uiShader.SetUniformMat4f("u_Projection", orthoPlay);

            fboIntermediateShader.Bind();
            fboIntermediateShader.SetUniform1i("u_Texture", 4);

            textShader.Bind();
            textShader.SetUniform1i("u_Texture", 1);
            textShader.SetUniformMat4f("u_Projection", orthoFull);

            shader3D.Bind();
            shader3D.SetUniform1i("u_Texture", 2);
            shader3D.SetUniform1i("u_Normal", 6);
            shader3D.SetUniform1i("skybox", 0);
            shader3D.SetUniformMat4f("u_VP", camera.projection * camera.view);
            shader3D.SetUniform3f("lightPos", 0.f, 200.f, 0.f);
            shader3D.SetUniform3f("viewPos", camera.position.x, camera.position.y, camera.position.z);

            shaderCube.Bind();
            shaderCube.SetUniform1f("skybox", 0);
            shaderCube.SetUniformMat4f("u_VP", camera.projection * camera.view);

            frameBuffer.ReInit(newPlayWidth, newPlayHeight, FB_COLOR, 2, false);
            gaussianFramebuffer[0].ReInit(newPlayWidth / DOWNSCALE_FACTOR, newPlayHeight / DOWNSCALE_FACTOR, FB_COLOR, 1);
            gaussianFramebuffer[1].ReInit(newPlayWidth / DOWNSCALE_FACTOR, newPlayHeight / DOWNSCALE_FACTOR, FB_COLOR, 1);
            intermediateFramebuffer.ReInit(newPlayWidth, newPlayHeight);

        }
    }


}
#include "Game.h"

void Game::OnRender() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);



    //START FRAMEBUFFER

    intermediateFramebuffer.Bind();

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, newPlayWidth, newPlayHeight);
    glScissor(0, 0, newPlayWidth, newPlayHeight);



    //DRAW STAGE

    shader3D.Bind();

    shader3D.SetUniformMat4f("u_VP", camera.vpMat);
    shader3D.SetUniform3f("lightDir", 0.f, 0.34202f, 0.93969f);
    //shader3D.SetUniform3f("lightPos", 60.f * FT::cos(SDL_GetTicks() / 1000.f), 150.f, 60.f * FT::sin(SDL_GetTicks() / 1000.f));
    shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);
    shader3D.SetUniform3f("viewPos", camera.position.x, camera.position.y, camera.position.z);


    stage.DrawTilesOcclude(shader3D);

    //DRAW SKYBOX

    shaderCube.Bind();
    skybox.Draw(shaderCube, camera);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    //DRAW ENTITIES

    shader.Bind();

    player.DrawBullets();
    player.Draw();

    for (Enemy enemy : enemyPool.enemies) {
        enemy.DrawBullets();
    }
    for (Enemy enemy : enemyPool.enemies) {
        enemy.Draw();
    }

    //MOVE TO SPAWNER
    //myLaser.ResetDraw();

    // FINISH FRAMEBUFFER
    fboIntermediateShader.Bind();
    frameBuffer.Bind();

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    intermediateFramebuffer.BindTexture(4);
    intermediateFramebuffer.Draw();

    glViewport(0, 0, newPlayWidth/DOWNSCALE_FACTOR, newPlayHeight/DOWNSCALE_FACTOR);
    glScissor(0, 0, newPlayWidth/DOWNSCALE_FACTOR, newPlayHeight/DOWNSCALE_FACTOR);


    bool horizontal = true, first_iteration = true;
    int amount = 3; //KIND OF PERFORMANCE HEAVY
    gaussianShader.Bind();

    for (unsigned int i = 0; i < amount; i++)
    {
        gaussianFramebuffer[horizontal].Bind();
        gaussianFramebuffer[horizontal].BindTexture(5);
        gaussianShader.SetUniform1i("horizontal", horizontal);

        if (first_iteration) {
            frameBuffer.BindTexture(5, 2);
            frameBuffer.Draw();
        }
        else {
            gaussianFramebuffer[!horizontal].BindTexture(5);
            gaussianFramebuffer[!horizontal].Draw();
        }

        horizontal = !horizontal;

        if (first_iteration)
            first_iteration = false;
    }
    frameBuffer.Unbind(); //UNBINDS ALL FRAMEBUFFERS, SHOULD BE STATIC

    glViewport(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);
    glScissor(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);

    fboShader.Bind();
    frameBuffer.Draw();

    frameBuffer.BindTexture(4, 1);
    gaussianFramebuffer[!horizontal].BindTexture(5);




    //TEXT
    glViewport(newXOffset, newYOffset, newWidth, newHeight);
    glScissor(newXOffset, newYOffset, newWidth, newHeight);


    uiShader.Bind();
    gameUI.Draw();

    textShader.Bind();
    //MOVE TO UI
    myFont.StaticDraw();


}

void Game::Render(Renderer& renderer){

    SDL_GL_SwapWindow(window);
}

void Game::RenderImGuiDebug() {

    {
//        ImGui::PushFont(font);
        ImGui::Begin("Game");
        ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//        ImGui::PopFont();
        ImGui::End();
    }
    
    {
//        ImGui::PushFont(font);
        ImGui::Begin("Camera");
        ImGui::InputFloat3("Position", &camera.position.x, "%.3f");
        ImGui::InputFloat3("Target", &camera.target.x, "%.3f");
        ImGui::InputFloat2("Angles", &camera.angles.x, "%.4f");
        if (ImGui::Button("Override"))
        {
            camera.CameraStop();
            camera.CameraFloat();
        } ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            camera.position = glm::vec3{ 0, 0, 0 };
            camera.angles = glm::vec2{ 0, 0 };
            camera.CameraStop();
            camera.CameraFloat();
        }

        ImGui::Separator();
        ImGui::Text("Movement:");
        ImGui::InputInt("Frames", &frames);
        ImGui::InputFloat3("Delta_1", delta1, "%.3f");
        ImGui::InputFloat3("Delta_2", delta2, "%.3f");
        ImGui::InputFloat3("Delta_3", delta3, "%.3f");

        if (ImGui::Button("Lerp"))
        {
            camera.LinearInterpTo(frames, delta1[0], delta1[1], delta1[2]);
        } ImGui::SameLine();
        if (ImGui::Button("Cerp"))
        {
            camera.CubicInterpBoth(frames, delta1[0], delta1[1], delta1[2]);
        } ImGui::SameLine();
        if (ImGui::Button("BezierQuad"))
        {
            camera.QuadraticBezier(frames, delta1[0], delta1[1], delta1[2], delta2[0], delta2[1], delta2[2]);
        } ImGui::SameLine();
        if (ImGui::Button("BezierCubic"))
        {
            camera.CubicBezier(frames, delta1[0], delta1[1], delta1[2], delta2[0], delta2[1], delta2[2], delta3[0], delta3[1], delta3[2]);
        }

        ImGui::Separator();
        ImGui::Text("Rotation:");
        ImGui::InputInt("Frames", &aframes);
        ImGui::InputFloat2("ADelta_1", adelta1, "%.3f");
        ImGui::InputFloat2("ADelta_2", adelta2, "%.3f");
        ImGui::InputFloat2("ADelta_3", adelta3, "%.3f");

        if (ImGui::Button("ALerp"))
        {
            camera.LinearInterpToAngle(aframes, adelta1[0], adelta1[1]);
        } ImGui::SameLine();
        if (ImGui::Button("ACerp"))
        {
            camera.CubicInterpBothAngle(aframes, adelta1[0], adelta1[1]);
        } ImGui::SameLine();
        if (ImGui::Button("ABezierQuad"))
        {
            camera.QuadraticBezierAngle(aframes, adelta1[0], adelta1[1], adelta2[0], adelta2[1]);
        } ImGui::SameLine();
        if (ImGui::Button("ABezierCubic"))
        {
            camera.CubicBezierAngle(aframes, adelta1[0], adelta1[1], adelta2[0], adelta2[1], adelta3[0], adelta3[1]);
        }

//        ImGui::PopFont();
        ImGui::End();
    }

    {
//        ImGui::PushFont(font);
        ImGui::Begin("Lua");            
        if (ImGui::Button("Start Main Script"))
        {
            lua.script_file("scripts/stage1.lua");
        }
        ImGui::Separator();
        ImGui::TextWrapped("Lua console\n");
        if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            char* input_end = InputBuf + strlen(InputBuf);
            while (input_end > InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
            if (InputBuf[0])
                lua.script(InputBuf);
            strcpy(InputBuf, "");
        }
//        ImGui::PopFont();
        ImGui::End();
    }  

}


void Game::RenderImGuiPause() {

    //ImGui::GetMainViewport()->Pos
    //SetNextWindowRelativePos
    ImGuiWindowFlags window_flags = 0;
    //window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoScrollbar;
    //window_flags |= ImGuiWindowFlags_MenuBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    //window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoBackground;

    //const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    //ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos + ImVec2(80, 500), ImGuiCond_FirstUseEver);

    {
        ImGui::SetNextWindowPos(ImVec2(-300, -200), ImGuiCond_FirstUseEver);
        //ImGui::SetNextWindowPos(ImVec2(20 + newXOffset, 500 + newYOffset), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
        //ImGui::PushFont(font);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImGui::Begin("Pause menu", (bool*)__null, window_flags);

        if (ImGui::Button("Resume")) {}

        if (ImGui::Button("Restart")) {}

        if (ImGui::Button("Exit to main menu")) {}


        ImGui::End();
        ImGui::PopStyleVar();
        //ImGui::PopFont();
    }

}



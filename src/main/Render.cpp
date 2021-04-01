#include "Game.h"

void Game::OnRender() {
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
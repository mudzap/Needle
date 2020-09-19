#include "Game.h"
#include "ProjData.h"

Game::Game() {

    window = NULL;
    
    isRunning = true;

    context = 0;

}

int Game::OnExecute() {

    OnInit();

    RNG::setSeed(5);

    glViewport(DEFAULT_X_OFFSET, DEFAULT_Y_OFFSET, DEFAULT_PLAY_W, DEFAULT_PLAY_H);

    orthoPlay = glm::ortho(-DEFAULT_PLAY_W / 2, DEFAULT_PLAY_W / 2, -DEFAULT_PLAY_H / 2, DEFAULT_PLAY_H / 2, 0.0f, 1.0f);
    orthoFull = glm::ortho(-DEFAULT_W / 2, DEFAULT_W / 2, -DEFAULT_H / 2, DEFAULT_H / 2, 0.f, 1.0f);
    camera.projection = glm::perspective(glm::radians(60.f), DEFAULT_PLAY_W / DEFAULT_PLAY_H, 1.f, 750.f);

    shader.InitShader("shaders/vertexTransform.shader", "shaders/fragTransform.shader");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniformMat4f("u_Projection", orthoPlay);
    shader.SetUniform1f("scale", 1);
    
    textShader.InitShader("shaders/vertexPrimitive.shader", "shaders/fragSingleChannel.shader");
    textShader.Bind();
    textShader.SetUniform1i("u_Texture", 1);
    textShader.SetUniformMat4f("u_Projection", orthoFull);
    textShader.SetUniform4f("v_Color", 0.5f, 1.f, 0.5f, 1.f);
    textShader.SetUniform1f("scale", 1);

    shader3D.InitShader("shaders/vertex3d.shader", "shaders/frag3d.shader");
    shader3D.Bind();
    shader3D.SetUniform1i("u_Texture", 2);
    shader3D.SetUniform1i("skybox", 3);
    shader3D.SetUniformMat4f("u_VP", camera.projection * camera.view);
    shader3D.SetUniformMat4f("u_Model", glm::mat4(1.f));
    shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);


    // STAGE

    Stage stage;
    stage.LoadModel("models/stairs.obj");
    stage.FillBuffer();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                stage.PushBackStageTile(NORTH, { i, 2 * j, k }, 0);
                stage.PushBackStageTile(EAST, { i, 2 * j, k }, 0);
                stage.PushBackStageTile(SOUTH, { i, 2 * j, k }, 0);
                stage.PushBackStageTile(WEST, { i, 2 * j, k }, 0);
            }
        }
    }

    Model cube("models/cube.obj");


    Texture cubeMap;
    cubeMap.LoadTextureCubeMap({
        "sprites/px.png",
        "sprites/nx.png" ,
        "sprites/py.png" ,
        "sprites/ny.png" ,
        "sprites/pz.png" ,
        "sprites/nz.png" },
        true
    );

    Texture texture;
    texture.LoadTexture("sprites/entities.png", true);

    Texture texture2;
    texture2.LoadTexture("sprites/texture.png", true);

    Font myFont;
    myFont.Load("fonts/B612Mono-Bold.ttf");
    myFont.Prepare(512, 512,
        42, 1);
    myFont.Load("fonts/Roboto-Bold.ttf");
    myFont.Add(42, 1);
    myFont.Load("fonts/EBGaramond-Regular.ttf");
    myFont.Add(42, 1);
    myFont.Load("fonts/LibreCaslonDisplay-Regular.ttf");
    myFont.Add(42, 1);
    myFont.EndLoad();
    myFont.LoadTexture(true);

    myFont.Bind(1);
    texture2.Bind(2);
    cubeMap.BindCubeMap(3);
    texture.Bind(0);

    //myFont.Save("sprites/fontAtlas.png");


    // AUDIO

    //Audio::LoadMusic("audio/Warp Tapes 89-93 Part 1.wav");
    //Audio::PlayMusic();
    //Audio::LoadSample("audio/bell.wav");



    // PLAYER

    PlayerArgs playerArgs;
    TransformArgs playerTransform = {
        .position = Complex(0.f, -200.f),
        .velocity = Complex::zero,
    }; 
    Player player(playerArgs, playerTransform, playerAnimation);

    // LASER, MOVE TO SPAWNER

    LaserArgs laser1;
    TransformArgs laser1Transform{
        .position = {-300.f,-200.f},
        .velocity = {8.f,0.f},
        .rotation = Complex::FromAngle(90.f),
        .rotationDelta = Complex::FromAngle(-2.f)
    };

    Laser myLaser(laser1, laser1Transform);


    // LUA

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine);

    Script::CreateUsertypes(lua);
    LuaData::LoadProjData(lua);
    LuaData::LoadAnimData(lua);

    lua["cam"] = &camera;
    lua["enemies"] = &enemyPool; //MOVE THIS TO SCRIPT CLASS
    lua["doAfterFrames"] = &Timer::AddLuaTimer;
       
    lua.script_file("scripts/stage1.lua");
    sol::function stage1Main = lua["main"];


    SDL_Event event;


    //UI

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 20);
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("fonts/EBGaramond-BoldItalic.ttf", 15);


    //GUI

    int frames = 60;
    float delta1[3] = { 0, 0, 0 };
    float delta2[3] = { 0, 0, 0 };
    float delta3[3] = { 0, 0, 0 };

    int aframes = 60;
    float adelta1[2] = { 0, 0 };
    float adelta2[2] = { 0, 0 };
    float adelta3[2] = { 0, 0 };
    
    char InputBuf[256] = { 0 };

    //GAME LOOP //

    while (isRunning) {

        //ON EVENT

        OnEvent(event, player, lua);


        //ON LOOP

        Audio::HandleAudio();
        timer.Process();

        for (Enemy* enemies : enemyPool.activeEnemies) {
            player.HandlePlayerCollision(enemies);
            enemies->HandleEnemy();
        }

        player.HandlePlayerMovement();

        //MOVE TO SPAWNER
        myLaser.HandleNodes();
        

        //MOVE TO UI, NORMALIZE VALUES 
        //SET TEXT (STATIC OR NOT)
        //GET QUADS (ONLY STATIC)!
        myFont.GetQuad("SCORE:", 0, Complex(224.f, -422.f));
        myFont.GetQuad("SCORE:", 0, Complex(224.f, 422.f));
        myFont.GetQuad(std::to_string(player.playerArgs.pointScore), 0, Complex(224.f, 386.f));
        myFont.GetQuad("GRAZE:", 0, Complex(224.f, 346.f));
        myFont.GetQuad(std::to_string(player.playerArgs.grazeScore), 0, Complex(224.f, 310.f));

        camera.HandleCamera();

        OnLoop();
        


        //ON RENDER

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);
        glScissor(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        shader3D.Bind();

        shader3D.SetUniformMat4f("u_VP", camera.vpMat);
        shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);

        stage.DrawTilesOcclude(camera, shader3D);

        cube.Draw();

        glDisable(GL_DEPTH_TEST);

        shader.Bind();

        player.Draw();

        for (Enemy* enemies : enemyPool.activeEnemies) {
            enemies->DrawBullets();
        }
        for (Enemy* enemies : enemyPool.activeEnemies) {
            enemies->Draw();
        }

        //MOVE TO SPAWNER
        //myLaser.ResetDraw();

        glViewport(newXOffset, newYOffset, newWidth, newHeight);
        glScissor(newXOffset, newYOffset, newWidth, newHeight);

        textShader.Bind();
        //MOVE TO UI
        myFont.ResetDraw();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        
        ImGui::NewFrame();
        {
            ImGui::PushFont(font1);
            ImGui::Begin("Player");
            ImGui::Text("Lives:\n%i\n", player.playerArgs.hearts);
            ImGui::Text("Graze:\n%i\n", player.playerArgs.grazeScore);
            ImGui::PopFont();
            ImGui::End();
        }

        {
            ImGui::PushFont(font1);
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

            ImGui::PopFont();
            ImGui::End();
        }

        {
            ImGui::PushFont(font1);
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
            ImGui::PopFont();
            ImGui::End();
        }

        {
            ImGui::PushFont(font1);
            ImGui::Begin("Shader");
            if (ImGui::Button("Recompile"))
            {
                shader.ReInitShader();
                shader3D.ReInitShader();
                textShader.ReInitShader();
                shader.Bind();
                shader.SetUniform1i("u_Texture", 0);
                shader.SetUniformMat4f("u_Projection", orthoPlay);
                shader.SetUniform1f("scale", 1);
                textShader.Bind();
                textShader.SetUniform1i("u_Texture", 1);
                textShader.SetUniformMat4f("u_Projection", orthoFull);
                textShader.SetUniform4f("v_Color", 0.5f, 1.f, 0.5f, 1.f);
                textShader.SetUniform1f("scale", 1);
                shader3D.Bind();
                shader3D.SetUniform1i("u_Texture", 2);
                shader3D.SetUniform1i("skybox", 3);
                shader3D.SetUniformMat4f("u_VP", camera.projection * camera.view);
                shader3D.SetUniformMat4f("u_Model", glm::mat4(1.f));
                shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);
            }
            ImGui::PopFont();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

     
        SDL_GL_SwapWindow(window);


    }


    OnCleanup();

    return 0;

}

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    Game game;


    return game.OnExecute();

}
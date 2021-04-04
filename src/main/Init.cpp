#include "Game.h"
#include "video/Shader.h"

bool Game::OnInit() {
	
    bool success = true;
    
    // GL 3.0 + GLSL 130
    Shader::glslVersion = "#version 330 core";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
    //SDL_OPENGL | SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER
	//SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
    printf((char*)glGetString(GL_VERSION));

    window = SDL_CreateWindow("Shinmyoumaru", 32, 32, DEFAULT_W, DEFAULT_H, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        SHMY_LOGE("Window could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    //Create context
    context = SDL_GL_CreateContext(window);
    if (context == NULL)
    {
        SHMY_LOGE("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {

        //Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            SHMY_LOGE("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
        }

        //Use Vsync
        if (SDL_GL_SetSwapInterval(-1) < 0)
        {
            SHMY_LOGD("Warning: Unable to set Adaptive VSync! SDL Error: %s\n", SDL_GetError());

            if (SDL_GL_SetSwapInterval(1) < 0)
                SHMY_LOGD("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
                
        }

        //Initialize OpenGL
        if (!initGL())
        {
            SHMY_LOGE("Unable to initialize OpenGL!\n");
            success = false;
        }

    }
    

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif


    if (err)
    {
        SHMY_LOGE("Failed to initialize OpenGL loader!\n");
        return 1;
    }


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(Shader::glslVersion.c_str());


    //INIT SDL_MIXER
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        SHMY_LOGE("Couldn't initialize SDL_mixer.");
        success = false;
    }

	return success;
}

int Game::initGL()
{
    //Success flag
    bool success = true;

    glClearColor(0.1f, 0.1f, 0.1f, 1.f);

    glViewport(DEFAULT_X_OFFSET, DEFAULT_Y_OFFSET, DEFAULT_PLAY_W, DEFAULT_PLAY_H);
    glScissor(DEFAULT_X_OFFSET, DEFAULT_Y_OFFSET, DEFAULT_PLAY_W, DEFAULT_PLAY_H);
    //glAlphaFunc(GL_LEQUAL, 0.5); 
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_BLEND);

    glDisable(GL_MULTISAMPLE);
    glDisable(GL_SCISSOR_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return success;
}

int Game::initShaders()
{
    //Success flag
    bool success = true;

    orthoPlay = glm::ortho(-DEFAULT_PLAY_W / 2, DEFAULT_PLAY_W / 2, -DEFAULT_PLAY_H / 2, DEFAULT_PLAY_H / 2, 0.0f, 1.0f);
    orthoFull = glm::ortho(-DEFAULT_W / 2, DEFAULT_W / 2, -DEFAULT_H / 2, DEFAULT_H / 2, 0.f, 1.0f);
    camera.projection = glm::perspective(glm::radians(60.f), DEFAULT_PLAY_W / DEFAULT_PLAY_H, 1.f, 750.f);
 
    SHMY_LOGV("Initializing transform shader\n");
#ifdef _MAT4_INSTANCING_
    shader.InitShader("shaders/vertexTransform_old.shader", "shaders/fragTransform.shader");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniformMat4f("u_Projection", orthoPlay);
    shader.SetUniform1f("scale", 1);
#else
    shader.InitShader("shaders/vertexTransform.shader", "shaders/fragTransform.shader");
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniformMat4f("u_Projection", orthoPlay);
#endif

    SHMY_LOGV("Initializing bloom shader\n");
    fboShader.InitShader("shaders/vertexPrimitive.shader", "shaders/fragBloom.shader");
    fboShader.Bind();
    fboShader.SetUniform1i("scene", 4);
    fboShader.SetUniform1i("bloomBlur", 5);
    fboShader.SetUniform1f("exposure", 0.7f);

    SHMY_LOGV("Initializing intermediate bloom shader\n");
    fboIntermediateShader.InitShader("shaders/vertexPrimitive.shader", "shaders/fragPrimitiveBloom.shader");
    fboIntermediateShader.Bind();
    fboIntermediateShader.SetUniform1i("u_Texture", 4);

    SHMY_LOGV("Initializing ui shader\n");
    uiShader.InitShader("shaders/vertexPrimitive.shader", "shaders/fragPrimitive.shader");
    uiShader.Bind();
    uiShader.SetUniform1i("u_Texture", 7);
    //uiShader.SetUniformMat4f("u_Projection", orthoPlay);

    SHMY_LOGV("Initializing text shader\n");
    textShader.InitShader("shaders/vertexSingleChannel.shader", "shaders/fragSingleChannel.shader");
    textShader.Bind();
    textShader.SetUniform1i("u_Texture", 1);
    textShader.SetUniformMat4f("u_Projection", orthoFull);

    SHMY_LOGV("Initializing 3d shader\n");
    shader3D.InitShader("shaders/vertex3d.shader", "shaders/frag3d.shader");
    shader3D.Bind();
    shader3D.SetUniform1i("u_Texture", 2);
    shader3D.SetUniform1i("u_Normal", 6);
    shader3D.SetUniform1i("skybox", 0);
    shader3D.SetUniformMat4f("u_VP", camera.projection * camera.view);
    shader3D.SetUniform3f("lightPos", 0.f, 200.f, 0.f);
    shader3D.SetUniform3f("viewPos", camera.position.x, camera.position.y, camera.position.z);

    SHMY_LOGV("Initializing skybox shader\n");
    shaderCube.InitShader("shaders/skyboxVertexShader.shader", "shaders/skyboxFragShader.shader");
    shaderCube.Bind();
    shaderCube.SetUniform1f("skybox", 0);
    shaderCube.SetUniformMat4f("u_VP", camera.projection * camera.view);

    SHMY_LOGV("Initializing gaussian blur shader\n");
    gaussianShader.InitShader("shaders/vertexPrimitive.shader", "shaders/fragGaussian.shader");
    gaussianShader.Bind();
    gaussianShader.SetUniform1i("u_Texture", 5);

    return success;

}

int Game::initLua() {

    bool success = true;

    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine);

    Script::CreateUsertypes(lua);
    LuaData::LoadProjData(lua);
    LuaData::LoadAnimData(lua);

    lua.script_file("scripts/stage1.lua");

    lua["cam"] = camera;
    lua["enemies"] = &enemyPool;
    lua["spawners"] = &spawnerPool;
    lua["doAfterFrames"] = &Timer::AddLuaTimer;
       
    stage1Main = lua["main"];

    return success;

}

int Game::initMaterials() {

    bool success = true;
    
    texture.LoadTexture("assets/sprites/entities.png");
    normalTest.LoadTextureMipmap("assets/sprites/stone_normal.png");
    uiTexture.LoadTexture("assets/sprites/ui.png");

    std::vector<std::string> testPaths = { "assets/sprites/entities.png","assets/sprites/entities.png","assets/sprites/entities.png" };
    materials.AddTextureSet(testPaths);

    materials.LoadTextureSets();

    materials.BindTextures();
    //materials.UnbindTextures();
    materials.FlushTextures();

    cubeMap.LoadTextureCubeMap({
        "assets/sprites/px.png",
        "assets/sprites/nx.png",
        "assets/sprites/py.png",
        "assets/sprites/ny.png",
        "assets/sprites/pz.png",
        "assets/sprites/nz.png"
    });

    return success;

}

int Game::initStage() {

    bool success = true;

    stage.LoadModel("assets/models/stairs.obj", "assets/sprites/stone.png", 2);
    stage.LoadModel("assets/models/sphere.obj", "assets/sprites/stone.png", 3);
    
    stage.PushBackStageTile(NORTH, { 0, 0, 0 }, 0);
    stage.PushBackStageTile(NORTH, { 0, 0, 0 }, 1);
    stage.FillBuffer();

    return success;

}


void Game::printProgramLog(int program)
{
    //Make sure name is shader
    if (glIsProgram(program))
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            //Print Log
            SHMY_LOGD("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        SHMY_LOGD("Name %d is not a program\n", program);
    }
}
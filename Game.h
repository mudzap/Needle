#ifndef _GAME_H_
#define _GAME_H_

#define GLM_FORCE_INLINE

//#define GLM_FORCE_ALIGNED
//#define GLM_FORCE_SSE2

#define GLM_FORCE_NO_CTOR_INIT
#define GLM_FORCE_SIZE_T_LENGTH


extern "C"
{
#include "lua/include/lua.h"
#include "lua/include/lauxlib.h"
#include "lua/include/lualib.h"
}

#define GLEW_STATIC

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl.h"
#include "imgui\imgui_impl_opengl3.h"
#include <SDL.h>

#include "OBJ_Loader.h"

#include "fasttrigo.h"
#include <SDL_mixer.h>
#include "stb_image.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "SDL_thread.h"
#include "stb_image_write.h"
#include "stb_truetype.h"
#include "sol/sol.hpp"
#include <stdio.h>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <array>
#include <algorithm>
#include <thread>
#include <map>
#include <unordered_map>
#include <deque>
#include <queue>
#include <functional>
#include <set>
#include <cstdarg>

#include "Texture.h"
#include "RNG.h"
#include "Projectile.h"
#include "Spawner.h"
#include "Player.h"
#include "Renderer.h"
#include "Shader.h"
#include "Complex.h"
#include "Hitbox.h"
#include "Transform.h"
#include "Mesh.h"
#include "Font.h"
#include "Laser.h"
#include "Enemy.h"
#include "Timer.h"
#include "Pool.h"
#include "Script.h"
#include "Model.h"
#include "Camera.h"
#include "UI.h"
#include "ProjData.h"
#include "AnimData.h"
#include "LuaData.h"
#include "Animation.h"
#include "Stage.h"
#include "Audio.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const float DEFAULT_PLAY_W = 768;
const float DEFAULT_PLAY_H = 896;
const float DEFAULT_X_OFFSET = 64;
const float DEFAULT_Y_OFFSET = 32;
const float DEFAULT_W = 1280;
const float DEFAULT_H = 960;

class Game {

    public:

        Game();
        int OnExecute();

        bool OnInit();
        int initGL();
        void OnEvent(SDL_Event& Event, Player& player, sol::state &lua);
        void OnLoop();
        void OnRender();
        void Render(Renderer& renderer);
        void OnCleanup();

        void printProgramLog(int program);

        bool isRunning;

        Camera camera;
        Timer timer;
        Pool enemyPool;

        Shader shader;
        Shader textShader;
        Shader shader3D;

        Audio audio;

        glm::mat4 orthoPlay;
        glm::mat4 orthoFull;

        glm::mat4 modelMat;
        glm::mat4 projectionMat;

        SDL_Window* window;
        SDL_GLContext context;

        float newWidth = DEFAULT_W;
        float newHeight = DEFAULT_H;
        float newScale = 1.f;
        float newPlayWidth = DEFAULT_PLAY_W;
        float newPlayHeight = DEFAULT_PLAY_H;
        float newXOffset = 0;
        float newYOffset = 0;
        float newXPlayOffset = DEFAULT_X_OFFSET;
        float newYPlayOffset = DEFAULT_Y_OFFSET;



    private:

        //std::string vertexShader, fragmentShader;

};

#endif
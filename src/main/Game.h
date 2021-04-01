#ifndef _GAME_H_
#define _GAME_H_

//#define GLM_FORCE_INLINE
//#define GLM_FORCE_ALIGNED
//#define GLM_FORCE_SSE2

//#define GLM_FORCE_NO_CTOR_INIT
//#define GLM_FORCE_SIZE_T_LENGTH

//#define GLM_ENABLE_EXPERIMENTAL


extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define GL_GLEXT_PROTOTYPES

//#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
//#include <gl/glu.h>
#include <GL/glu.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vector.h>
#include <freetype-gl/markup.h>
#include <freetype-gl/font-manager.h>
#include <freetype-gl/vertex-buffer.h>
#include <freetype-gl/edtaa3func.h>
#include <freetype-gl/text-buffer.h>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb/stb_image_write.h"
#include "stb/stb_truetype.h"
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

#include "util/StringFormat.h"
#include "video/Texture.h"
#include "math/RNG.h"
#include "entities/Projectile.h"
#include "entities/Spawner.h"
#include "entities/Player.h"
#include "video/Renderer.h"
#include "video/Shader.h"
#include "math/Complex.h"
#include "entities/Hitbox.h"
#include "entities/Transform.h"
#include "video/Mesh.h"
#include "video/Font.h"
#include "entities/Laser.h"
#include "entities/Enemy.h"
#include "util/Timer.h"
#include "game/Pool.h"
#include "util/Script.h"
#include "video/Model.h"
#include "game/Camera.h"
#include "menu/UI.h"
#include "data/ProjData.h"
#include "data/AnimData.h"
#include "data/LuaData.h"
#include "entities/Animation.h"
#include "game/Stage.h"
#include "game/Audio.h"
#include "video/CubeMap.h"
#include "video/Framebuffer.h"
#include "menu/PlayUI.h"
#include "video/Material.h"

#include "util/Log.h"

#define DOWNSCALE_FACTOR 3
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
        int initShaders();
        int initLua();
        int initMaterials();
        int initStage();

        void OnEvent();

        void OnLoop();

        void OnRender();
        void Render(Renderer& renderer);
        void RenderImGuiDebug();

        void OnCleanup();

        void printProgramLog(int program);

        bool isRunning;

        Camera camera;
        Timer timer;
        Pool enemyPool;

        //Font
        Font myFont;

        //Shaders
        Shader shader;
        Shader uiShader;
        Shader fboShader;
        Shader textShader;
        Shader shader3D;
        Shader shaderCube;
        Shader fboIntermediateShader;
        Shader gaussianShader;

        //Materials
        TextureMap<512, 3> materials = TextureMap<512, 3>(6);
        Texture texture;
        Texture normalTest;
        Texture uiTexture;
        Texture cubeMap;
        CubeMap skybox;
        

        Framebuffer frameBuffer = Framebuffer();
        Framebuffer gaussianFramebuffer[2] = {
            Framebuffer(),
            Framebuffer()
        };
        Framebuffer intermediateFramebuffer = Framebuffer();

        Audio audio;

        glm::mat4 orthoPlay;
        glm::mat4 orthoFull;

        glm::mat4 modelMat;
        glm::mat4 projectionMat;

        //UI
        PlayUI gameUI;

        //SDL
        SDL_Event event;
        SDL_Window* window;
        SDL_GLContext context;

        //Window sizes
        float newWidth = DEFAULT_W;
        float newHeight = DEFAULT_H;
        float newScale = 1.f;
        float newPlayWidth = DEFAULT_PLAY_W;
        float newPlayHeight = DEFAULT_PLAY_H;
        float newXOffset = 0;
        float newYOffset = 0;
        float newXPlayOffset = DEFAULT_X_OFFSET;
        float newYPlayOffset = DEFAULT_Y_OFFSET;

        //ImGui Debug
        int frames = 60;
        float delta1[3] = { 0, 0, 0 };
        float delta2[3] = { 0, 0, 0 };
        float delta3[3] = { 0, 0, 0 };

        int aframes = 60;
        float adelta1[2] = { 0, 0 };
        float adelta2[2] = { 0, 0 };
        float adelta3[2] = { 0, 0 };
        
        char InputBuf[256] = { 0 };

        //Player
        Player player;

        //Lua
        sol::state lua;
        sol::function stage1Main; //Only for debug for now

        //Stage
        Stage stage;

};

#endif
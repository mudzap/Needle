#ifndef _TEXTURE_
#define _TEXTURE_

#include <vector>
#include <string>

#include <SDL.h>
//#include <SDL_image.h>
#include <string>
#include "stb_image.h"
#include "GL/glew.h"

class Texture {

    public:

        Texture();
        Texture(const std::string& path);
        ~Texture();

        void LoadTexture(const std::string& path);

        void LoadTextureMipmap(const std::string& path);

        void LoadTextureCubeMap(const std::vector<std::string>& path);
        //void loadCubemap(std::vector<std::string> faces);

        void Bind(unsigned int slot = 0);
        void BindCubeMap(unsigned int slot = 0);
        void Unbind();

        int width, height, bpp;

        unsigned int texture;
        int slot;

    private:

        //std::string filepath;

};

#endif
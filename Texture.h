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
        ~Texture();

        void LoadTexture(const std::string& path, bool keepBuffer);
        void LoadTextureCubeMap(const std::vector<std::string>& path, bool keepBuffer);
        //void loadCubemap(std::vector<std::string> faces);

        void Bind(unsigned int slot = 0) const;
        void BindCubeMap(unsigned int slot) const;
        void Unbind() const;

        unsigned char* localBuffer;
        int width, height, bpp;

        unsigned int texture;

    private:

        //std::string filepath;

};

#endif
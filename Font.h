#ifndef _FONT_
#define _FONT_

//#undef _OLD_FONT_

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Texture.h"
#include "Transform.h"
#include "Mesh.h"

#include "stb_image_write.h"
#include "stb_truetype.h"
#include "stb_rect_pack.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vector.h>
#include <freetype-gl/markup.h>
#include <freetype-gl/font-manager.h>
#include <freetype-gl/vertex-buffer.h>
#include <freetype-gl/edtaa3func.h>
#include <freetype-gl/text-buffer.h>

#include <vector>
#include <array>

enum FontType {
	F_STATIC,
	F_DYNAMIC
};

struct ColorVertex {
	glm::vec2 pos;
	glm::vec2 uv;
	glm::vec4 color;
};

struct Character {
	ColorVertex characterVertices[4];
};

#ifdef _OLD_FONT_
class Font: public Texture, public Mesh {
#else
class Font {
#endif

	public:


#ifdef _OLD_FONT_
		Font();
#else
		Font(unsigned int expectedSize = 0, FontType type = F_STATIC);
#endif

		~Font();

		void StaticDraw();
		void DynamicDraw();

		void InitBuffers(unsigned int expectedSize, FontType type);

		void FillBuffers();

		void LoadTexture(const unsigned int w, const unsigned int h, ftgl::texture_atlas_t* currentatlas);

		void Bind(unsigned int slot);

		void Unbind();

		void LoadTexture(const unsigned int w, const unsigned int h);

#ifdef _OLD_FONT_

		void Load(const std::string fontPath);
		void Prepare(const int w, const int h, const int lh, const int sampling);
		void Add(const int lh, const int sampling);
		void EndLoad();
		void GetQuad(const std::string text, const int font, Complex position);
		void Save(const std::string pngPath);
		std::vector<stbtt_packedchar> packedChar;

#else

		void LoadTexture(const bool keepBuffer);

		//STATIC AND DYNAMIC
		void AddText(ftgl::texture_font_t* font, std::string& text, glm::vec4* color, glm::vec2* pen, unsigned int bytesPerChar = 1);

#endif

		unsigned char* bitmap;

	private:

#ifdef _OLD_FONT_
		unsigned char* localBuffer;
		int lineHeight; /* line height */
		stbtt_pack_context context;
		int fonts;
		int index = 0;
#else
		std::vector<Character> characters;
		std::vector<unsigned int> codepoints;
		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int atlas = 0;
		unsigned int size = 0;
		int slot = 0;

		int* first_index;
		int* length;

#endif

};

#endif
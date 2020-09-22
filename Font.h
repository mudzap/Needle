#ifndef _FONT_
#define _FONT_

#include "Complex.h"
#include "Mesh.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Texture.h"
#include "Transform.h"

#include "stb_image_write.h"
#include "stb_truetype.h"
#include "stb_rect_pack.h"

#include <vector>
#include <array>


struct Character {
	Vertex vertex[4];
	TVertex transform;
};

class Font: public Texture, public Mesh {
	public:

		Font();
		~Font();

		void Load(const std::string fontPath);
		void Prepare(const int w, const int h, const int lh, const int sampling);
		void Add(const int lh, const int sampling);
		void EndLoad();
		void GetQuad(const std::string text, const int font, Complex position);
		void Save(const std::string pngPath);

		void LoadTexture(const bool keepBuffer);

		void Draw();

		unsigned char* bitmap;
		std::vector<stbtt_packedchar> packedChar;


	private:

		unsigned char* localBuffer;
		int lineHeight; /* line height */
		stbtt_pack_context context;
		int fonts;
		int index = 0;
};

#endif
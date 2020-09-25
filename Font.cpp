#include "Font.h"


#ifdef _OLD_FONT_

Font::Font() {
    InitPrimitiveBuffers();
}

Font::~Font() {
    free(bitmap);
}

void Font::Load(const std::string fontPath) {

    /* load font file */
    long size;

    FILE* fontFile = fopen(fontPath.c_str(), "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset  */

    localBuffer = new unsigned char[size];

    fread(localBuffer, size, 1, fontFile);
    fclose(fontFile);
    
}

void Font::Prepare(const int w, const int h, const int lh, const int sampling) {

    width = w;
    height = h;
    bitmap = new unsigned char[width * height];
    fonts = 1;

    if (!stbtt_PackBegin(&context, bitmap, width, height, 0, 1, NULL))
        printf("Failed to initialize font\n");

    packedChar.resize(fonts*('~' - ' '));
    stbtt_PackSetOversampling(&context, sampling, sampling);
    if (!stbtt_PackFontRange(&context, localBuffer, 0, lh, ' ', '~' - ' ', &packedChar[0]))
        printf("Failed to pack font\n");

}


void Font::LoadTexture(const bool keepBuffer) {

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (bitmap && !keepBuffer) {
        stbi_image_free(bitmap);
    }
}


//TODO
void Font::Add(const int lh, const int sampling) {

    fonts++;
    packedChar.resize(fonts * ('~' - ' '));
    stbtt_PackSetOversampling(&context, sampling, sampling);
    if (!stbtt_PackFontRange(&context, localBuffer, 0, lh, ' ', '~' - ' ', &packedChar[(fonts-1) * ('~' - ' ')]))
        printf("Failed to pack font\n");

}

void Font::EndLoad() {
    stbtt_PackEnd(&context);
}

//SAVE BITMAP AS PNG
void Font::Save(const std::string pngPath) {
    stbi_write_png(&pngPath[0], width, height, 1, bitmap, width);
}


void Font::GetQuad(const std::string text, const int font, Complex position) {

    size += text.size();
    vertices.reserve(size);

    //TEXTURE IS FLIPPED, FLIP QUADS
    stbtt_aligned_quad quad;
    position.y *= -1;

    for (int i = 0; i < text.length(); i++) {
        stbtt_GetPackedQuad(&packedChar[font * ('~' - ' ')], width, height, text[i] - ' ', &position.x, &position.y, &quad, 1);
        const float xmin = quad.x0;
        const float xmax = quad.x1;
        const float ymin = -quad.y1;
        const float ymax = -quad.y0;

        vertices.emplace_back(std::array<Vertex, 4> {
            Vertex{ { xmin, ymin },{ quad.s0, quad.t1 } },
            Vertex{ { xmin, ymax },{ quad.s0, quad.t0 } },
            Vertex{ { xmax, ymax },{ quad.s1, quad.t0 } },
            Vertex{ { xmax, ymin },{ quad.s1, quad.t1 } }
        });
    }

}


#else

Font::Font(unsigned int expectedSize, FontType type) : size(expectedSize) {

    characters.reserve(expectedSize);
    codepoints.reserve(expectedSize);
    InitBuffers(expectedSize, type);

}

Font::~Font() {
    glDeleteTextures(1, &atlas);
}

void Font::DynamicDraw() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Character), characters.data());

    glMultiDrawArrays(GL_TRIANGLE_FAN, first_index, length, size);
    
    glBindVertexArray(0);

    characters.clear();

}

void Font::StaticDraw() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glMultiDrawArrays(GL_TRIANGLE_FAN, first_index, length, size);

    glBindVertexArray(0);

}

void Font::InitBuffers(unsigned int expectedSize, FontType type) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, uv));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, color));

    if (type == F_STATIC) {

        glBufferData(GL_ARRAY_BUFFER, expectedSize * sizeof(Character), 0, GL_STATIC_DRAW);

        first_index = new int[size];
        for (int j = 0; j < size; j++) {
            first_index[j] = j * 4;
        }

        length = new int[size];
        for (int j = 0; j < size; j++) {
            length[j] = 4;
        }

    }
    else if (type == F_DYNAMIC) {

        glBufferData(GL_ARRAY_BUFFER, expectedSize * sizeof(Character), 0, GL_DYNAMIC_DRAW);

        first_index = new int[size];
        for (int j = 0; j < size; j++) {
            first_index[j] = j * 4;
        }

        length = new int[size];
        for (int j = 0; j < size; j++) {
            length[j] = 4;
        }

    }
    else {
        printf("Unknown font draw type");
    }


    glBindVertexArray(0);
}

void Font::FillBuffers() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Character), characters.data());

    glBindVertexArray(0);

}

void Font::LoadTexture(const unsigned int w, const unsigned int h, ftgl::texture_atlas_t* currentatlas) {

    glGenTextures(1, &atlas);
    glBindTexture(GL_TEXTURE_2D, atlas);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, currentatlas->data);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Font::Bind(unsigned int slot) {
    this->slot = slot;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, atlas);
}

void Font::Unbind() {
    this->slot = -1;
    glBindTexture(GL_TEXTURE_2D, 0);
}


void Font::AddText(ftgl::texture_font_t* font, std::string& text, glm::vec4* color, glm::vec2* pen, unsigned int bytesPerChar) {
    size_t i;
    float r = color->x, g = color->y, b = color->z, a = color->w;

    for (i = 0; i < text.length()*bytesPerChar; ++i)
    {
        ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, text.c_str() + i);
        if (glyph != NULL)
        {
            float kerning = 0.0f;
            if (i > 0)
            {
                kerning = ftgl::texture_glyph_get_kerning(glyph, text.c_str() + i - 1);
            }
            pen->x += kerning;
            float x0 = pen->x + glyph->offset_x;
            float y0 = pen->y + glyph->offset_y;
            float x1 = x0 + glyph->width;
            float y1 = y0 - glyph->height;
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;

            Character character;
            character.characterVertices[0] = { {x0,y0},  {s0,t0},  {r,g,b,a} };
            character.characterVertices[1] = { {x0,y1},  {s0,t1},  {r,g,b,a} };
            character.characterVertices[2] = { {x1,y1},  {s1,t1},  {r,g,b,a} };
            character.characterVertices[3] = { {x1,y0},  {s1,t0},  {r,g,b,a} };

            characters.emplace_back(character);
            codepoints.emplace_back(glyph->codepoint);

            pen->x += glyph->advance_x;
        }
    }

}

#endif

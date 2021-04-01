#include "Font.h"

Font::Font() {

}

void Font::Init(unsigned int expectedSize, FontType type) {
    size = expectedSize;

    characters.reserve(expectedSize);
    indices.reserve(expectedSize*6);
    InitBuffers(expectedSize, type);
}

Font::~Font() {
    glDeleteTextures(1, &atlas);
}

/*
void Font::Init(unsigned int expectedSize, FontType type) {

    size = expectedSize;
    characters.reserve(expectedSize);
    indices.reserve(expectedSize*6);
    InitBuffers(expectedSize, type);

}
*/

void Font::DynamicDraw() {

    glBindVertexArray(vao);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Character), characters.data());

    glDrawElements(GL_TRIANGLES, size * 6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    characters.clear();

}

void Font::StaticDraw() {

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, size*6, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

}

void Font::InitBuffers(unsigned int expectedSize, FontType type) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, uv));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ColorVertex), (void*)offsetof(ColorVertex, color));

    if (type == F_STATIC) {

        glBufferData(GL_ARRAY_BUFFER, expectedSize * sizeof(Character), 0, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * expectedSize * sizeof(indices), 0, GL_STATIC_DRAW);

    }
    else if (type == F_DYNAMIC) {

        glBufferData(GL_ARRAY_BUFFER, expectedSize * sizeof(Character), 0, GL_DYNAMIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * expectedSize * sizeof(unsigned int), 0, GL_DYNAMIC_DRAW);

    }
    else {
        printf("Unknown font draw type");
    }


    glBindVertexArray(0);
}

void Font::FillBuffers() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, characters.size() * sizeof(Character), characters.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());

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


void Font::AddTextGradient(ftgl::texture_font_t* font, std::string& text, glm::vec4* color, glm::vec2* pen, unsigned int bytesPerChar) {

    size_t i;

    // GET COLOR GRADIENTS
    const float trueLengthF = (float)(text.length() / bytesPerChar);
    const int trueLengthI = text.length() / bytesPerChar;

    //GRADIENTS
    const glm::vec4 upperLeftRight = (color[1] - color[0]) / trueLengthF;
    const glm::vec4 lowerLeftRight = (color[3] - color[2]) / trueLengthF;
    //glm::vec4 leftTopBottom = (color[1] - color[0]) / (float)(text.length() / bytesPerChar);
    //glm::vec4 rightTopBottom = (color[1] - color[0]) / (float)(text.length() / bytesPerChar);

    //INITIAL COLORS
    glm::vec4 topLeftC = color[0];
    glm::vec4 bottomLeftC = color[2];

    for (i = 0; i < trueLengthI; ++i)
    {
        ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, text.c_str() + i * bytesPerChar);
        if (glyph != NULL)
        {
            float kerning = 0.0f;
            if (i > 0)
            {
                kerning = ftgl::texture_glyph_get_kerning(glyph, text.c_str() + (i - 1) * bytesPerChar);
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
            character.characterVertices[0] = { {x0,y0},  {s0,t0},  topLeftC }; //TOP LEFT
            character.characterVertices[1] = { {x0,y1},  {s0,t1},  bottomLeftC }; //BOTTOM LEFT
            character.characterVertices[2] = { {x1,y1},  {s1,t1},  bottomLeftC + lowerLeftRight }; //BOTTOM RIGHT
            character.characterVertices[3] = { {x1,y0},  {s1,t0},  topLeftC + upperLeftRight }; //TOP RIGHT

            topLeftC += upperLeftRight;
            bottomLeftC += lowerLeftRight;

            characters.emplace_back(character);

            unsigned short offset = 4*(indices.size()/6);
            indices.emplace_back(offset + 0);
            indices.emplace_back(offset + 1);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 0);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 3);
            //codepoints.emplace_back(glyph->codepoint);

            pen->x += glyph->advance_x;
        }
    }

}

void Font::AddText(ftgl::texture_font_t* font, std::string& text, glm::vec4* color, glm::vec2* pen, unsigned int bytesPerChar) {

    size_t i;

    float r = color->x, g = color->y, b = color->z, a = color->w;

    for (i = 0; i < text.length() / bytesPerChar; ++i)
    {
        ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(font, text.c_str() + i * bytesPerChar);
        if (glyph != NULL)
        {
            float kerning = 0.0f;
            if (i > 0)
            {
                kerning = ftgl::texture_glyph_get_kerning(glyph, text.c_str() + (i - 1) * bytesPerChar);
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

            unsigned short offset = 4 * (indices.size() / 6);
            indices.emplace_back(offset + 0);
            indices.emplace_back(offset + 1);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 0);
            indices.emplace_back(offset + 2);
            indices.emplace_back(offset + 3);
            //codepoints.emplace_back(glyph->codepoint);

            pen->x += glyph->advance_x;
        }
    }

}
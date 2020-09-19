#include "Font.h"

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
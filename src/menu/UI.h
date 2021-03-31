#ifndef _UI_
#define _UI_

#include <vector>
#include <string>

#include <freetype-gl/freetype-gl.h>
#include <freetype-gl/texture-atlas.h>
#include <freetype-gl/texture-font.h>
#include <freetype-gl/vector.h>
#include <freetype-gl/markup.h>
#include <freetype-gl/font-manager.h>
#include <freetype-gl/vertex-buffer.h>
#include <freetype-gl/edtaa3func.h>
#include <freetype-gl/text-buffer.h>

#include "math/Complex.h"
#include "video/Mesh.h"
#include "video/Font.h"

//LETS CONSIDER
/*	WE NEED:
*	-A WAY TO KEEP TRACK OF THE CURRENTLY POINTED ELEMENT
*	-A LIST OF THE CURRENT ELEMENTS
*	-A WAY TO TRACK THE CURSOR
*	-A CALLBACK FUNCTION
*	-AN ORDERED ID 
*	-THE FOLLOWING ELEMENTS:
*		. MAIN MENU
*		. PAUSE SCREEN
*		. INGAME UI
*		. OPTIONS, SOUND, ETC.. MENUS IN THE MAIN MENU
*	-FADE IN, FADE OUT, LERP EFFECTS
*	-HIERARCHY
*	-HITBOX, DRAWBOX, ETC.. PER MENU AND PER ELEMENT
*/

/*
HIERARCHY WILL BE MADE WITH TWO LEVELS, MASTER AND SLAVE ONLY. NO MORE HIERARCHY LEVELS NEEDED FOR OUR PURPOSES
*/

struct CustomText {
	std::string text;
	glm::vec4 color[4];
	bool shadow = false;
	bool cartoon = false;
	int fontHandle = 0;
};

class IElement {

public:

	void FadeIn();
	void FadeOut();
	void PopUp();
	void PopDown();

private:

	glm::vec4 targetColor;

	CustomText text;
	Quad quad;
	void(*callback)();

};

class MenuElement {

public:

	MenuElement(Quad quad, glm::vec4) : quad(quad) {

	}

	void FadeInMenu();

	void FadeOutMenu();

	void GotoNextElement();
	void GotoPrevElement();

	void MakeActive();
	void MakeInactive();

private:

	void PopUpElement();
	void PopDownElement();

	std::vector<IElement> elements;

	int currentActiveElement;

	Quad quad;

};

class UI {

	public:

		UI(unsigned int width, unsigned int height) : atlasWidth(width), atlasHeight(height) {
			atlas = texture_atlas_new(width, height, 1);
		};

		~UI() {
			texture_atlas_clear(atlas);
			glDeleteTextures(1, &textureID);
		};

		// RETURNS A HANDLE
		int AddFace(unsigned int fontSize, std::string& dir) {
			fonts.push_back(texture_font_new_from_file(atlas, fontSize, dir.c_str()));
			return fonts.size() - 1;
		};

		// FOR DEBUGGING PURPOSES
		void SaveFontAtlas(std::string& dir) {
			stbi_write_png(dir.c_str(), atlasWidth, atlasHeight, 1, atlas->data, atlasWidth * 1);
		};


		void LoadAtlasTexture() {

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlas->data);
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		void Bind(unsigned int slot) {
			this->slot = slot;
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		void Unbind() {
			this->slot = -1;
			glBindTexture(GL_TEXTURE_2D, 0);
		}


	private:

		unsigned int textureID;
		unsigned int slot;

		//MOVE TO MENU
		std::vector<IElement> elements;
		Font dynamicText;
		Font staticFont;

		// CONTAINS ALL THE TEXT TO BE DISPLAYED, MAYBE USEFUL FOR DYNAMIC FONT TO KEEP TRACK OF STUFF
		std::vector<std::string> text;

		// SINGULAR ATLAS
		ftgl::texture_atlas_t* atlas;
		unsigned int atlasWidth;
		unsigned int atlasHeight;

		// STORES ALL FONTS TO BE UTILIZED, CONSIDER DEALLOCATION AND REALLOCATION ON LOADING
		std::vector<ftgl::texture_font_t*> fonts;

};

#endif
#ifndef _UI_
#define _UI_

#include <vector>
#include <string>

#include "Complex.h"
#include "Mesh.h"
#include "Font.h"

enum GUIElementType {

	TEXT = 0,
	BUTTON = 1,
	CHOICE = 2

};

struct TextElement {
	std::string text;
	std::string font;
};

struct ButtonElement {
	std::string text;
	std::string font;
	void (*onClick)();
	void (*onHover)();
};

struct ChoiceElement {
	std::vector<std::string> text;
	std::string font;
	void (*onClick)();
	void (*onHover)();
	unsigned int activeChoice;
	unsigned int choices;
};


struct GUIElement {

	Complex position;
	Quad UVquad;
	GUIElementType type;
	union typeParameters {
		TextElement textType;
		ButtonElement buttonType;
		ChoiceElement choiceType;
	};

};

struct MenuSet {

	Quad drawingQuad;
	Quad UVquad;
	std::vector<GUIElement> elements;

};

/*
MenuSet pauseMenu {

	.drawingQuad = {64, 96, 700, 300},
	.UVquad = {0, 0, 1, 1},
	.elements = {},

};*/

class UI {

	public:

		void PushMenuSet(MenuSet);
		void PopMenuSet();

		MenuSet currentSet;
		Font dynamicFont;
		Font staticFont;

};

#endif
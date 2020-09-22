#ifndef _FRAMEBUFFER_
#define _FRAMEBUFFER_

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl.h"
#include "imgui\imgui_impl_opengl3.h"
#include <SDL.h>
#include <stdio.h>

#include "Mesh.h"

enum FramebufferBehavior {
	FB_DEPTH,
	FB_COLOR
};

class Framebuffer {

	public:

		Framebuffer(unsigned int width, unsigned int height, FramebufferBehavior fboB = FB_COLOR, unsigned int count = 1);
		~Framebuffer();

		void Draw();

		void Bind();

		void BindTexture(unsigned int slot, unsigned int id = 1);

		void Unbind();

	private:

		FramebufferBehavior fboBehavior;

		unsigned int rbo;
		unsigned int fbo;
		unsigned int* texture;
		bool complete = false;

		unsigned int vao;
		unsigned int vbo;
		unsigned int count;

};

#endif
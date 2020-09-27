#include "Framebuffer.h"

#define DOWNSCALE_FACTOR 3


Framebuffer::Framebuffer() {
}

Framebuffer::Framebuffer(unsigned int width, unsigned int height, FramebufferBehavior fboB, unsigned int count, bool useDepth) {

	Init(width, height, fboB, count, useDepth);

}

Framebuffer::~Framebuffer() {

	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(count, texture);
	glDeleteRenderbuffers(1, &rbo);

}


void Framebuffer::ReInit(unsigned int width, unsigned int height, FramebufferBehavior fboB, unsigned int count, bool useDepth) {

	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(count, texture);
	glDeleteRenderbuffers(1, &rbo);

	Init(width, height, fboB, count, useDepth);	

}

void Framebuffer::Init(unsigned int width, unsigned int height, FramebufferBehavior fboB, unsigned int count, bool useDepth) {

	this->count = count;
	fboBehavior = fboB;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);


	//CREATE AND ATTACH TEXTURE TO FRAMEBUFFER
	texture = new unsigned int[count];
	glGenTextures(count, texture);

	unsigned int attachments[count];

	for (unsigned int i = 0; i < count; i++) {

		attachments[i] = GL_COLOR_ATTACHMENT0 + i;

		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width/(i*(DOWNSCALE_FACTOR - 1) + 1), height / (i*(DOWNSCALE_FACTOR - 1) + 1), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture[i], 0);

	}

	glDrawBuffers(count, attachments);

	

	//CREATE AND ATTACH DEPTH BUFFER
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

	//CREATE AND ATTACH RENDER BUFFER
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if (useDepth) {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		complete = true;
	else
		printf("Framebuffer is incomplete!");


	//SETUP FB VAO AND CACHE IT
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex simpleQuad[4] = { //son of a fucking bitch
		{{-1.f, 1.f}, {0.f, 1.f}},
		{{1.f, 1.f}, {1.f, 1.f}},
		{{1.f, -1.f}, {1.f, 0.f}},
		{{-1.f, -1.f}, {0.f, 0.f}}
	};

	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), simpleQuad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);

}

void Framebuffer::Draw() {

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindVertexArray(0);

}

void Framebuffer::Bind() {

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

}

void Framebuffer::BindTexture(unsigned int slot, unsigned int id) {

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture[id - 1]);

}

void Framebuffer::Unbind() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
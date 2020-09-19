#ifndef _RENDERER_
#define _RENDERER_

#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <deque>
#include <set>

#include "Mesh.h"
#include "Shader.h"

class Renderer {

public:
	
	void Draw(const unsigned int vao, const Shader& shader) const;
	void Draw(const unsigned int vao, const unsigned int ibo, const Shader& shader) const;
	void DrawInstanced(const unsigned int vao, const unsigned int ibo, const Shader& shader) const;
	void Clear() const;

	void InitBuffers();

	void ResetDraw();
	void Draw(TVertex* instanceData);
	void Draw(TVertex* instanceData, unsigned int instances);
	void Draw3D();
	void DrawEffects();
	//void DrawParticles(); PER OBJECT!
	void Draw();

private:

	/*
	ORDER		PRIM	VBO		IBO		SHADER
	3D: 		TRIS	3D		NONSEQ	3D		CH
	Effects:	FAN		FX		SEQ		TRANSF	CH
	Particles:	IFAN	BULLET  SEQ		TRANSF		!
J	Entities:	FAN		ENTITY	SEQ		TRANSF
J	Laser: 		STRIP	STRIP	SEQ		TRANSF
	Bullets: 	IFAN	BULLET	SEQ		TRANSF		!
J	Entities2:	FAN		ENTITY	SEQ		TRANSF
J	Text: 		FAN		TEXT2	SEQ		SCHANN	CH
	*/

	//INSTANCED VAO IS PER OBJECT!
	
	/*
	std::deque<unsigned int> 
	std::vector<unsigned int> instances;

	unsigned int instancedVao;
	unsigned int instancedMeshVbo;
	unsigned int instancedTransfVbo;
	std::vector<Vertex> instanceVertices;
	std::vector<Vertex> instanceTVertices;
	*/

	unsigned int fanVao;
	unsigned int fanVbo;
	std::vector<Vertex> fanVertices;

	unsigned int trisVao;
	unsigned int trisVbo;
	unsigned int trisIbo;
	std::vector<unsigned int> trisIndices;
	std::vector<Vertex> trisVertices;

	unsigned int stripVao;
	unsigned int stripVbo;
	std::vector<Vertex> stripVertices;

};


#endif




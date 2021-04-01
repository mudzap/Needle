#include "Mesh.h"

Mesh::Mesh() : size(0) {
}

/*
Mesh::Mesh(const Quad& quad) : size(0) {

	InitTransformBuffers();
	InitQuad(quad);

}
*/

void Mesh::InitMesh(const Quad& quad) {
	size = 0;
	InitTransformBuffers();
	InitQuad(quad);

}

//USE TO PREVENT SEVERAL ALLOCATIONS, NO NEED FOR InitMesh(size)
//RIGHT NOW YOU NEED TO CALL IT ANYWAYS, FUCK YOU OPENGL
Mesh::Mesh(const unsigned int size) : size(0) {

	vertices.reserve(size);

}

Mesh::~Mesh() {
}

#ifdef _MAT4_INSTANCING_
//INITIALIZES BUFFERS AND VERTEX POINTERS ETC
void Mesh::InitTransformBuffers() {

	tvertices.resize(1);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 0 + offsetof(TVertex, transform)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 4 + offsetof(TVertex, transform)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 8 + offsetof(TVertex, transform)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 12 + offsetof(TVertex, transform)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex), 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::InitTransformBuffers(unsigned int maxInstances) {

	tvertices.resize(1);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 0 + offsetof(TVertex, transform)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 4 + offsetof(TVertex, transform)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 8 + offsetof(TVertex, transform)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(float) * 12 + offsetof(TVertex, transform)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * maxInstances, 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

#else

void Mesh::InitTransformBuffers() {

	tvertices.resize(1);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, transform));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex), 0, GL_STREAM_DRAW);

	glBindVertexArray(0);
}

void Mesh::InitTransformBuffers(unsigned int maxInstances) {

	tvertices.resize(1);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, translation));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)offsetof(TVertex, transform));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);


	glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * maxInstances, 0, GL_STREAM_DRAW);

	glBindVertexArray(0);
}

#endif

void Mesh::InitPrimitiveBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void Mesh::Init3DBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glGenBuffers(1, &instanceVbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glBindVertexArray(0);
}

// SINGLE MESH, CONSTANT
void Mesh::InitQuad(const Quad& args) {
	const float textConst = 1.f / 1024.f;

	Quad quad = args;

	quad.y -= 0.5;
	quad.h -= 0.5;

	vertices.resize(1);

	vertices[0][0] = {
		{-quad.w * 0.5f, -quad.h * 0.5f},
		{textConst * quad.x, textConst * (1024.f - (quad.y + quad.h))}
	};
	vertices[0][1] = {
		{quad.w * 0.5f, -quad.h * 0.5f},
		{textConst * (quad.x + quad.w),	textConst * (1024.f - (quad.y + quad.h))}
	};
	vertices[0][2] = {
		{quad.w * 0.5f, quad.h * 0.5f},
		{textConst * (quad.x + quad.w),	textConst * (1024.f - quad.y)}
	};
	vertices[0][3] = {
		{-quad.w * 0.5f, quad.h * 0.5f},
		{textConst * quad.x, textConst * (1024.f - quad.y)}
	};

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	unsigned int indices[6] = { 0, 1, 2, 0, 2, 3 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void Mesh::ResetDraw() {

	int first_index[size];
	for (int j = 0; j < size; j++) {
		first_index[j] = j * 4;
	}

	int length[size];
	for (int j = 0; j < size; j++) {
		length[j] = 4;
	}

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size * 4 * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);

	glMultiDrawArrays(GL_TRIANGLE_FAN, first_index, length, size);

	glBindVertexArray(0);

	vertices.clear();
	size = 0;

}


//IBO AND VERTEX ALREADY DEFINED
void Mesh::Draw(TVertex& instanceData) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TVertex), &instanceData);

	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

//DRAWS INSTANCED ELEMENT, VERTEX ALREADY EXISTS SINCE IT IS CONSTANT
void Mesh::Draw(TVertex& instanceData, unsigned int instances) {

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TVertex) * instances, &instanceData);

	//glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instances);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances);

	glBindVertexArray(0);

}

//ONLY IF NO BUFFERS ARE CORRECTLY SETUP
void Mesh::Draw(Complex& translation, Complex& transform, unsigned int instances) {

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(Complex), sizeof(Complex) * instances, &translation);
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(Complex), sizeof(Complex) * instances, &transform);

	//glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instances);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, instances);

	glBindVertexArray(0);

}
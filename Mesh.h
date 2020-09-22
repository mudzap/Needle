#ifndef _MESH_
#define _MESH_

#include <algorithm>
#include "Complex.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>
#include <array>

struct Vertex {
	Complex position = { 0.f, 0.f };
	Complex uv = { 0.f, 0.f };
};
const Vertex defaultVertex;

struct TVertex {
	Complex translation = { 0.f, 0.f };
	glm::mat4 transform = glm::mat4(1.f);
};
const TVertex defaultTVertex;

struct Quad {
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;

	bool operator == (const Quad& other) const {
		return (x == other.x && y == other.y);
	}
};
const Quad defaultQuad;

template <>
struct std::hash<Quad> {
	std::size_t operator() (const Quad& k) const
	{
		using std::size_t;
		using std::hash;

		return ((hash<float>()(k.x)
			^ (hash<float>()(k.y) << 1)) >> 1);
	}
};

class Mesh {

	public:

		Mesh(const Quad& quad);
		Mesh(const unsigned int size);
		Mesh();
		~Mesh();


		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int instanceVbo = 0;
		unsigned int ibo = 0;
		bool instanced;

		void InitTransformBuffers();
		void InitTransformBuffers(unsigned int maxInstances);
		void InitPrimitiveBuffers();
		void Init3DBuffers();

		void InitMesh(const Quad& quad);

		void ResetDraw();
		void Draw(TVertex& instanceData);
		void Draw(TVertex& instanceData, unsigned int instances);

		std::vector<std::array<Vertex,4>> vertices;
		std::vector<unsigned int> indices;
		std::vector<TVertex> tvertices;
		int size;


};

#endif
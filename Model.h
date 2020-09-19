#ifndef _MODEL_
#define _MODEL_

#include "glm/glm.hpp"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <vector>
#include <algorithm>
#include <cstdarg>

#include "OBJ_Loader.h"
#include "Complex.h"


struct vec2 {
	float x, y;
};

struct vec3 {
	float x, y, z;
};

struct Vertex3D {
	vec3 position = { 0.f, 0.f, 0.f };
	vec3 normals = { 0.f, 0.f, 0.f };
	Complex uv = { 0.f, 0.f };
};
const Vertex3D defaultVertex3D;

struct Mesh3D {
	//UNEEDED
};

class Model {

	public:

		Model(const std::string& filepath);
		Model();
		~Model();

		void Draw();

		void Draw(const unsigned int modelID);

		void FillBuffer();

		void InitBuffer();

		void LoadModel(const std::string& filepath);

	private:

		std::vector<objl::Mesh> mesh;
		std::vector<unsigned int> verticesOffset;
		std::vector<unsigned int> indicesOffset;

		unsigned int vertexSize;
		unsigned int indexSize;

		std::vector<unsigned int> vao;
		std::vector<unsigned int> vbo;
		std::vector<unsigned int> ibo;

};

#endif // !_MODEL
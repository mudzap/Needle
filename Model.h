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
#include "Texture.h"

struct Material {

	// AMBIENT
	objl::Vector3 Ka;

	// DIFFUSE
	objl::Vector3 Kd;

	// SPECULAR
	objl::Vector3 Ks;

	// SPECULAR EXPONENT
	float Ns;

	//TRANSPARENCY
	float Tr;

	//float textureID;

};

struct Mesh3D {
	// Mesh Name
	std::string meshName;

	// Vertex List
	std::vector<objl::Vertex> vertices;

	// Index List
	std::vector<unsigned int> indices;

	// Material
	Material meshMaterial;
};

class Model {

	public:

		Model(const std::string& filepath, const std::string& texture, unsigned int slot);
		Model();
		~Model();

		void LoadModel(const std::string& filepath, const std::string& texture, unsigned int slot);

		void Draw();

		void Draw(const unsigned int modelID);

		void FillBuffer();

		void InitBuffer();

		//void LoadModel(const std::string& filepath);

		std::vector<Texture> textures;

	private:

		std::vector<Mesh3D> mesh;
		std::vector<unsigned int> verticesOffset;
		std::vector<unsigned int> indicesOffset;

		unsigned int vertexSize;
		unsigned int indexSize;

		std::vector<unsigned int> vao;
		std::vector<unsigned int> vbo;
		std::vector<unsigned int> matVbo;
		std::vector<unsigned int> instanceVbo;
		std::vector<unsigned int> ibo;

};

#endif // !_MODEL
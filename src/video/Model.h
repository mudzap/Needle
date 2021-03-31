#ifndef _MODEL_
#define _MODEL_

#include "glm/glm.hpp"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <vector>
#include <algorithm>
#include <cstdarg>

#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags

#include "Complex.h"
#include "Texture.h"
#include "Material.h"

struct Vertex3D {

	// Position Vector
	glm::vec3 position;

	// Normal Vector
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	// Texture Coordinate Vector
	glm::vec2 uv;

};

struct Mesh3D {

	// Mesh Name
	std::string meshName;

	// Vertex List
	std::vector<Vertex3D> vertices;

	// Index List
	std::vector<unsigned int> indices;

	// Material
	Material meshMaterial;

};

class Model {

	public:

		Model();
		~Model();

		void LoadModel(const std::string& filepath, const std::string& texture, unsigned int slot);

		void StartDraw();
		void Draw(const unsigned int modelID);
		void EndDraw();

		void FillBuffer();

		void InitBuffer();

		Mesh3D GetMesh(const unsigned int modelID);

		//void LoadModel(const std::string& filepath);

		std::vector<Texture> textures;

	private:

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh3D ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Mesh3D> mesh;

		unsigned int vertexSize;
		unsigned int indexSize;

		unsigned int vao;
		std::vector<unsigned int> vbo;
		std::vector<unsigned int> matVbo;
		std::vector<unsigned int> ibo;

};

#endif // !_MODEL
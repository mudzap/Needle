#ifndef _STAGE_
#define _STAGE_

#define TILE_W 128.f
#define TILE_L 128.f
#define TILE_H 32.f

#define UNIT_CUBE_DIAGONAL_LENGTH 1.732f
#define EASE_CONSTANT 0.9f
#define SPHERE_RADIUS (TILE_W * UNIT_CUBE_DIAGONAL_LENGTH * EASE_CONSTANT)


#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include "OBJ_Loader.h"

#include "Model.h"
#include "Complex.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

const Complex NORTH = { 0, 1 };
const Complex EAST = { 1, 0 };
const Complex SOUTH = { 0, -1 };
const Complex WEST = { -1, 0 };

struct itrio {
	int x = 0;
	int y = 0;
	int z = 0;
};

struct StageTileInformation {
	Complex cardinalDirection = NORTH;
	itrio tile = { 0, 0, 0 };
	unsigned int objectModelID = 0;
	glm::mat4 tileViewMatrices = glm::mat4(1.f);

	bool operator< (const StageTileInformation& rhs) {
		return (objectModelID < rhs.objectModelID);
	}
};

class Stage : public Model {

	public:
		
		void DrawTilesOcclude(const Camera& camera, Shader& shader);

		void StartDraw();
		void Draw(const unsigned int modelID);
		void EndDraw();

		void PushBackStageTile(const Complex cardinalDirection, const itrio tile, unsigned int objectID);
		void PopBackStageTile();
		void GetDrawables(const Camera& camera);
		void DrawTilesOcclude(Shader& shader);
		void SetStageTile(const unsigned int index, const Complex cardinalDirection, const itrio tile, const unsigned int objectID);
		void SortTiles();

		void ClearTiles();

		void FillBuffer();
		void InitBuffer();


	private:


		std::vector<StageTileInformation> tiles;
		std::vector<Mesh3D> mesh;
		std::vector<unsigned int> drawables;
		std::vector<unsigned int> elementsPerMaterial;

		unsigned int vertexSize;
		unsigned int indexSize;

		unsigned int vao;
		std::vector<unsigned int> vbo;
		std::vector<unsigned int> matVbo;
		std::vector<unsigned int> ibo;

};

#endif
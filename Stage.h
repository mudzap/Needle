#ifndef _STAGE_
#define _STAGE_

#define TILE_W 128.f
#define TILE_L 128.f
#define TILE_H 32.f

#define UNIT_CUBE_DIAGONAL_LENGTH 1.732f
#define EASE_CONSTANT 0.9f

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include <vector>
#include <string>
#include "OBJ_Loader.h"

#include "Model.h"
#include "Complex.h"
#include "Shader.h"
#include "Camera.h"

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
};

class Stage : public Model {

	public:

		void PushBackStageTile(const StageTileInformation& tileInformation);
		void PushBackStageTile(const Complex cardinalDirection, const itrio tile, unsigned int objectID);
		void PopBackStageTile();
		void SetStageTile(const unsigned int index, const StageTileInformation& tileInformation);
		void DrawTile(const unsigned int index, Shader& shader);
		void DrawTilesOcclude(const Camera& camera, Shader& shader);
		void DrawTiles(Shader& shader);

		void ClearTiles();

	private:

		std::vector<StageTileInformation> tiles;
		std::vector<glm::mat4> tileViewMatrices;

};

#endif
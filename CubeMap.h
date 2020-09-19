#ifndef _CUBEMAP_
#define _CUBEMAP_

#include "Mesh.h"
#include "Model.h"

struct CubeMap: public Model {

	CubeMap();
	~CubeMap();
	void FillBuffer();
	void Draw();

};

#endif
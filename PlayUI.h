#ifndef _PLAYUI_
#define _PLAYUI_

//const float _DEFAULT_W_ = 1280;
//const float _DEFAULT_H_ = 960;

#include "Mesh.h"
#include <vector>
#include <array>

#define _UI_TSIZE_ 1024.f
#define _DEFAULT_W_ = 640.0f;
#define _DEFAULT_H_ = 480.0f;


class PlayUI {

	public:

		PlayUI();

		void Draw();

	private:

		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int ibo = 0;

		Vertex playUI[16] = {
			{{-640.f / 640.0f, 480.f / 480.0f}, {0.f / _UI_TSIZE_, 1024.f / _UI_TSIZE_}},//
			{{-576.f / 640.0f, 480.f / 480.0f}, {68.f / _UI_TSIZE_, 1024.f / _UI_TSIZE_}},
			{{-576.f / 640.0f, -480.f / 480.0f}, {68.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},
			{{-640.f / 640.0f, -480.f / 480.0f}, {0.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},
			{{-576.f / 640.0f, 480.f / 480.0f}, {68.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},//
			{{160.f / 640.0f, 480.f / 480.0f}, {68.f / _UI_TSIZE_, 820.f / _UI_TSIZE_}},
			{{160.f / 640.0f, 448.f / 480.0f}, {102.f / _UI_TSIZE_, 820.f / _UI_TSIZE_}},
			{{-576.f / 640.0f, 448.f / 480.0f}, {102.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},
			{{-576.f / 640.0f, -448.f / 480.0f}, {102.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},//
			{{160.f / 640.0f, -448.f / 480.0f}, {102.f / _UI_TSIZE_, 820.f / _UI_TSIZE_}},
			{{160.f / 640.0f, -480.f / 480.0f}, {136.f / _UI_TSIZE_, 820.f / _UI_TSIZE_}},
			{{-576.f / 640.0f, -480.f / 480.0f}, {136.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},
			{{160.f / 640.0f, 480.f / 480.0f}, {136.f / _UI_TSIZE_, 1024.f / _UI_TSIZE_}},//
			{{640.f / 640.0f, 480.f / 480.0f}, {614.f / _UI_TSIZE_, 1024.f / _UI_TSIZE_}},
			{{640.f / 640.0f, -480.f / 480.0f}, {614.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}},
			{{160.f / 640.0f, -480.f / 480.0f}, {136.f / _UI_TSIZE_, 0.f / _UI_TSIZE_}}
		};

		unsigned int playIndices[4*6] = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15
		};

};

#endif
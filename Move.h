#ifndef _MOVE_
#define _MOVE_

#include "Matrix2x2.h"
#include "Vector2.h"
#include "SDL.h"
#include <vector>
#include "Renderer.h"
#include <functional>
#include "Complex.h"

/*
struct Vertex {
	float position[2] = { 0, 0 };
	float uv[2] = { 0, 0 };
};
//typedef struct Vertex Vertex;

struct TVertex {
	Complex translation = { 0.f, 0.f };
	glm::mat4 transform = glm::mat4(1.f);
};

struct Quad {
	float x;
	float y;
	float w;
	float h;

	bool operator == (const Quad& other) const {
		return (x == other.x && y == other.y);
	}
};
//typedef struct Quad Quad;

template <>
struct std::hash<Quad> {
	std::size_t operator() (const Quad& k) const
	{
		using std::size_t;
		using std::hash;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return ((hash<float>()(k.x)
			^ (hash<float>()(k.y) << 1)) >> 1);
	}
};
*/

struct EntityArgs {
	Quad quad;
	Complex position = { 0, 0 };
	Complex velocity = { 0, 0 };
	Complex acceleration = { 0, 0 };
	float rotation = 0;
	float rotationDelta = 0;
	float visualRotation = 0;
};

EntityArgs defaultEntity;

class Move
{
	public:

		Move();
		Move(EntityArgs *args);
		~Move();
		EntityArgs entityArgs;
		
		void TranslateCartesian(Move* entity, Complex translation);
		void Translate(Move* entity, Complex translation, float angle);
		void Accelerate(Move* entity, Complex acceleration);
		void UpdateKinematicCartesian(Move* entity);
		void UpdateKinematic(Move* entity);
		void UpdateAngle(Move* entity);
		void Rotate(Move* entity, float angle);

		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int instanceVbo = 0;
		unsigned int ibo = 0;
		bool instanced = false;

		void Initialize();
		void Draw(unsigned int instances);
		Vertex* vertices = new Vertex[4];
		unsigned int* indices = new unsigned int[4];

	private:

		
		//std::vector<float> vertices;
		//std::vector<unsigned int> indices;


};

#endif
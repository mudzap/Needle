#ifndef _LASER_
#define _LASER_

#include "Mesh.h"
#include "Transform.h"
#include "Complex.h"
#include <vector>
#include <array>
#include "Timer.h"
#include <algorithm>

struct LaserArgs {
	unsigned int laserInterval = 2;
	float laserWidth = 16.f;
	Quad quad = { 0, 768, 128, 32 };
	unsigned int nodes = 32;
};

class Laser: public Mesh {

	public:

		Laser();
		Laser(LaserArgs& laser, TransformArgs& transform);
		~Laser();

		std::vector<Complex> nodePos;
		std::vector<Complex> nodeVel;

		std::vector<std::array<Complex,2>> nodeNormals;

		std::vector<Complex> nodeRotation;
		std::vector<Complex> nodeRotationDelta;
		std::vector<unsigned int> nodeLifetime;

		LaserArgs laser;
		TransformArgs transform;
		unsigned int currentNodes;


		void SetRotationDelta();
		void InitNode();
		void HandleNodes();
		std::array<Complex, 2> GetNormals(Complex vector);
		void GetVertex();
		void ResetDraw();
		float SquaredDistanceToLaserSegment(const Laser& laser, const Complex position, const int ID);
		void InitLaser();

	private:

		std::vector<std::array<Vertex, 2>> vertices;
		//float nodeSpawnInterval; //nodeSpawnInterval = laserLength / (nodes * nodeVel)
		Complex initialPosition;
		Complex initialVelocity;
		Complex initialRotation;
		Complex initialRotationDelta;

};

#endif
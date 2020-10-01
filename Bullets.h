#ifndef _BULLETS_
#define _BULLETS_

#include <vector>
#include "Projectile.h"
#include "Complex.h"
#include "Transform.h"
#include "Mesh.h"
#include "Hitbox.h"
#include <algorithm>
#include <thread>
#include <unordered_map>

class Player;
struct SpawnerArgs;

class Bullets: public Mesh {

	public:

		Bullets();
		Bullets(const Projectile& projectile, const int reserveSize);
		//Bullets(const int reserveSize);
		~Bullets();

		void InitSpawner(const Projectile& projectile);
		//void InitSpawner();
		void ClearBullet(const int ID);
		void BatchClearBullets();
		void ScissorTest();
		void BatchSwap();
		void BatchSwap(const int i);

		Complex aimSpot;

#ifdef _USE_SSE2_
		using vector_cmplx = std::vector<float, XSIMD_DEFAULT_ALLOCATOR(Complex)>;
		using vector_float = std::vector<float, XSIMD_DEFAULT_ALLOCATOR(Complex)>;
		using vector_uint = std::vector<float, XSIMD_DEFAULT_ALLOCATOR(Complex)>;

		std::vector<Complex> position;
		std::vector<Complex> velocity;
		std::vector<Complex> acceleration;

		std::vector<unsigned int> grazeable;

		std::vector<Complex> cartesianVelocity;

		std::vector<Complex> visualRotation;
		std::vector<Complex> rotationMat;

		std::vector<unsigned int> sineTimer;
		std::vector<unsigned int> stopAndGoTimer;
		std::vector<unsigned int> zigzagTimer;
#else
		std::vector<Complex> position;
		std::vector<Complex> velocity;
		std::vector<Complex> acceleration;

		std::vector<unsigned int> grazeable;

		std::vector<Complex> cartesianVelocity;

		std::vector<Complex> visualRotation;
		std::vector<Complex> rotationMat;

		std::vector<unsigned int> sineTimer;
		std::vector<unsigned int> stopAndGoTimer;
		std::vector<unsigned int> zigzagTimer;

#endif

		std::vector<unsigned int> cullList;


		void Instantiate(const Projectile& projectile, const Complex& bulletPosition, const SpawnerArgs& spawner);

		void InitBullet(const int ID);
		//void InitMesh();

		void ProcessBullet(const int ID);
		void BatchProcessBullets();
		void FillVertices();

		void DeleteBullet(const int ID);
		void BatchDeleteBullets();

		inline void Accelerated(const int ID);
		inline void Exponential(const int ID);
		inline void Sinusoidal(const int ID);
		inline void Zigzagging(const int ID);
		inline void Deflected(const int ID);
		inline void Gravity(const int ID);
		inline void StopAndGo(const int ID);

		inline void BatchAccelerated();
		inline void BatchExponential();
		inline void BatchSinusoidal();
		inline void BatchZigzagging();
		inline void BatchDeflected();
		inline void BatchGravity();
		inline void BatchStopAndGo();

		unsigned int indexPointer;
		unsigned int tempPointer;
		unsigned int trashSize;
		unsigned int reserveSize;
		unsigned int currentSize;

		Complex initialAccel;
		float exponent;
		float sineAmplitude;
		Complex desiredVelocity;
		float zigzagAmplitude;
		float squaredHitboxRadius;
		unsigned int shouldGraze;
		unsigned int rotates;
		Complex gravity;
		Quad quad;
		Complex initialRotationMat;
		Complex rotationDeltaMat;
		Complex zigzagRotationMat;
		float sineFreqConstant;
		float stopAndGoFreqConstant;
		float zigzagFreqConstant;
		std::vector <void(Bullets::*)()> bulletMethods;

		std::unordered_map< Quad, std::array<Vertex, 4>> meshCache;


};

#endif
#ifndef _ENEMY_
#define _ENEMY_

#include "Transform.h"
#include "Mesh.h"
#include "Complex.h"
#include "Hitbox.h"
#include "Projectile.h"
//#include "Timer.h"
#include "Spawner.h"
#include "Animation.h"

enum EnemyState {
	MOVE_TOWARDS,
	SPRING_TOWARDS,
	SLOW_TOWARDS,
	STOP,
	FULL_STOP,
	DRIFT,
	DEAD
};

class Enemy: public Transform, public Animation, public Hitbox {

	public:

		//TAKES ITEM, SPAWNER, 
		Enemy();
		Enemy(const TransformArgs& entity, const AnimationArgs& animArgs);
		Enemy(const Complex position, const AnimationArgs& animArgs);
		~Enemy();
		void SetPosition(Complex position);
		void Destroy();
		void MoveTowards1(Complex position, float speed, float accel);
		void MoveTowards2(float angle, Complex speed, Complex accel);
		void SpringTowards1(Complex position, float accelPerUnit);
		void SpringTowards2(float angle, float distance, float accelPerUnit);
		void SlowTowards1(Complex position, float velocityPerUnit);
		void SlowTowards2(float angle, float distance, float velocityPerUnit);
		void Stop1();
		void Stop2(float brakeFactor);
		void Drift1();
		void Drift2(float friction);
		void CircleAround(Complex positon);

		void AddSpawner(const Spawner& spawner);
		void AddSpawnerManual(const Complex offset,  const unsigned int reserveSize, const SpawnerArgs& spawner, const Projectile& projectile);
		void AddSpawnerConstant(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const ConstantArgs& constant);
		void AddSpawnerBarrage(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const BarrageArgs& barrage);
		void AddSpawnerRandom(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const RandomArgs& random);

		void HandleEnemy();

		void DrawBullets();

		void Draw();

		std::vector<Spawner> enemySpawners;

	private:

		Complex direction;
		float exponent;
		float velocityPerUnit;
		float accelPerUnit;
		Complex targetPosition;
		float brakeConstant;
		EnemyState state = STOP;

};

#endif
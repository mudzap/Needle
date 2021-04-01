#ifndef _SPAWNER_
#define _SPAWNER_

#define CONSTANT 0
#define BARRAGE 1
#define RANDOM 2

#include <vector>
#include <deque>
#include <variant>

#include "video/Texture.h"
#include "SDL2/SDL.h"
#include "math/Complex.h"
#include "util/Timer.h"
#include "math/RNG.h"

#include "Projectile.h"
#include "Bullets.h"
#include "Transform.h"

struct SpawnerArgs {
	float angle = 0.f;
	float angleDelta = 0.f;
	float angleAccel = 0.f;

	float loopingConeStart = 0.f;
	float loopingConeEnd = 0.f;
};
const SpawnerArgs defaultSpawner;

struct ConstantArgs {
	Projectile baseProjectile = defaultProjectile;
	bool aimed = false;

	float initialBulletAngle = 0.f;
	unsigned int bulletArrays = 1;
	float angleBetweenArray = 0.f;
	unsigned int bulletPerArray = 1;
	float angleBetweenBullets = 0.f;
	float bulletOffsetRadius = 0.f;
	unsigned int startTimer = 0;
	unsigned int stopTimer = 0;
	unsigned int bulletShotTimer = 0;
	Complex perArrayAccel = { 0.f, 0.f };
	Complex perBulletAccel = { 0.f, 0.f };
};
const ConstantArgs defaultConstant;

struct BarrageArgs {
	Projectile baseProjectile = defaultProjectile;
	bool aimed = false;

	float initialBulletAngle = 0.f;
	unsigned int bulletArrays = 1;
	float angleBetweenArray = 0.f;
	unsigned int bulletPerArray = 1;
	float angleBetweenBullets = 0.f;
	float bulletOffsetRadius = 0.f;
	unsigned int startTimer = 0;
	unsigned int stopTimer = 0;
	unsigned int bulletShotTimer = 0;
	Complex perArrayAccel = { 0.f, 0.f };
	Complex perBulletAccel = { 0.f, 0.f };
	Complex perBarrageAccel = { 0.f, 0.f };

	unsigned int barrageTimer = 1000;
	unsigned int bulletsPerBarrage = 3;
	//unsigned int barrageBulletsPerArray = 10;
};
const BarrageArgs defaultBarrage;

struct RandomArgs {
	Projectile baseProjectile = defaultProjectile;
	bool aimed = false;

	float initialBulletAngle = 0.f;
	unsigned int bulletArrays = 1;
	float angleBetweenArray = 0.f;
	unsigned int bulletPerArray = 1;
	float bulletOffsetRadius = 0.f;
	unsigned int startTimer = 0;
	unsigned int stopTimer = 0;
	unsigned int bulletShotTimer = 0;
	Complex perBulletAccel = { 0.f, 0.f };

	float speedRange[2] = { 0.9f, 1.1f };
	float accelRange[2] = { 0.01f, 0.02f };
	float coneRange[2] = { 0, 360 };
	//float loopingCone[2] = { 0, 90 };
};
const RandomArgs defaultRandom;

struct PlayerSpawnerArgs {
	Projectile baseProjectile = defaultProjectile;
	bool aimed = false;

	float initialBulletAngle = 0.f;
	unsigned int bulletArrays = 1;
	float angleBetweenArray = 0.f;
	unsigned int bulletPerArray = 1;
	float angleBetweenBullets = 0.f;
	float bulletOffsetRadius = 0.f;
	unsigned int bulletShotTimer = 0;
};
const PlayerSpawnerArgs defaultPlayerSpawner;

class Player;
struct PlayerArgs;

class Spawner : public Transform, public Bullets {
public:

	Spawner();

	Spawner(
		const Complex offset,
		const unsigned int reserveSize,
		const SpawnerArgs& spawner,
		const Projectile& projectile = defaultProjectile
	);
	Spawner(
		const Complex offset,
		const unsigned int reserveSize,
		const SpawnerArgs& spawner,
		const ConstantArgs& constant = defaultConstant
	);
	Spawner(
		const Complex offset,
		const unsigned int reserveSize,
		const SpawnerArgs& spawner,
		const BarrageArgs& barrage = defaultBarrage
	);
	Spawner(
		const Complex offset,
		const unsigned int reserveSize,
		const SpawnerArgs& spawner,
		const RandomArgs& random = defaultRandom
	);
	Spawner(
		const Complex offset,
		const unsigned int reserveSize,
		const SpawnerArgs& spawner,
		const PlayerSpawnerArgs& playerSpawner = defaultPlayerSpawner
	);

	~Spawner();

	//inline void InstantiateBullet(Bullets* const pool, const Projectile* projectile, const Complex* position);

	inline void InitializeConstantSpawner();
	inline void InitializeEmptySpawner();
	inline void InitializeBarrageSpawner();
	inline void InitializeRandomSpawner();
	inline void InitializePlayerSpawner();

	void ClearCollideable();
	void PushCollideable(unsigned int i);

	void HandleSpawner();
	void HandleEmpty();
	void HandleConstantSpawner();
	void HandleBarrageSpawner();
	void HandleRandomSpawner();
	bool HandlePlayerSpawner();

	void ResetTimer();


	inline void GetRandom();

	inline void LoopAtCone();

	void InstantiateConstantPattern();
	void InstantiateBarragePattern();
	void InstantiateRandomPattern();

	SpawnerArgs spawner;

	//union doesn't work for some reason
	//union { //ANONYMOUS UNION
		Projectile projectile;
		ConstantArgs constant;
		BarrageArgs barrage;
		RandomArgs random;
		PlayerSpawnerArgs playerSpawner;
	//};
	
	Complex offset;
	unsigned int* collideableBullets;
	unsigned int colliderSize;
	unsigned int collideables = 0;

private:

	void (Spawner::* handlingFunction)() = NULL;

	//unsigned int currentBullet = 0;
	bool stopBarrage;
	unsigned int barrageResettable;
	unsigned int barrageBullets;
	Projectile tempProjectile[3];

	//CLEANUP ARRAYS, ONLY ONE OBJECT/PRIMITIVE NEEDED 

	float currentBulletAngle[3];
	Complex currentBulletSpeed[3];
	Complex currentBulletAccel[3];
	Complex playerPosition;
	unsigned int timer[3];
	unsigned int resettableTimer[3];
	bool shouldFire[3];

};

#endif
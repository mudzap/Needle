#include "Spawner.h"
#include "Player.h"
#include "util/Timer.h"
#include "game/Audio.h"

/*
TODO:	MAKE PLAYER FINDER
		UNIFY PLAYER FINDER, ONLY ONE CALL NEEDED PER INITIALIZATION/HANDLER CALL
*/

Spawner::Spawner(){
}


//MANUAL SPAWNER CONSTRUCTOR
Spawner::Spawner(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const Projectile& projectile) :
	Bullets(projectile, reserveSize),
	spawner(spawner),
	specialArgs(projectile)
{
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
	InitializeEmptySpawner();
	handlingFunction = &Spawner::HandleEmpty;
}

//CONSTANT SPAWNER CONSTRUCTOR
Spawner::Spawner(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const ConstantArgs& constant) :
	Bullets(constant.baseProjectile, reserveSize),
	spawner(spawner),
	specialArgs(constant),
	offset(offset)
{
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
	InitializeConstantSpawner();
	handlingFunction = &Spawner::HandleConstantSpawner;
}

//BARRAGE SPAWNER CONSTRUCTOR
Spawner::Spawner(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const BarrageArgs& barrage) :
	Bullets(barrage.baseProjectile, reserveSize),
	spawner(spawner),
	specialArgs(barrage),
	offset(offset)
{
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
	InitializeBarrageSpawner();
	handlingFunction = &Spawner::HandleBarrageSpawner;
}

//RANDOM SPAWNER CONSTRUCTOR
Spawner::Spawner(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const RandomArgs& random) :
	Bullets(random.baseProjectile, reserveSize),
	spawner(spawner),
	specialArgs(random),
	offset(offset)
{
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
	InitializeRandomSpawner();
	handlingFunction = &Spawner::HandleRandomSpawner;
}

Spawner::Spawner(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const PlayerSpawnerArgs& playerSpawner) :
	Bullets(playerSpawner.baseProjectile, reserveSize),
	spawner(spawner),
	specialArgs(playerSpawner),
	offset(offset)
{
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
	InitializePlayerSpawner();
	handlingFunction = &Spawner::HandleEmpty;
}

Spawner::~Spawner() {
}

void Spawner::HandleSpawner() {
	(*this.*handlingFunction)();
}

void Spawner::ClearCollideable() {
	collideables = 0;
}

void Spawner::PushCollideable(unsigned int i) {
	if (collideables < colliderSize) {
		collideableBullets[collideables] = i;
		collideables++;
	}
}

inline void Spawner::InitializeConstantSpawner() {

	tempProjectile[CONSTANT] = specialArgs.baseProjectile;
	currentBulletAccel[CONSTANT] = specialArgs.baseProjectile.projectile.acceleration;
	currentBulletSpeed[CONSTANT] = specialArgs.baseProjectile.projectile.initialVel;
	timer[CONSTANT] = Timer::lifeTimeFrames;
	resettableTimer[CONSTANT] = Timer::lifeTimeFrames;

	if (specialArgs.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}

}

inline void Spawner::InitializeBarrageSpawner() {

	tempProjectile[BARRAGE] = specialArgs.baseProjectile;
	currentBulletAccel[BARRAGE] = specialArgs.baseProjectile.projectile.acceleration;
	currentBulletSpeed[BARRAGE] = specialArgs.baseProjectile.projectile.initialVel;
	timer[BARRAGE] = Timer::lifeTimeFrames;
	resettableTimer[BARRAGE] = Timer::lifeTimeFrames;

	if (specialArgs.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}


}
inline void Spawner::InitializeRandomSpawner() {

	tempProjectile[RANDOM] = specialArgs.baseProjectile;
	GetRandom();
	timer[RANDOM] = Timer::lifeTimeFrames;
	resettableTimer[RANDOM] = Timer::lifeTimeFrames;

}

inline void Spawner::InitializeEmptySpawner() {

	tempProjectile[0] = specialArgs;
	timer[0] = Timer::lifeTimeFrames;
	resettableTimer[0] = Timer::lifeTimeFrames;

	/*
	if (projectile.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}
	*/
}

inline void Spawner::InitializePlayerSpawner() {

	tempProjectile[0] = specialArgs;
	timer[0] = Timer::lifeTimeFrames;
	resettableTimer[0] = Timer::lifeTimeFrames;

	/*
	if (projectile.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}
	*/

}

bool Spawner::HandlePlayerSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	if (Timer::lifeTimeFrames - timer[0] >= specialArgs.bulletShotTimer) {
		return true;
	}
	else {
		return false;
	}

}

void Spawner::ResetTimer() {
	timer[0] = Timer::lifeTimeFrames;
}

void Spawner::HandleEmpty() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer[CONSTANT];
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[CONSTANT];

}

void Spawner::HandleConstantSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer[CONSTANT];
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[CONSTANT];

	shouldFire[CONSTANT] = false;

	if (tempTime >= specialArgs.startTimer) {

		if (specialArgs.stopTimer <= 0)
			shouldFire[CONSTANT] = true;

		if (specialArgs.stopTimer + specialArgs.startTimer > tempTime)
			shouldFire[CONSTANT] = true;

	}

	if (shouldFire[CONSTANT] && resetTime > specialArgs.bulletShotTimer) {

		InstantiateConstantPattern();
		resettableTimer[CONSTANT] = Timer::lifeTimeFrames;

		currentBulletSpeed[CONSTANT] += specialArgs.perBulletAccel;

		if (specialArgs.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

	}
}
void Spawner::HandleBarrageSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer[BARRAGE];
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[BARRAGE];

	shouldFire[BARRAGE] = false;

	if (tempTime >= specialArgs.startTimer) {

		if (specialArgs.stopTimer <= 0)
			shouldFire[BARRAGE] = true;

		if (specialArgs.stopTimer + specialArgs.startTimer > tempTime)
			shouldFire[BARRAGE] = true;

	}

	if (barrageResettable - Timer::lifeTimeFrames > specialArgs.barrageTimer) {
		stopBarrage = false;
		barrageBullets = 0;
	}

	if (shouldFire[BARRAGE] && resetTime > specialArgs.bulletShotTimer && !stopBarrage) {

		if (specialArgs.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		InstantiateBarragePattern();
		resettableTimer[BARRAGE] = Timer::lifeTimeFrames;
		barrageBullets++;

		currentBulletSpeed[BARRAGE] += specialArgs.perBulletAccel;

		if (barrageBullets >= specialArgs.bulletsPerBarrage - 1) {
			currentBulletSpeed[BARRAGE] = specialArgs.baseProjectile.projectile.initialVel;
			stopBarrage = true;
			barrageResettable = Timer::lifeTimeFrames;
		}
	}

}
void Spawner::HandleRandomSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer[RANDOM];
	//const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[RANDOM];

	shouldFire[RANDOM] = false;

	if (tempTime >= specialArgs.startTimer) {

		if (specialArgs.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		if (specialArgs.stopTimer <= 0)
			shouldFire[RANDOM] = true;

		if (specialArgs.stopTimer + specialArgs.startTimer > tempTime)
			shouldFire[RANDOM] = true;

	}

	if (shouldFire[RANDOM]) {

		for (uint16_t i = 0; i < specialArgs.bulletPerArray; i++) {

			GetRandom();
			currentBulletSpeed[RANDOM] += specialArgs.perBulletAccel;

			InstantiateRandomPattern();
		}
	}

}

//ELIMINATE STATE
inline void Spawner::GetRandom() {
	currentBulletAngle[RANDOM] = RNG::Range(specialArgs.coneRange[0], specialArgs.coneRange[1]);
	currentBulletSpeed[RANDOM].x = RNG::Range(specialArgs.speedRange[0], specialArgs.speedRange[1]);
	currentBulletAccel[RANDOM].x = RNG::Range(specialArgs.accelRange[0], specialArgs.accelRange[1]);
}


//ELIMINATE STATE
inline void Spawner::LoopAtCone() {
	if (spawner.angle < spawner.loopingConeStart)
		spawner.angleDelta *= -1;
	if (spawner.angle > spawner.loopingConeEnd)
		spawner.angleDelta *= -1;
}

//REMOVE STATE
void Spawner::InstantiateConstantPattern() {
	const float offsetTemp = specialArgs.angleBetweenBullets * (specialArgs.bulletPerArray - 1) * 0.5f;

	currentBulletAngle[CONSTANT] = specialArgs.initialBulletAngle + specialArgs.angle - offsetTemp;
	float temp = currentBulletAngle[CONSTANT];

	for (unsigned int i = 0; i < specialArgs.bulletPerArray; i++) {

		for (unsigned int j = 0; j < specialArgs.bulletArrays; j++) {

			tempProjectile[CONSTANT].projectile =
			{
				.initialVel = currentBulletSpeed[CONSTANT],
				.acceleration = currentBulletAccel[CONSTANT],
				.angle = currentBulletAngle[CONSTANT],
			};

			Instantiate(tempProjectile[CONSTANT], transform.position, spawner);
			currentBulletAngle[CONSTANT] += specialArgs.angleBetweenArray;

		}

		currentBulletAngle[CONSTANT] = temp + specialArgs.angleBetweenBullets;
		temp = currentBulletAngle[CONSTANT];

	}

	Audio::QueueSample(0);

}

void Spawner::InstantiateBarragePattern() {
	const float offsetTemp = specialArgs.angleBetweenBullets * (specialArgs.bulletPerArray - 1) * 0.5f;

	currentBulletAngle[BARRAGE] = specialArgs.initialBulletAngle + spawner.angle - offsetTemp;
	float temp = currentBulletAngle[BARRAGE];

	for (unsigned int i = 0; i < specialArgs.bulletPerArray; i++) {

		for (unsigned int j = 0; j < specialArgs.bulletArrays; j++) {

			tempProjectile[BARRAGE].projectile =
			{
				.initialVel = currentBulletSpeed[BARRAGE],
				.acceleration = currentBulletAccel[BARRAGE],
				.angle = currentBulletAngle[BARRAGE],
			};

			Instantiate(tempProjectile[BARRAGE], transform.position, spawner);
			currentBulletAngle[BARRAGE] += specialArgs.angleBetweenArray;

		}

		currentBulletAngle[BARRAGE] = temp + specialArgs.angleBetweenBullets;
		temp = currentBulletAngle[BARRAGE];

	}

}

void Spawner::InstantiateRandomPattern() {
	currentBulletAngle[RANDOM] = specialArgs.initialBulletAngle + spawner.angle;

	for (uint8_t i = 0; i < specialArgs.bulletArrays; i++) {
		currentBulletAngle[RANDOM] += (i - 1) * specialArgs.angleBetweenArray;

		tempProjectile[RANDOM].projectile =
		{
			.initialVel = currentBulletSpeed[RANDOM],
			.acceleration = currentBulletAccel[RANDOM],
			.angle = currentBulletAngle[RANDOM],
		};

		Instantiate(tempProjectile[RANDOM], transform.position, spawner);
	}
}
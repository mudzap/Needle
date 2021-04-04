#include "Spawner.h"
#include "Player.h"
#include "util/Timer.h"
#include "game/Audio.h"

/*
TODO:	MAKE PLAYER FINDER
		UNIFY PLAYER FINDER, ONLY ONE CALL NEEDED PER INITIALIZATION/HANDLER CALL
*/

Spawner::~Spawner(){
	delete[] collideableBullets;
}

//MANUAL SPAWNER CONSTRUCTOR
void Spawner::RecreateManual(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const Projectile& projectile) {
	InitSpawner(projectile, reserveSize);
	InitCore(offset, reserveSize, spawner);

	this->projectile = projectile;
	InitializeEmptySpawner();
	handlingFunction = &Spawner::HandleEmpty;
}

//CONSTANT SPAWNER CONSTRUCTOR
void Spawner::RecreateConstant(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const ConstantArgs& constant) {
	InitSpawner(constant.baseProjectile, reserveSize);
	InitCore(offset, reserveSize, spawner);

	this->constant = constant;
	InitializeConstantSpawner();
	handlingFunction = &Spawner::HandleConstantSpawner;
}

//BARRAGE SPAWNER CONSTRUCTOR
void Spawner::RecreateBarrage(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const BarrageArgs& barrage) {
	InitSpawner(barrage.baseProjectile, reserveSize);
	InitCore(offset, reserveSize, spawner);
	
	this->barrage = barrage;
	InitializeBarrageSpawner();
	handlingFunction = &Spawner::HandleBarrageSpawner;
}

//RANDOM SPAWNER CONSTRUCTOR
void Spawner::RecreateRandom(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const RandomArgs& random) {
	InitSpawner(random.baseProjectile, reserveSize);
	InitCore(offset, reserveSize, spawner);

	this->random = random;
	InitializeRandomSpawner();
	handlingFunction = &Spawner::HandleRandomSpawner;
}

//PLAYER SPAWNER CONSTRUCTOR
void Spawner::RecreateEmpty(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner, const PlayerSpawnerArgs& playerSpawner) {
	InitSpawner(playerSpawner.baseProjectile, reserveSize);
	InitCore(offset, reserveSize, spawner);
	
	this->playerSpawner = playerSpawner;
	InitializePlayerSpawner();
	handlingFunction = &Spawner::HandleEmpty;
}

void Spawner::InitCore(const Complex offset, const unsigned int reserveSize, const SpawnerArgs& spawner) {
	this->offset = offset;
	this->spawner = spawner;

	//Why? What the fuck did I want to do with these? Do I use them in the player class?
	//Lets just adapt a new pool instead of this
	colliderSize = reserveSize / 4;
	collideables = 0;
	collideableBullets = new unsigned int[reserveSize / 4];
}

void Spawner::Handle() {
	(*this.*handlingFunction)(); //Should have went with polyomorphism, but noooo, "muh oop bad"
	//Maybe its not that bad lmao
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

	tempProjectile = constant.baseProjectile;
	currentBulletAccel = constant.baseProjectile.projectile.acceleration;
	currentBulletSpeed = constant.baseProjectile.projectile.initialVel;
	timer = Timer::lifeTimeFrames;
	resettableTimer = Timer::lifeTimeFrames;

	if (constant.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}

}

void Spawner::InitializeBarrageSpawner() {

	tempProjectile = barrage.baseProjectile;
	currentBulletAccel = barrage.baseProjectile.projectile.acceleration;
	currentBulletSpeed = barrage.baseProjectile.projectile.initialVel;
	timer = Timer::lifeTimeFrames;
	resettableTimer = Timer::lifeTimeFrames;

	if (barrage.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}


}
void Spawner::InitializeRandomSpawner() {

	tempProjectile = random.baseProjectile;
	GetRandom();
	timer = Timer::lifeTimeFrames;
	resettableTimer = Timer::lifeTimeFrames;

}

void Spawner::InitializeEmptySpawner() {

	tempProjectile = projectile;
	timer = Timer::lifeTimeFrames;
	resettableTimer = Timer::lifeTimeFrames;

	/*
	if (projectile.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}
	*/
}

void Spawner::InitializePlayerSpawner() {

	tempProjectile = projectile;
	timer = Timer::lifeTimeFrames;
	resettableTimer = Timer::lifeTimeFrames;

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

	if (Timer::lifeTimeFrames - timer >= playerSpawner.bulletShotTimer) {
		return true;
	}
	else {
		return false;
	}

}

void Spawner::ResetTimer() {
	timer = Timer::lifeTimeFrames;
}

void Spawner::HandleEmpty() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer;
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer;

}

void Spawner::HandleConstantSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer;
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer;

	shouldFire = false;

	if (tempTime >= barrage.startTimer && !asleep) {

		if (barrage.stopTimer <= 0)
			shouldFire = true;

		if (barrage.stopTimer + barrage.startTimer > tempTime)
			shouldFire = true;

	}

	if (shouldFire && resetTime > constant.bulletShotTimer) {

		InstantiateConstantPattern();
		resettableTimer = Timer::lifeTimeFrames;

		currentBulletSpeed += constant.perBulletAccel;

		if (constant.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

	}
}
void Spawner::HandleBarrageSpawner() {

	spawner.angle += spawner.angleDelta;
	spawner.angleDelta += spawner.angleAccel;

	if (spawner.loopingConeEnd != spawner.loopingConeStart)
		LoopAtCone();

	const unsigned int tempTime = Timer::lifeTimeFrames - timer;
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer;

	shouldFire = false;

	if (tempTime >= barrage.startTimer && !asleep) {

		if (barrage.stopTimer <= 0)
			shouldFire = true;

		if (barrage.stopTimer + barrage.startTimer > tempTime)
			shouldFire = true;

	}

	if (barrageResettable - Timer::lifeTimeFrames > barrage.barrageTimer) {
		stopBarrage = false;
		barrageBullets = 0;
	}

	if (shouldFire && resetTime > barrage.bulletShotTimer && !stopBarrage) {

		if (barrage.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		InstantiateBarragePattern();
		resettableTimer = Timer::lifeTimeFrames;
		barrageBullets++;

		currentBulletSpeed += barrage.perBulletAccel;

		if (barrageBullets >= barrage.bulletsPerBarrage - 1) {
			currentBulletSpeed = barrage.baseProjectile.projectile.initialVel;
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

	const unsigned int tempTime = Timer::lifeTimeFrames - timer;
	//const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[RANDOM];

	shouldFire = false;

	if (tempTime >= random.startTimer && !asleep) {

		if (random.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		if (random.stopTimer <= 0)
			shouldFire = true;

		if (random.stopTimer + random.startTimer > tempTime)
			shouldFire = true;

	}

	if (shouldFire) {

		for (uint16_t i = 0; i < barrage.bulletPerArray; i++) {

			GetRandom();
			currentBulletSpeed += random.perBulletAccel;

			InstantiateRandomPattern();
		}
	}

}

//ELIMINATE STATE
inline void Spawner::GetRandom() {
	currentBulletAngle = RNG::Range(random.coneRange[0], random.coneRange[1]);
	currentBulletSpeed.x = RNG::Range(random.speedRange[0], random.speedRange[1]);
	currentBulletAccel.x = RNG::Range(random.accelRange[0], random.accelRange[1]);
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
	const float offsetTemp = constant.angleBetweenBullets * (constant.bulletPerArray - 1) * 0.5f;

	currentBulletAngle = constant.initialBulletAngle + spawner.angle - offsetTemp;
	float temp = currentBulletAngle;

	for (unsigned int i = 0; i < constant.bulletPerArray; i++) {

		for (unsigned int j = 0; j < constant.bulletArrays; j++) {

			tempProjectile.projectile =
			{
				.initialVel = currentBulletSpeed,
				.acceleration = currentBulletAccel,
				.angle = currentBulletAngle,
			};

			Instantiate(tempProjectile, transform.position, spawner);
			currentBulletAngle += constant.angleBetweenArray;

		}

		currentBulletAngle = temp + constant.angleBetweenBullets;
		temp = currentBulletAngle;

	}

	Audio::QueueSample(0);

}

void Spawner::InstantiateBarragePattern() {
	const float offsetTemp = barrage.angleBetweenBullets * (barrage.bulletPerArray - 1) * 0.5f;

	currentBulletAngle = barrage.initialBulletAngle + spawner.angle - offsetTemp;
	float temp = currentBulletAngle;

	for (unsigned int i = 0; i < barrage.bulletPerArray; i++) {

		for (unsigned int j = 0; j < barrage.bulletArrays; j++) {

			tempProjectile.projectile =
			{
				.initialVel = currentBulletSpeed,
				.acceleration = currentBulletAccel,
				.angle = currentBulletAngle,
			};

			Instantiate(tempProjectile, transform.position, spawner);
			currentBulletAngle += barrage.angleBetweenArray;

		}

		currentBulletAngle = temp + barrage.angleBetweenBullets;
		temp = currentBulletAngle;

	}

}

void Spawner::InstantiateRandomPattern() {
	currentBulletAngle = random.initialBulletAngle + spawner.angle;

	for (uint8_t i = 0; i < random.bulletArrays; i++) {
		currentBulletAngle += (i - 1) * random.angleBetweenArray;

		tempProjectile.projectile =
		{
			.initialVel = currentBulletSpeed,
			.acceleration = currentBulletAccel,
			.angle = currentBulletAngle,
		};

		Instantiate(tempProjectile, transform.position, spawner);
	}
}

void Spawner::Sleep() {
	asleep = true;
}

void Spawner::WakeUp() {
	asleep = false;
}
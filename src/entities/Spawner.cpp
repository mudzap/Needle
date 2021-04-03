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
	projectile(projectile)
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
	constant(constant),
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
	barrage(barrage),
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
	random(random),
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
	playerSpawner(playerSpawner),
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

void Spawner::Handle() {
	(*this.*handlingFunction)(); //Should have went with polyomorphism, but noooo, "muh oop bad"
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

	tempProjectile[CONSTANT] = constant.baseProjectile;
	currentBulletAccel[CONSTANT] = constant.baseProjectile.projectile.acceleration;
	currentBulletSpeed[CONSTANT] = constant.baseProjectile.projectile.initialVel;
	timer[CONSTANT] = Timer::lifeTimeFrames;
	resettableTimer[CONSTANT] = Timer::lifeTimeFrames;

	if (constant.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}

}

void Spawner::InitializeBarrageSpawner() {

	tempProjectile[BARRAGE] = barrage.baseProjectile;
	currentBulletAccel[BARRAGE] = barrage.baseProjectile.projectile.acceleration;
	currentBulletSpeed[BARRAGE] = barrage.baseProjectile.projectile.initialVel;
	timer[BARRAGE] = Timer::lifeTimeFrames;
	resettableTimer[BARRAGE] = Timer::lifeTimeFrames;

	if (barrage.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}


}
void Spawner::InitializeRandomSpawner() {

	tempProjectile[RANDOM] = random.baseProjectile;
	GetRandom();
	timer[RANDOM] = Timer::lifeTimeFrames;
	resettableTimer[RANDOM] = Timer::lifeTimeFrames;

}

void Spawner::InitializeEmptySpawner() {

	tempProjectile[0] = projectile;
	timer[0] = Timer::lifeTimeFrames;
	resettableTimer[0] = Timer::lifeTimeFrames;

	/*
	if (projectile.aimed) {
		//AIM AT PLAYER, GET ANGLE
	}
	*/
}

void Spawner::InitializePlayerSpawner() {

	tempProjectile[0] = projectile;
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

	if (Timer::lifeTimeFrames - timer[0] >= playerSpawner.bulletShotTimer) {
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

	if (tempTime >= barrage.startTimer) {

		if (barrage.stopTimer <= 0)
			shouldFire[CONSTANT] = true;

		if (barrage.stopTimer + barrage.startTimer > tempTime)
			shouldFire[CONSTANT] = true;

	}

	if (shouldFire[CONSTANT] && resetTime > constant.bulletShotTimer) {

		InstantiateConstantPattern();
		resettableTimer[CONSTANT] = Timer::lifeTimeFrames;

		currentBulletSpeed[CONSTANT] += constant.perBulletAccel;

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

	const unsigned int tempTime = Timer::lifeTimeFrames - timer[BARRAGE];
	const unsigned int resetTime = Timer::lifeTimeFrames - resettableTimer[BARRAGE];

	shouldFire[BARRAGE] = false;

	if (tempTime >= barrage.startTimer) {

		if (barrage.stopTimer <= 0)
			shouldFire[BARRAGE] = true;

		if (barrage.stopTimer + barrage.startTimer > tempTime)
			shouldFire[BARRAGE] = true;

	}

	if (barrageResettable - Timer::lifeTimeFrames > barrage.barrageTimer) {
		stopBarrage = false;
		barrageBullets = 0;
	}

	if (shouldFire[BARRAGE] && resetTime > barrage.bulletShotTimer && !stopBarrage) {

		if (barrage.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		InstantiateBarragePattern();
		resettableTimer[BARRAGE] = Timer::lifeTimeFrames;
		barrageBullets++;

		currentBulletSpeed[BARRAGE] += barrage.perBulletAccel;

		if (barrageBullets >= barrage.bulletsPerBarrage - 1) {
			currentBulletSpeed[BARRAGE] = barrage.baseProjectile.projectile.initialVel;
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

	if (tempTime >= random.startTimer) {

		if (random.aimed) {
			//AIM AT PLAYER, GET ANGLE
		}

		if (random.stopTimer <= 0)
			shouldFire[RANDOM] = true;

		if (random.stopTimer + random.startTimer > tempTime)
			shouldFire[RANDOM] = true;

	}

	if (shouldFire[RANDOM]) {

		for (uint16_t i = 0; i < barrage.bulletPerArray; i++) {

			GetRandom();
			currentBulletSpeed[RANDOM] += random.perBulletAccel;

			InstantiateRandomPattern();
		}
	}

}

//ELIMINATE STATE
inline void Spawner::GetRandom() {
	currentBulletAngle[RANDOM] = RNG::Range(random.coneRange[0], random.coneRange[1]);
	currentBulletSpeed[RANDOM].x = RNG::Range(random.speedRange[0], random.speedRange[1]);
	currentBulletAccel[RANDOM].x = RNG::Range(random.accelRange[0], random.accelRange[1]);
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

	currentBulletAngle[CONSTANT] = constant.initialBulletAngle + spawner.angle - offsetTemp;
	float temp = currentBulletAngle[CONSTANT];

	for (unsigned int i = 0; i < constant.bulletPerArray; i++) {

		for (unsigned int j = 0; j < constant.bulletArrays; j++) {

			tempProjectile[CONSTANT].projectile =
			{
				.initialVel = currentBulletSpeed[CONSTANT],
				.acceleration = currentBulletAccel[CONSTANT],
				.angle = currentBulletAngle[CONSTANT],
			};

			Instantiate(tempProjectile[CONSTANT], transform.position, spawner);
			currentBulletAngle[CONSTANT] += constant.angleBetweenArray;

		}

		currentBulletAngle[CONSTANT] = temp + constant.angleBetweenBullets;
		temp = currentBulletAngle[CONSTANT];

	}

	Audio::QueueSample(0);

}

void Spawner::InstantiateBarragePattern() {
	const float offsetTemp = barrage.angleBetweenBullets * (barrage.bulletPerArray - 1) * 0.5f;

	currentBulletAngle[BARRAGE] = barrage.initialBulletAngle + spawner.angle - offsetTemp;
	float temp = currentBulletAngle[BARRAGE];

	for (unsigned int i = 0; i < barrage.bulletPerArray; i++) {

		for (unsigned int j = 0; j < barrage.bulletArrays; j++) {

			tempProjectile[BARRAGE].projectile =
			{
				.initialVel = currentBulletSpeed[BARRAGE],
				.acceleration = currentBulletAccel[BARRAGE],
				.angle = currentBulletAngle[BARRAGE],
			};

			Instantiate(tempProjectile[BARRAGE], transform.position, spawner);
			currentBulletAngle[BARRAGE] += barrage.angleBetweenArray;

		}

		currentBulletAngle[BARRAGE] = temp + barrage.angleBetweenBullets;
		temp = currentBulletAngle[BARRAGE];

	}

}

void Spawner::InstantiateRandomPattern() {
	currentBulletAngle[RANDOM] = random.initialBulletAngle + spawner.angle;

	for (uint8_t i = 0; i < random.bulletArrays; i++) {
		currentBulletAngle[RANDOM] += (i - 1) * random.angleBetweenArray;

		tempProjectile[RANDOM].projectile =
		{
			.initialVel = currentBulletSpeed[RANDOM],
			.acceleration = currentBulletAccel[RANDOM],
			.angle = currentBulletAngle[RANDOM],
		};

		Instantiate(tempProjectile[RANDOM], transform.position, spawner);
	}
}
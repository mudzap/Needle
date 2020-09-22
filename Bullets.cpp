#include "Bullets.h"
#include "Timer.h"


Bullets::Bullets() {
}

Bullets::Bullets(const Projectile& projectile, const int reserveSize)
	: indexPointer(0), trashSize(0), reserveSize(reserveSize), currentSize(0) {
	InitSpawner(projectile);
}

Bullets::~Bullets() {

}

void Bullets::InitSpawner(const Projectile& projectile) {

	InitTransformBuffers(reserveSize);

	vertices.resize(1);
	tvertices.resize(reserveSize);

	Complex aimSpot = Complex::zero;

	shouldGraze = projectile.projectile.flags & PFLAG_GRAZEABLE;
	quad = projectile.common.quad;
	squaredHitboxRadius = projectile.common.hitbox.squaredHitRadius;
	rotates = projectile.common.flags & PFLAG_ROTATES_TOWARDS;


	if (projectile.projectile.acceleration != Complex::zero) {
		initialAccel = projectile.projectile.acceleration;
		bulletMethods.emplace_back(&Bullets::BatchAccelerated);
	}

	if (projectile.projectile.decayRate != 1.f) {
		exponent = projectile.projectile.decayRate;
		bulletMethods.emplace_back(&Bullets::BatchExponential);
	}

	if (projectile.projectile.angleDelta != 0.f) {
		rotationDeltaMat = Complex::FromAngle(projectile.projectile.angleDelta);
		bulletMethods.emplace_back(&Bullets::BatchDeflected);
	}

	if (projectile.projectile.sineFrequency > 0.f) {
		sineAmplitude = projectile.projectile.sineAmplitude;
		sineFreqConstant = 1000.f / projectile.projectile.sineFrequency;
		bulletMethods.emplace_back(&Bullets::BatchSinusoidal);
	}

	if (projectile.projectile.stopandgoFrequency > 0.f) {
		desiredVelocity = projectile.projectile.initialVel; //////////////////////////////////FIX!!!!!!!!!!!!!!!!!!!!!!
		stopAndGoFreqConstant = 1000.f / projectile.projectile.stopandgoFrequency;
		bulletMethods.emplace_back(&Bullets::BatchStopAndGo);
	}

	if (projectile.projectile.zigzagFrequency > 0.f) {
		zigzagFreqConstant = 1000.f / projectile.projectile.zigzagFrequency;
		zigzagRotationMat = Complex::FromAngle(projectile.projectile.zigzagAmplitude);
		initialRotationMat = Complex::FromAngle(-projectile.projectile.zigzagAmplitude * 0.5f);
		bulletMethods.emplace_back(&Bullets::BatchZigzagging);
	}

	if (projectile.projectile.gravityConstant != Complex::zero) {
		gravity = projectile.projectile.gravityConstant;
		bulletMethods.emplace_back(&Bullets::BatchGravity);
	}

	InitMesh(quad);

	position.resize(reserveSize);
	velocity.resize(reserveSize);
	acceleration.resize(reserveSize);
	grazeable.resize(reserveSize);
	cartesianVelocity.resize(reserveSize);
	visualRotation.resize(reserveSize);
	rotationMat.resize(reserveSize);
	sineTimer.resize(reserveSize);
	stopAndGoTimer.resize(reserveSize);
	zigzagTimer.resize(reserveSize);
	bulletMethods.reserve(7);

}

void Bullets::ClearBullet(const int ID) {
	//SPAWN PARTICLE
	DeleteBullet(ID);
}

void Bullets::BatchClearBullets() {

	for (unsigned int i = 0; i < currentSize; i++) {
		//SPAWN PARTICLES AT POS
	}

	BatchDeleteBullets();
}

void Bullets::ScissorTest() {

	const float widthCheck = 384.f + quad.w * 0.5f;
	const float heigthCheck = 448.f + quad.h * 0.5f;

	for (unsigned int i = 0; i < currentSize - trashSize; i++) {
		bool destroy = false;
		if (position[i].x > widthCheck || position[i].x < -widthCheck)
			destroy = true;
		if (position[i].y > heigthCheck || position[i].y < -heigthCheck)
			destroy = true;

		if (destroy) {
			BatchSwap(i, currentSize - trashSize - 1);
			trashSize++;
		}
			 
	}
}

void Bullets::BatchSwap(const int i, const int j) {

	std::iter_swap(position.begin() + i, position.begin() + j);
	std::iter_swap(velocity.begin() + i, velocity.begin() + j);
	std::iter_swap(acceleration.begin() + i, acceleration.begin() + j);

	std::iter_swap(grazeable.begin() + i, grazeable.begin() + j);
	std::iter_swap(cartesianVelocity.begin() + i, cartesianVelocity.begin() + j);

	std::iter_swap(visualRotation.begin() + i, visualRotation.begin() + j);

	std::iter_swap(rotationMat.begin() + i, rotationMat.begin() + j);

	std::iter_swap(sineTimer.begin() + i, sineTimer.begin() + j);
	std::iter_swap(stopAndGoTimer.begin() + i, stopAndGoTimer.begin() + j);
	std::iter_swap(zigzagTimer.begin() + i, zigzagTimer.begin() + j);

}

void Bullets::Instantiate(const Projectile& projectile, const Complex& bulletPosition, const SpawnerArgs& spawner) {

	if (trashSize > 0) {
		tempPointer = indexPointer;
		indexPointer = currentSize - trashSize;
	}

	sineTimer[indexPointer] = Timer::lifeTimeFrames;
	stopAndGoTimer[indexPointer] = Timer::lifeTimeFrames;
	zigzagTimer[indexPointer] = Timer::lifeTimeFrames;

	position[indexPointer] = bulletPosition;
	velocity[indexPointer] = projectile.projectile.initialVel; //FIX!
	// INCLUDE ZIGZAG ROTATION MAT
	rotationMat[indexPointer] = Complex::FromAngle(projectile.projectile.angle); //FIX!, WHY PROJECTILE?

	visualRotation[indexPointer] = Complex::right;

	cartesianVelocity[indexPointer] = Complex::zero;
	grazeable[indexPointer] = shouldGraze;
	

	InitBullet(indexPointer);

	if (trashSize > 0) {
		trashSize--;
		indexPointer = tempPointer;
	}
	else {
		indexPointer = (indexPointer + 1) % reserveSize;
		if (currentSize < reserveSize)
			currentSize++;
	}

}


void Bullets::InitBullet(const int ID) {

	//SPAWN SPAWN PARTICLES


	/*if ( AIMED ) {
		//SET AIM
	}*/

}

void Bullets::DeleteBullet(const int ID) {

	position.erase(position.begin() + ID);
	velocity.erase(velocity.begin() + ID);
	acceleration.erase(acceleration.begin() + ID);

	grazeable.erase(grazeable.begin() + ID);
	
	cartesianVelocity.erase(cartesianVelocity.begin() + ID);

	visualRotation.erase(visualRotation.begin() + ID);

	rotationMat.erase(rotationMat.begin() + ID);

	sineTimer.erase(sineTimer.begin() + ID);
	stopAndGoTimer.erase(stopAndGoTimer.begin() + ID);
	zigzagTimer.erase(zigzagTimer.begin() + ID);

}

void Bullets::BatchDeleteBullets() {

	position.clear();
	velocity.clear();
	acceleration.clear();

	//exponent.clear();

	//sineAmplitude.clear();
	//desiredVelocity.clear();
	//zigzagAmplitude.clear();

	//squaredHitboxRadius.clear();
	grazeable.clear();
	//rotates.clear();

	//gravity.clear();
	cartesianVelocity.clear();

	visualRotation.clear();

	//quad.clear();

	rotationMat.clear();
	//rotationDeltaMat.clear();
	//zigzagRotationMat.clear();

	sineTimer.clear();
	stopAndGoTimer.clear();
	zigzagTimer.clear();

	//sineFreqConstant.clear();
	//stopAndGoFreqConstant.clear();
	//zigzagFreqConstant.clear();

	//bulletMethods.clear();

	//OR OTHERWISE, CURRENTSIZE = 0, TRASHSIZE = 0

}

void Bullets::ProcessBullet(const int ID) {

	Transform::Translate(position, ID, velocity[ID], rotationMat[ID]);
	Transform::TranslateCartesian(position, ID, cartesianVelocity[ID]);

	Accelerated(ID);
	Exponential(ID);
	Deflected(ID);
	Gravity(ID);

	if (rotates) {
		Transform::UpdateAngle(visualRotation, ID, rotationMat, velocity);
	}
}

void Bullets::BatchProcessBullets() {

	for (unsigned int i = 0; i < currentSize; i++) {
		position[i] += velocity[i] * rotationMat[i];
	}

	for (int j = 0; j < bulletMethods.size(); j++) {
		(*this.*bulletMethods[j])();
	}

	if (rotates)
		Transform::BatchUpdateAngle(visualRotation, rotationMat, velocity, currentSize - trashSize);

}

void Bullets::FillVertices() {

	for (unsigned int i = 0; i < currentSize - trashSize; i++) {
		Complex rotation = visualRotation[i];
		glm::mat4 rotationMatrix = { rotation.x, rotation.y, 0.f, 0.f, -rotation.y, rotation.x, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
		tvertices[i].translation = position[i];
		tvertices[i].transform = rotationMatrix;
	}

}

//		BULLET BEHAVIOR		//

inline void Bullets::Accelerated(const int ID) {
	Transform::Accelerate(velocity, ID, acceleration[ID]);
}
inline void Bullets::Exponential(const int ID) {
	acceleration[ID] *= exponent;
}
inline void Bullets::StopAndGo(const int ID) {
	const unsigned int timeTemp = (Timer::lifeTimeFrames - stopAndGoTimer[ID]);
	const float temp = timeTemp * 3.333333333333333f / stopAndGoFreqConstant; //This doesn't make sense but the pattern feels better like this (1/60 * 2 ?)
	const float temp2 = temp * temp;

	if (timeTemp < stopAndGoFreqConstant * 0.4f)
		velocity[ID] = desiredVelocity * (temp2 + 2 * temp + 1);
	if (timeTemp > stopAndGoFreqConstant * 0.5f && timeTemp < stopAndGoFreqConstant * 0.9f)
		velocity[ID] = desiredVelocity * (-temp2 + 4 * temp - 3);

	if (timeTemp >= stopAndGoFreqConstant)
		stopAndGoTimer[ID] = Timer::lifeTimeFrames;
}
inline void Bullets::Sinusoidal(const int ID) {
	acceleration[ID].y = sineAmplitude * FT::cos((Timer::lifeTimeFrames - sineTimer[ID]) * 1.f / sineFreqConstant);
}
inline void Bullets::Zigzagging(const int ID) {
	if (Timer::lifeTimeFrames - zigzagTimer[ID] > zigzagFreqConstant)
	{
		Transform::Rotate(rotationMat, ID, zigzagRotationMat);
		zigzagAmplitude *= -1;
		zigzagRotationMat *= -1;
		zigzagTimer[ID] = Timer::lifeTimeFrames;
	}
}
inline void Bullets::Deflected(const int ID) {
	Transform::Rotate(rotationMat, ID, rotationDeltaMat);
}
inline void Bullets::Gravity(const int ID) {
	position[ID] += cartesianVelocity[ID];
	cartesianVelocity[ID] += gravity;
}


//		 BATCH BULLET BEHAVIOR		//


inline void Bullets::BatchAccelerated() {
	for (unsigned int i = 0; i < currentSize; i++)
		Accelerated(i);
}
inline void Bullets::BatchExponential() {
	for (unsigned int i = 0; i < currentSize; i++)
		Exponential(i);
}
inline void Bullets::BatchStopAndGo() {
	for (unsigned int i = 0; i < currentSize; i++)
		StopAndGo(i);
}
inline void Bullets::BatchSinusoidal() {
	for (unsigned int i = 0; i < currentSize; i++)
		Sinusoidal(i);
}
inline void Bullets::BatchZigzagging() {
	for (unsigned int i = 0; i < currentSize; i++)
		Zigzagging(i);
}
inline void Bullets::BatchDeflected() {
	Transform::BatchRotate(rotationMat, rotationDeltaMat, currentSize);
}
inline void Bullets::BatchGravity() {
	for (unsigned int i = 0; i < currentSize; i++)
		Gravity(i);
}
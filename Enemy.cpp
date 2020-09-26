#include "Enemy.h"

Enemy::Enemy() {
	enemySpawners.reserve(8);
}

Enemy::Enemy(const TransformArgs& entity, const AnimationArgs& animArgs)
	: Transform(entity), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Hitbox(animArgs.hitbox) {

	enemySpawners.reserve(8);

	SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

}

Enemy::Enemy(const Complex position, const AnimationArgs& animArgs)
	: Transform(defaultTransform), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Hitbox(animArgs.hitbox) {

	enemySpawners.reserve(8);

	SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

	transform.position = position;

}

Enemy::~Enemy() {
}

void Enemy::SetPosition(Complex position) {
	transform.position = position;
	targetPosition = position;
}

void Enemy::Destroy() {
	transform.velocity = Complex::zero;
	transform.acceleration = Complex::zero;
	state = DEAD;
}

void Enemy::MoveTowards1(Complex position, float speed, float accel) {
	direction = Complex::Normal(position);
	transform.velocity = direction * speed;
	transform.acceleration = direction * accel;
	state = MOVE_TOWARDS;
}

void Enemy::MoveTowards2(float angle, Complex speed, Complex accel) {
	direction = Complex::FromAngle(angle);
	transform.velocity = direction * speed;
	transform.acceleration = direction * accel;
	state = MOVE_TOWARDS;
}

void Enemy::SpringTowards1(Complex position, float accelPerUnit) {
	targetPosition = position + transform.position;
	this->accelPerUnit = accelPerUnit;
	state = SPRING_TOWARDS;
}

void Enemy::SpringTowards2(float angle, float distance, float accelPerUnit) {
	targetPosition = Complex::FromAngle(angle) * distance + transform.position;
	this->accelPerUnit = accelPerUnit;
	state = SPRING_TOWARDS;
}

void Enemy::SlowTowards1(Complex position, float velocityPerUnit) {
	targetPosition = transform.position + position;
	this->velocityPerUnit = velocityPerUnit;
	state = SLOW_TOWARDS;
}

void Enemy::SlowTowards2(float angle, float distance, float velocityPerUnit) {
	targetPosition = Complex::FromAngle(angle) * distance + transform.position;
	this->velocityPerUnit = velocityPerUnit;
	state = SLOW_TOWARDS;
}

void Enemy::Stop1() {
	transform.velocity = Complex::zero;
	transform.acceleration = Complex::zero;
	state = FULL_STOP;
}

void Enemy::Stop2(float brakeFactor) {
	brakeConstant = brakeFactor;
	transform.acceleration = Complex::zero;
	state = STOP;
}

void Enemy::Drift1() {
	exponent = 1;
	state = DRIFT;
}

void Enemy::Drift2(float exponent) {
	this->exponent = exponent;
	state = DRIFT;
}

void Enemy::CircleAround(Complex position) {
	targetPosition = position + transform.position;
	accelPerUnit = Complex::SquaredAbs(transform.velocity) / Complex::SquaredAbs(targetPosition - transform.position);
	state = SPRING_TOWARDS;
}

void Enemy::AddSpawner(
	const Spawner& spawner
) {
	enemySpawners.emplace_back(spawner);
}

void Enemy::AddSpawnerManual(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const Projectile& projectile
) {
	enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, projectile));
}

void Enemy::AddSpawnerConstant(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const ConstantArgs& constant) {
	enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, constant));
}
void Enemy::AddSpawnerBarrage(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const BarrageArgs& barrage) {
	enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, barrage));
}
void Enemy::AddSpawnerRandom(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const RandomArgs& random) {
	enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, random));
}

void Enemy::HandleEnemy() {

	Transform::TranslateCartesian(transform.velocity);

	switch (state) {

		case DEAD: {
			break;
		}

		case STOP: {
			transform.velocity *= 1.f / (brakeConstant + 1.f);
			break;
		}

		case FULL_STOP: {
			break;
		}

		case MOVE_TOWARDS: {
			transform.velocity += transform.acceleration;
			break;
		}

		case SPRING_TOWARDS: {
			transform.velocity += (targetPosition - transform.position) * accelPerUnit;
			break;
		}

		case SLOW_TOWARDS: {
			transform.velocity = (targetPosition - transform.position) * velocityPerUnit;
			break;
		}

		case DRIFT: {
			transform.velocity *= exponent;
			break;
		}

	}

	ipair state = { 1, 0 };

	if (transform.velocity.x > 1.f)
		state.x = 2;
	if (transform.velocity.x < -1.f)
		state.x = 0;

	TransitionToAnimationState(state);

	HandleAnimation();

	for (int i = 0; i < enemySpawners.size(); i++) {
		enemySpawners[i].transform.position = transform.position + enemySpawners[i].offset;
		enemySpawners[i].HandleSpawner();
		enemySpawners[i].BatchProcessBullets();
		enemySpawners[i].ScissorTest();
		enemySpawners[i].FillVertices();
	}

#ifdef _MAT4_INSTANCING_
	tvertices[0] = { transform.position, glm::mat4(1.f) };
#else
	tvertices[0] = { transform.position, {1.0, 0.0} };
#endif

}

void Enemy::DrawBullets() {

	for (int i = 0; i < enemySpawners.size(); i++) {
		enemySpawners[i].Draw(enemySpawners[i].tvertices[0], enemySpawners[i].currentSize - enemySpawners[i].trashSize);
	}

}

void Enemy::Draw() {
	TVertex transformV;
	transformV.translation = transform.position;
	Animation::Draw(&transformV);
}
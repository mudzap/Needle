#include "Enemy.h"

Enemy::Enemy() {

	dead = true;

}

Enemy::Enemy(const TransformArgs& entity, const AnimationArgs& animArgs, const unsigned int health)
	: Transform(entity), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Hitbox(animArgs.hitbox), health(health) {

	SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

}

Enemy::Enemy(const Complex position, const AnimationArgs& animArgs, const unsigned int health)
	: Transform(defaultTransform), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Hitbox(animArgs.hitbox), health(health) {

	SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

	transform.position = position;

}

Enemy::~Enemy() {
}

void Enemy::RecreateTransform(const TransformArgs& entity, const AnimationArgs& animArgs, const unsigned int health) {

	this->health = health;
	transform = entity;
	drawQuad = animArgs.drawQuad;
	dimensions = animArgs.states;
	frameTime = animArgs.frameTime;
	hitbox = animArgs.hitbox;
	dead = false;
	state = STOP;

	//REMOVE
	InitTransformBuffers();

	//SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

}

void Enemy::RecreatePosition(const Complex position, const AnimationArgs& animArgs, const unsigned int health) {

	transform.position = position;
	drawQuad = animArgs.drawQuad;
	dimensions = animArgs.states;
	frameTime = animArgs.frameTime;
	hitbox = animArgs.hitbox;
	this->health = health;
	dead = false;
	state = STOP;

	//REMOVE
	InitTransformBuffers();

	//SetStateSprites(3);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

}

void Enemy::SetPosition(Complex position) {
	transform.position = position;
	targetPosition = position;
}

void Enemy::Destroy() {
	transform.velocity = Complex::zero;
	transform.acceleration = Complex::zero;
	dead = true;
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
	//enemySpawners.emplace_back(spawner);
}

void Enemy::AddSpawnerManual(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const Projectile& projectile
) {
	//enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, projectile));
}

void Enemy::AddSpawnerConstant(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const ConstantArgs& constant) {
	//enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, constant));
}
void Enemy::AddSpawnerBarrage(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const BarrageArgs& barrage) {
	//enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, barrage));
}
void Enemy::AddSpawnerRandom(
	const Complex offset,
	const unsigned int reserveSize,
	const SpawnerArgs& spawner,
	const RandomArgs& random) {
	//enemySpawners.emplace_back(Spawner(offset, reserveSize, spawner, random));
}


void Enemy::Handle(Player& player) {

	if (dead) {

		MakeInactive();

	} else {

		Transform::TranslateCartesian(transform.velocity);

		switch (state) {

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

		std::vector<Spawner>& playerSpawner = player.GetPlayerSpawners();

		for (int i = 0; i < playerSpawner.size(); i++) {
			CheckCollideable(playerSpawner[i]);
		}

#ifdef _MAT4_INSTANCING_
		tvertices[0] = { transform.position, glm::mat4(1.f) };
#else
		tvertices[0] = { transform.position, {1.0, 0.0} };
#endif

		/*		DEPRECATED, MOVING TOWARDS NON-DEPENDANT SPAWNERS
		for (int i = 0; i < enemySpawners.size(); i++) {
			enemySpawners[i].transform.position = transform.position + enemySpawners[i].offset;
			enemySpawners[i].Handle();
		}
		*/

		ScissorTest();

	}

	/*		DITTO
	for (int i = 0; i < enemySpawners.size(); i++) {
		enemySpawners[i].BatchProcessBullets();
		enemySpawners[i].ScissorTest();
		enemySpawners[i].FillVertices();
	}
	*/

}

void Enemy::ScissorTest() {

	//1.5 IS MEANT FOR EXTRA SPACE, IT MEANS THAT AN ENEMY MUST BE OFFSET 2 TIMES ITS QUAD LENGTH (DRAWN) TO BE CULLED
	//ITS 2 DUE TO THE SCISSOR TEST BEING BASED ON POSITION, RATHER THAN EDGES
	const float widthCheck = 384.f + drawQuad.w * 1.5f;
	const float heigthCheck = 448.f + drawQuad.h * 1.5f;
	

	if (transform.position.x > widthCheck || transform.position.x < -widthCheck ||
		transform.position.y > heigthCheck || transform.position.y < -heigthCheck) {
		dead = true;
	}

	//BatchSwap();
	//BatchSwap(i, currentSize - trashSize - 1);
	//trashSize++;

}

void Enemy::DrawBullets() {

/*
	for (int i = 0; i < enemySpawners.size(); i++) {
		enemySpawners[i].Draw(enemySpawners[i].tvertices[0], enemySpawners[i].currentSize - enemySpawners[i].trashSize);
	}
*/

}

void Enemy::Draw() {

	if (!dead) {
		TVertex transformV;
		transformV.translation = transform.position;
		Animation::Draw(&transformV);
	}

}

void Enemy::CheckCollideable(Spawner& spawner) {

	const unsigned int size = spawner.currentSize - spawner.trashSize;

	for (unsigned int i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHurtRadius, spawner.squaredHitboxRadius, transform.position, spawner.position[i])) {

			spawner.BatchSwap(i);

			health--;

			if (health <= 0)
				dead = true;

			return;

		}

	}
}

/*
void Enemy::MakeInactive() {

	std::unique_ptr<KeyContainer<Enemy>> thisEnemy = std::make_unique<KeyContainer<Enemy>>(KeyContainer<Enemy>{this, ID});

	if(parentPool == NULL || ID > 0 ) {
		SHMY_LOGE("This enemy doesn't belong to a pool, ID: %d\n", ID);
	} else {
		parentPool->FreeHandle(thisEnemy.get());
	}

}
*/
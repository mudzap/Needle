#include "Player.h"
	
Player::Player(const PlayerArgs& player, const TransformArgs& entity, const AnimationArgs& animArgs)
	: playerArgs(player), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Transform(entity), Hitbox(animArgs.hitbox) {
	controllable = true;
	conversation = false;
	collideableBullets.reserve(512);

	//MANUAL SPAWNER CONSTRUCTOR
	unsigned int bullets = 100;
	for (int i = 0; i < spawners.size(); i++) {
		spawners[i] = Spawner(spawnerOffsets[i], bullets, spawnerArgs[i], playerSpawnerArgs[i]);
	}
	
	SetStateSprites(1);

	SetStateSprites(8,
		{ 3, 0 }
	);

	SetStateSprites(6, 2,
		0, 0,	6, 0
	);

	CreateMapAndMesh(1024.f, animArgs.spriteSheetQuad);

}

Player::~Player() {
}

void Player::HandlePlayerMovement() {

	transform.position += transform.velocity;

	ipair state = { 3, 0 };

	if (transform.velocity.x > 1.f)
		state.x = 6;
	if (transform.velocity.x < -1.f)
		state.x = 0;

	TransitionToAnimationState(state);

	HandleAnimation();

	BindPlayerToScreen();

};

void Player::HandlePlayerCollision(Enemy* enemy) {

	for (int i = 0; i < enemy->enemySpawners.size(); i++)
		CheckCollisionRoutine(enemy->enemySpawners[i]);

};

void Player::HandleSpawners() {

	for (int i = 0; i < spawners.size(); i++) {

		if (spawners[i].HandlePlayerSpawner() && shooting) {
			spawners[i].Instantiate(playerSpawnerArgs[i].baseProjectile, transform.position + spawners[i].offset, spawnerArgs[i]);
			spawners[i].ResetTimer();
		}

		spawners[i].BatchProcessBullets();
		spawners[i].ScissorTest();
		spawners[i].FillVertices();

	}

}

void Player::CheckGrazeable(Spawner& spawner) {

#ifdef _USE_SSE2_
	unsigned int size = spawner.currentSize - spawner.trashSize;
	size = size / 4 + size % 4;
#else
	const unsigned int size = spawner.currentSize - spawner.trashSize;
#endif

	collideableBullets.clear();

	for (unsigned int i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHurtRadius, spawner.squaredHitboxRadius, transform.position, spawner.position[i])) {
			
			collideableBullets.emplace_back(i);
			if (spawner.grazeable[i] & PFLAG_GRAZEABLE) {
				playerArgs.grazeScore++;
				spawner.grazeable[i] &= ~PFLAG_GRAZEABLE;
			}

		}

	}

}

void Player::CheckCollideable(Spawner& spawner) {
	const unsigned int size = collideableBullets.size();

	for (unsigned int i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHitRadius, spawner.squaredHitboxRadius,
			transform.position, spawner.position[collideableBullets[i]])) {
			
			spawner.BatchSwap(collideableBullets[i]);

			playerArgs.hearts--;
			collideableBullets.clear();
			return;
		}

	}

}

void Player::Draw() {

	TVertex transformV;
	transformV.translation = transform.position;
	Animation::Draw(&transformV);

}

void Player::DrawBullets() {

	for (int i = 0; i < spawners.size(); i++) {
		spawners[i].Draw(spawners[i].tvertices[0], spawners[i].currentSize - spawners[i].trashSize);
	}

}
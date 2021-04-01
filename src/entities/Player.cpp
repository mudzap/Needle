#include "Player.h"

Player::Player() {
}
	
Player::Player(const PlayerArgs& player, const TransformArgs& entity, const AnimationArgs& animArgs)
	: playerArgs(player), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Transform(entity), Hitbox(animArgs.hitbox) {
	controllable = true;
	conversation = false;

	boundingRadius = sqrt(hitbox.squaredHitRadius);

	//MANUAL SPAWNER CONSTRUCTOR
	unsigned int bullets = 100;
	for (int i = 0; i < playerSpawnerArgs.size(); i++) {
		spawners[0].emplace_back(Spawner(spawnerOffsets[i], bullets, spawnerArgs[i], playerSpawnerArgs[i]));
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

	powerLevel = playerArgs.power / 25;
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

void Player::HandlePlayerCollision(Enemy& enemy) {

	for (int i = 0; i < enemy.enemySpawners.size(); i++)
		CheckCollisionRoutine(enemy.enemySpawners[i]);

};

void Player::HandleSpawners() {

	for (int i = 0; i < spawners[powerLevel].size(); i++) {

		if (spawners[powerLevel][i].HandlePlayerSpawner() && shooting) {
			spawners[powerLevel][i].Instantiate(playerSpawnerArgs[i].baseProjectile, transform.position + spawners[powerLevel][i].offset, spawnerArgs[i]);
			spawners[powerLevel][i].ResetTimer();
		}

		spawners[powerLevel][i].BatchProcessBullets();
		spawners[powerLevel][i].ScissorTest();
		spawners[powerLevel][i].FillVertices();

	}

}

void Player::CheckGrazeable(Spawner& spawner) {

#ifdef _USE_SSE2_
	unsigned int size = spawner.currentSize - spawner.trashSize;
	size = size / 4 + size % 4;
#else
	const unsigned int size = spawner.currentSize - spawner.trashSize;
#endif

	for (unsigned int i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHurtRadius, spawner.squaredHitboxRadius, transform.position, spawner.position[i])) {
			
			//collideableBullets.emplace_back(i);
			spawner.PushCollideable(i);
			if (spawner.grazeable[i] & PFLAG_GRAZEABLE) {
				playerArgs.grazeScore++;
				spawner.grazeable[i] &= ~PFLAG_GRAZEABLE;
			}

		}

	}

}

void Player::CheckCollideable(Spawner& spawner) {
	//const unsigned int size = collideableBullets.size();
	const unsigned int size = spawner.collideables;

	for (unsigned int i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHitRadius, spawner.squaredHitboxRadius,
			transform.position, spawner.position[spawner.collideableBullets[i]])) {
			
			spawner.BatchSwap(spawner.collideableBullets[i]);
			spawner.ClearCollideable();

			playerArgs.hearts--;

			return;
		}

	}

	spawner.ClearCollideable();

}

void Player::Draw() {

	TVertex transformV;
	transformV.translation = transform.position;
	Animation::Draw(&transformV);

}

void Player::DrawBullets() {

	for (int i = 0; i < spawners[powerLevel].size(); i++) {
		spawners[powerLevel][i].Draw(spawners[powerLevel][i].tvertices[0], spawners[powerLevel][i].currentSize - spawners[powerLevel][i].trashSize);
	}

}
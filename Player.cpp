#include "Player.h"
	
Player::Player(const PlayerArgs& player, const TransformArgs& entity, const AnimationArgs& animArgs)
	: playerArgs(player), Animation(animArgs.drawQuad, animArgs.states, animArgs.frameTime), Transform(entity), Hitbox(animArgs.hitbox) {
	controllable = true;
	conversation = false;
	collideableBullets.reserve(512);
	
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

void Player::InitializePlayer(Player& player) {
}

void Player::CheckGrazeable(Spawner& spawner) {
	const unsigned int size = spawner.currentSize - spawner.trashSize;

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

	for (uint16_t i = 0; i < size; i++) {

		if (Hitbox::CheckCollision(hitbox.squaredHitRadius, spawner.squaredHitboxRadius,
			transform.position, spawner.position[collideableBullets[i]])) {
			
			spawner.BatchSwap(collideableBullets[i], spawner.currentSize - spawner.trashSize - 1);
			spawner.trashSize++;

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
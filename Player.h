#ifndef _PLAYER_
#define _PLAYER_

#include <deque>

#include "Complex.h"
#include "Spawner.h"
#include "Transform.h"
#include "Mesh.h"
#include "Hitbox.h"
#include "Enemy.h"
#include "Animation.h"

struct PlayerArgs {
	//Projectile playerBullets[3];
	//Projectile playerLasers[3];
	uint8_t hearts = 2;
	uint8_t bombs = 3;
	uint16_t power = 0;
	uint32_t grazeScore = 0;
	uint32_t pointScore = 0;
	float speed = 5;
};

struct PlayerSpawnerArgs {
	Projectile baseProjectile = defaultProjectile;
	bool aimed = false;

	float initialBulletAngle = 0.f;
	unsigned int bulletArrays = 1;
	float angleBetweenArray = 0.f;
	unsigned int bulletPerArray = 1;
	float angleBetweenBullets = 0.f;
	float bulletOffsetRadius = 0.f;
	Complex bulletOffsetPostion = { 0.f, 0.f };
	unsigned int bulletShotTimer = 0;
};
const PlayerSpawnerArgs defaultPlayerSpawner;

class Player : public Transform, public Hitbox, public Animation {

	public:

		Player(const PlayerArgs& player, const TransformArgs& entity, const AnimationArgs& animArgs);
		~Player();

		void InitializePlayer(Player& player);

		void HandlePlayerMovement() {
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

		void HandlePlayerCollision(Enemy* enemy) {

			for(int i = 0; i < enemy->enemySpawners.size(); i++)
				CheckCollisionRoutine(enemy->enemySpawners[i]);
		}

		//void AddPower(Player* player, Item* item);

		void AddScore(Player& player, uint32_t score) {
		}

		void Draw();

		PlayerArgs playerArgs;

	private:

		bool controllable;
		bool conversation;
		std::vector<unsigned int>collideableBullets;

		void HandleSpawners();

		void CheckGrazeable(Spawner& spawner);
		void CheckCollideable(Spawner& spawner);
		
		void CheckCollisionRoutine(Spawner& spawner) {
			CheckGrazeable(spawner);
			CheckCollideable(spawner);
		}

		void BindPlayerToScreen() {

			if (transform.position.x > 384.f)
				transform.position.x = 384.f;
			else if (transform.position.x < -384.f)
				transform.position.x = -384.f;

			if (transform.position.y > 448.f)
				transform.position.y = 448.f;
			else if (transform.position.y < -448.f)
				transform.position.y = -448.f;
			
		}

		std::array<Spawner, 3> spawners;
		std::array<SpawnerArgs, 3> spawnerArgs;
		std::array<PlayerSpawnerArgs, 3> playerSpawnerArgs;

};

#endif
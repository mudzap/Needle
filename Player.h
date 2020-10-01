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

#include "ProjData.h"

struct PlayerArgs {
	unsigned short hearts = 2;
	unsigned short bombs = 3;
	unsigned int power = 0;
	uint32_t grazeScore = 0;
	uint32_t pointScore = 0;
	float speed = 5;
};

const ProjArgs playerProjectileArgs1 = {
	.flags = (ProjFlags)0,
	.initialVel = {8.f, 0.f},
	.angle = 90.f
};

const PlayerSpawnerArgs playerSpawner1 = {
	.baseProjectile = Projectile(playerProjectileArgs1, RingedBulletCyan),
	.aimed = false,
	.bulletShotTimer = 100
};

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

			for (int i = 0; i < enemy->enemySpawners.size(); i++)
				CheckCollisionRoutine(enemy->enemySpawners[i]);
		};

		void HandleSpawners();


		//void AddPower(Player* player, Item* item);

		void AddScore(Player& player, uint32_t score) {
		}

		void Draw();
		void DrawBullets();

		PlayerArgs playerArgs;

		bool controllable = true;
		bool conversation = false;
		bool shooting = false;

	private:

		std::vector<unsigned int>collideableBullets;

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

		std::array<Spawner, 2> spawners;
		std::array<SpawnerArgs, 2> spawnerArgs = { defaultSpawner, defaultSpawner };
		std::array<PlayerSpawnerArgs, 2> playerSpawnerArgs = { playerSpawner1, playerSpawner1 };
		std::array<Complex, 2> spawnerOffsets = { Complex{-20.f, 5.f}, Complex{20.f, 5.f} };

};

#endif
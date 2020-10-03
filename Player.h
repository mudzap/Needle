#ifndef _PLAYER_
#define _PLAYER_

#include <deque>
#include <vector>

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
	float speed = 7.f;
	float halfspeed = speed * 0.5f;
};

const ProjArgs playerProjectileArgs1 = {
	.flags = (ProjFlags)0,
	.initialVel = {20.f, 0.f},
	.angle = 90.f	
};

const PlayerSpawnerArgs playerSpawner1 = {
	.baseProjectile = Projectile(playerProjectileArgs1, RingedBulletCyan),
	.aimed = false,
	.bulletShotTimer = 80
};

class Enemy;

class Player : public Transform, public Hitbox, public Animation {

	public:

		Player(const PlayerArgs& player, const TransformArgs& entity, const AnimationArgs& animArgs);
		~Player();

		void HandlePlayerMovement();
		void HandlePlayerCollision(Enemy* enemy);
		void HandleSpawners();


		//void AddPower(Player* player, Item* item);

		void AddScore(Player& player, uint32_t score) {
		}

		void Draw();
		void DrawBullets();

		std::vector<Spawner>& GetPlayerSpawners() {
			return spawners[powerLevel];
		};

		PlayerArgs playerArgs;

		bool controllable = true;
		bool conversation = false;
		bool shooting = false;

		//5 STATES, EACH ONE EVERY 25 POWER, 0, 25, 50, 75, 100
		//INDICATES WHICH SPAWNER FROM THE SPAWNER ARRAY TO USE
		std::vector<Spawner> spawners[5];

		float currentSpeed = 0.f;
		std::vector<unsigned int> collideableBullets;

	private:

		int powerLevel = 0;
		float boundingRadius = 0.f;

		void CheckGrazeable(Spawner& spawner);
		void CheckCollideable(Spawner& spawner);
		
		void CheckCollisionRoutine(Spawner& spawner) {
			CheckGrazeable(spawner);
			CheckCollideable(spawner);
		}

		void BindPlayerToScreen() {

			if (transform.position.x > 384.f - boundingRadius)
				transform.position.x = 384.f - boundingRadius;
			else if (transform.position.x < -384.f + boundingRadius)
				transform.position.x = -384.f + boundingRadius;

			if (transform.position.y > 448.f - boundingRadius)
				transform.position.y = 448.f - boundingRadius;
			else if (transform.position.y < -448.f + boundingRadius)
				transform.position.y = -448.f + boundingRadius;
			
		}

		//SPAWNER STATES
		//STATE 1
		std::array<SpawnerArgs, 2> spawnerArgs = { defaultSpawner, defaultSpawner };
		std::array<PlayerSpawnerArgs, 2> playerSpawnerArgs = { playerSpawner1, playerSpawner1 };
		std::array<Complex, 2> spawnerOffsets = { Complex{-20.f, 5.f}, Complex{20.f, 5.f} };

};

#endif
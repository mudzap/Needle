#ifndef _POOL_
#define _POOL_

#define MAX_ENEMIES 64

#include "entities/Enemy.h"
#include "video/Mesh.h"

#include <array>
#include <algorithm>
#include <set>

//Changes!
//You feed pool a list of handles, it returns a list of pointers
//Enemies and spawners are independent now, they use handles exclusively
//Spawners will keep operating separated of enemies
//You can command them with their handles
//Adding a spawner to an enemy means requesting a handle and storing it.

class Pool {

	public:

		std::array<Enemy, MAX_ENEMIES> enemies;
		std::set<Enemy*> activeEnemies;

		Enemy* CreateEnemyTransform(const TransformArgs& entity, const AnimationArgs& anim, int health) {
			Enemy* enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreateTransform(entity, anim, health);
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES;

			return enemyReference;
		};

		Enemy* CreateEnemyComplex(const Complex position, const AnimationArgs& anim, int health) {
			Enemy* enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreatePosition(position, anim, health);
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES;

			return enemyReference;
		};

		void DestroyEnemy(Enemy &enemy) {
			enemy.dead = true;
		};

		void DestroyAllEnemies() {
			for (Enemy elem : enemies) {
				DestroyEnemy(elem);
			}
			
		};

		void HandlePool(Player& player) {
			for (Enemy elem : enemies) {
				elem.HandleEnemy(player);
			}
		};


	private:

		int enemyPointer = 0;

};

#endif
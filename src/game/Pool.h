#ifndef _POOL_
#define _POOL_

#define MAX_ENEMIES 64

#include "Enemy.h"
#include "Mesh.h"

#include <array>
#include <algorithm>
#include <set>

class Pool {

	public:

		std::array<Enemy, MAX_ENEMIES> enemies;
		//std::set<Enemy*> activeEnemies;

		inline Enemy* CreateEnemyTransform(const TransformArgs& entity, const AnimationArgs& anim, int health) {
			Enemy* const enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreateTransform(entity, anim, health);
			//enemies[enemyPointer] = std::move(Enemy(entity, anim, health));
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES;

			//if (activeEnemies.find(enemyReference) == activeEnemies.end())
			//	activeEnemies.emplace(enemyReference);

			return enemyReference;
		};

		inline Enemy* CreateEnemyComplex(const Complex position, const AnimationArgs& anim, int health) {
			Enemy* const enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreatePosition(position, anim, health);
			//enemies[enemyPointer] = std::move(Enemy(position, anim, health));
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES;

			//if (activeEnemies.find(enemyReference) == activeEnemies.end())
			//	activeEnemies.emplace(enemyReference);

			return enemyReference;
		};

		inline void DestroyEnemy(Enemy &enemy) {
			//enemyReference->~Enemy();
			//activeEnemies.erase(enemyReference);
			enemy.dead = true;
		};

		inline void DestroyAllEnemies() {
			for (Enemy elem : enemies) {
				DestroyEnemy(elem);
			}
			//activeEnemies.clear();
			
		};

		inline void HandlePool(Player& player) {
			for (Enemy elem : enemies) {
				elem.HandleEnemy(player);
			}
		};


	private:

		int enemyPointer = 0;

};

#endif
#ifndef _POOL_
#define _POOL_

#include "entities/Enemy.h"
#include "entities/Entity.h"
#include "video/Mesh.h"
#include "util/Log.h"
#include "entities/Player.h"
#include <array>
#include <algorithm>
#include <set>
#include <memory>

#define MAX_ENEMIES_D 64

//Changes!
//You feed pool a list of handles, it returns a list of pointers
//Enemies and spawners are independent now, they use handles exclusively
//Spawners will keep operating separated of enemies
//You can command them with their handles
//Adding a spawner to an enemy means requesting a handle and storing it.

/** Allows for better type safety and easier bookkeeping in Pool */
template<typename T>
struct KeyContainer {
	T* type_ptr = NULL;
	int ID = -1;
};

//class Enemy;

template<typename T, int N>
class Pool {

	public:

		KeyContainer<T> GetHandle(){

			int returnHandle = 0;

			if(activeElements.size() >= N) {   //If the set of active elements contains the handle to all the elements
				SHMY_LOGD("Pool filled! Replacing on index %d\n", currentIndex);
				returnHandle = currentIndex;   //Replace
				currentIndex = (currentIndex + 1) % N;
			} else {    //Otherwise, give from the inactive elements set
				//int newHandle = inactiveElemets.cbegin();
				auto it = inactiveElements.begin();
				returnHandle = *it;
				inactiveElements.erase(inactiveElements.cbegin());
				activeElements.insert(returnHandle);
			}

			elements[returnHandle].activeSetPtr = &activeElements;
			elements[returnHandle].inactiveSetPtr = &inactiveElements;
			elements[returnHandle].ID = returnHandle;

			return KeyContainer{&elements[returnHandle], returnHandle};

		}

		void GetHandles(KeyContainer<T> handle[], int desiredElements){

			int i = 0;
			while(desiredElements > i) {
				handle[i] = GetHandle();
				i++;
			}

		}

		void FreeHandle(KeyContainer<T>* handle){

			if(handle->ID < 0 || inactiveElements.find(handle->ID) != inactiveElements.end()) {

				SHMY_LOGE("Tried to free handle with no object assigned, or handle previously freed\n");

			} else if(auto it = activeElements.find(handle->ID) != activeElements.end()) {
				
				activeElements.erase(it);   
				inactiveElements.insert(handle->ID);

			}

			handle->ID = -1;
			handle->type_ptr = NULL;

		}
		void FreeHandles(KeyContainer<T> handle[], int desiredElements){

			for(int i = 0; i < desiredElements; i++) {
				FreeHandle(&handle[i]);
			}

		}
		/** DANGEROUS, ONLY CALL AT END OF LEVEL, WHEN FREEING UP ALL THE HANDLES */
		void FreeAllHandles(){

			//Resets everything
			SHMY_LOGD("Resetting Pool\n");
			activeElements.clear();
			inactiveElements.clear();
			currentIndex = 0;

		}

		int ProcessElements(){

			for(int i: this->activeElements) {
				this->elements[i].Handle();
			}

			return 0;

		}

	protected:

		std::array<T, N> elements; /**<	Handle - Element map	*/
		std::set<int> activeElements;	/**< Keeps track of active elements (by means of its handle) */
		std::set<int> inactiveElements;	/**< Keeps track of inactive elements (ditto)*/

		int currentIndex = 0; 	/**< The handle itself */

};

template<int N>
class EnemPool: public Pool<Enemy, N> {

	public:

		int ProcessElements(Player& player){  ///< Overload for enemy pool

			for(int i: this->activeElements) {
				this->elements[i].Handle(player);
			}

			return 0;

		}

		std::unique_ptr<Player> target;

};

//DEPRECATED

class EnemyPool {
/*
	public:

		std::array<Enemy, MAX_ENEMIES_D> enemies;
		std::set<Enemy*> activeEnemies;

		Enemy* CreateEnemyTransform(const TransformArgs& entity, const AnimationArgs& anim, int health) {
			Enemy* enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreateTransform(entity, anim, health);
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES_D;

			return enemyReference;
		}

		Enemy* CreateEnemyComplex(const Complex position, const AnimationArgs& anim, int health) {
			Enemy* enemyReference = &enemies[enemyPointer];

			enemies[enemyPointer].RecreatePosition(position, anim, health);
			enemyPointer = (enemyPointer + 1) % MAX_ENEMIES_D;

			return enemyReference;
		}

		void DestroyEnemy(Enemy &enemy) {
			enemy.dead = true;
		}

		void DestroyAllEnemies() {
			for (Enemy elem : enemies) {
				DestroyEnemy(elem);
			}
			
		}

		void HandlePool(Player& player) {
			for (Enemy elem : enemies) {
				elem.Handle(player);
			}
		}


	private:

		int enemyPointer = 0;
		*/

};


#endif
#ifndef _ENTITY_
#define _ENTITY_

#define MAX_CHILDREN_PER_ENTITY 16

#include <set>
#include <unordered_set>
#include <memory>
#include <array>

#include "util/Log.h"
#include "game/Pool.h"

template<typename T, int N>
class Pool;

class Entity {

    public:

        Entity* target; //< Separate pointer for defined target (player, enemy, other function)

		std::set<int>* activeSetPtr = NULL;

		int ID = -1;

    protected:
    
        void MakeInactive();


};

template<typename T, int N>
class Children {

    public:

        void AddChild(KeyContainer<T> handle) {
            childrenID.insert(handle);
        }

        void KillChild(KeyContainer<T> handle) {
            childrenID.erase(handle);
            childrenPool->FreeHandle(handle);
        }

        void SleepChild(KeyContainer<T> handle) {
            childrenID.erase(handle);
        }

        void SetPool(Pool<T, N>* pool) {
            childrenPool = pool;
        }

    protected:

        Pool<T, N>* childrenPool;

        std::set<KeyContainer<T>> childrenID; //< Stores IDs of children objects which inherit the entity transforms

};

#endif
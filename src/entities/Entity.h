#ifndef _ENTITY_
#define _ENTITY_

#define MAX_CHILDREN_PER_ENTITY 16

#include <set>
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

        void AddChild(int ID) {
            //enemySpawners.emplace_back(spawner);
            int newID = childrenPool->GetHandle().ID;
            childrenID.insert(newID);
        }

        void KillChild(int ID) {
            //enemySpawners.emplace_back(spawner);
            int newID = childrenPool->GetHandle().ID;
            childrenID.insert(newID);
        }

    protected:

        Pool<T, N>* childrenPool;

        std::set<int> childrenID; //< Stores IDs of children objects which inherit the entity transforms

};

#endif
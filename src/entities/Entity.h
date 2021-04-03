#ifndef _ENTITY_
#define _ENTITY_

#define MAX_CHILDREN_PER_ENTITY 16

#include <set>
#include <memory>
#include <array>

#include "util/Log.h"

class Entity {

    public:

        Entity();

        std::array<int, MAX_CHILDREN_PER_ENTITY> childrenID; //< Stores IDs of children objects
        Entity* target; //< Separate pointer for defined target (player, enemy, other function)

		std::set<int>* activeSetPtr = NULL;
		std::set<int>* inactiveSetPtr = NULL;

		int ID = -1;

    protected:
    
        void MakeInactive();

};

#endif
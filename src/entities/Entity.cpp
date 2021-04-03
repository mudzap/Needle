#include "Entity.h"

Entity::Entity() {

    childrenID.fill({-1});

}

void Entity::MakeInactive() {

    //This error shouldn't ocurr, I might consider removing it
    //It has almost no overhead anyways, so lets keep it.

    if(ID < 0) {
        
        SHMY_LOGE("Tried to free handle with no object assigned\n");

    } else if(auto it = activeSetPtr->find(ID) != activeSetPtr->end()) {
        
        activeSetPtr->erase(it);   
        inactiveSetPtr->insert(ID);

        ID = -1;

    }

}
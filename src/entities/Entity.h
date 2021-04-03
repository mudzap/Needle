#ifndef _ENTITY_
#define _ENTITY_

#include <set>
#include <memory>

#include "util/Log.h"

class Entity {

    public:

		std::set<int>* activeSetPtr = NULL;
		std::set<int>* inactiveSetPtr = NULL;

		int ID = -1;

    protected:
    
        void MakeInactive() {

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

};

#endif
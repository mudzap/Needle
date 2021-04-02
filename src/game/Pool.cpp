#include "Pool.h"

template<typename T, int N>
int Pool<T,N>::getHandles(int handle[], int desiredElements){
    
    /*
    for(int i = 0; i < desiredElements; i++) {
        handle[i] = currentIndex + i;
        activeElements.insert(currentIndex + i);
        inactiveElements.erase(currentIndex + i);
        active++;
    }
    */

    int i = 0;
    while(desiredElements > i) {
        if(active >= N) {   //If the set of active elements contains the handle to all the elements
            handle[i] = currentIndex;   //Replace
            currentIndex = (currentIndex + 1) % N;
        } else {    //Otherwise, give from the inactive elements set
            //int newHandle = inactiveElemets.cbegin();
            handle[i] = inactiveElements.cbegin();
            inactiveElements.erase(inactiveElements.cbegin());
            activeElements.insert(handle[i]);
            active++;
        }

        i++;
    }

}

template<typename T, int N>
std::unique_ptr<T*[]> Pool<T,N>::getElement(int handle[], int desiredElements){

    std::unique_ptr<T*[]> elementPointers = std::make_unique<T*[]> (new T*[desiredElements]);

    for(int i = 0; i < desiredElements; i++) {
        elementPointers[i] = &elements[handle[i]];
    }

    return elementPointers;

}

template<typename T, int N>
void Pool<T,N>::freeHandle(Pool<T,N>& pool, int handle){

    if(auto it = pool.activeElements.find(handle) != pool.activeElements.end()) {
        pool.activeElements.erase(it);   
        pool.inactiveElements.insert(handle);
        pool.active--;
    }

}

template<typename T, int N>
void Pool<T,N>::freeHandles(Pool<T,N>& pool, int handle[], int desiredElements){

    for(int i = 0; i < desiredElements; i++) {
        Pool<T,N>::freeHandle(pool, handle[i]);
    }

}

template<typename T, int N>
void Pool<T,N>::freeAllHandles(Pool<T,N>& pool){

    //Resets everything
    pool.activeElements.clear();
    pool.inactiveElements.clear();
    pool.currentIndex = 0;
    pool.active = 0;

}

template<typename T, int N>
int Pool<T,N>::processElements(){

    for(int i: activeElements) {
        elements[i].handle();
    }

}
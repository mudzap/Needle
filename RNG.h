#ifndef _RNG_
#define _RNG_

#include <sstream>

class RNG {

    public:

        inline static void setSeed(uint32_t seed) {
            state = seed;
        }

        inline static float Range(float min, float max) {
            return xorshift32() * (max - min) / 4294967296 + min;
        };

        inline static uint32_t getState() {
            return xorshift32();
        };

    private:

        static uint32_t state;

        inline static uint32_t xorshift32() {
            state ^= state << 13;
            state ^= state >> 17;
            state ^= state << 5;
            return state;
        }
};

#endif
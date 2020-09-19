#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include <unordered_map>
#include <array>
#include <cstdarg>

#include "Complex.h"
#include "Mesh.h"
#include "Timer.h"
#include "Hitbox.h"

struct ipair {
	unsigned int x;
	unsigned int y;
};

struct AnimationArgs {
	HitboxArgs hitbox = { 64, 1024 };
	Quad drawQuad = { 0, 0, 32, 32 };
	ipair states = { 1, 1 };
	unsigned int frameTime = 5;
	Quad spriteSheetQuad = { 0, 0, 32, 32 };
};
const AnimationArgs defaultAnimationArgs;

class Animation: public Mesh {

	public:

		Animation();
		Animation(const Quad& drawQuad, const ipair animStates, const unsigned int frameTime);
		~Animation();

		//void SetStateSprites(unsigned int frames, unsigned int number, ipair states ...);
		void SetStateSprites(const unsigned int frames, const ipair state);
		void SetStateSprites(const unsigned int frames, const unsigned int states ...);
		void SetStateSprites(const unsigned int frames);

		void CreateMapAndMesh(const float textureSize, const Quad spriteRegion);
		void SetAnimationState(const ipair state);
		void TransitionToAnimationState(const ipair stateID);
		void SetAnimationState(const unsigned int stateX, const unsigned int stateY);
		void TransitionToAnimationState(const unsigned int stateX, const unsigned int stateY);
		void HandleAnimation();

		void Draw(const TVertex* instanceData);

	private:

		Quad drawQuad;
		unsigned int frameTime;

		std::vector<std::array<Complex,4>> spriteMap;
		//std::vector<unsigned int>* animationSpritesID;

		std::vector<unsigned int> animationSpritesID[16];//DYNAMIC ALLOCATION ISN'T NICE WITH LUA


		ipair dimensions;
		ipair currentState;
		ipair targetState;
		unsigned int currentSpriteFrame;
		unsigned int currentFrame;

		unsigned int animationFrameOffset;

		inline int sign(int x) {
			return (x > 0) - (x < 0);
		}

};

#endif
#ifndef _ANIMATION_
#define _ANIMATION_

#include <vector>
#include <unordered_map>
#include <array>
#include <cstdarg>

#include "math/Complex.h"
#include "video/Mesh.h"
#include "util/Timer.h"
#include "math/Math.h"
#include "util/Log.h"

#include "Hitbox.h"

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

		void Recreate(const AnimationArgs& animationArgs, const int stateSprites);

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

		Quad drawQuad;
		unsigned int frameTime;
		ipair dimensions;


	private:


		std::vector<std::array<Complex,4>> spriteMap;

		std::vector<unsigned int> animationSpritesID[16];//DYNAMIC ALLOCATION ISN'T NICE WITH LUA

		ipair currentState;
		ipair targetState;

		unsigned int currentSpriteFrame;
		unsigned int currentFrame;

		unsigned int animationFrameOffset;

};

#endif
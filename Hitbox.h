#ifndef _HIT_
#define _HIT_

#include "Complex.h"
#include <algorithm>

struct HitboxArgs {
	float squaredHitRadius = 0;
	float squaredHurtRadius = 0;

	bool operator==(const HitboxArgs& other) const
	{
		return (
			squaredHitRadius == other.squaredHitRadius &&
			squaredHurtRadius == other.squaredHurtRadius
			);
	}

};
const HitboxArgs defaultHitbox = { 36, 1024 };

class Hitbox {
	
	public:

		Hitbox();
		Hitbox(const HitboxArgs& args);
		~Hitbox();

		inline static bool CheckCollision(const float sRadius1, const float sRadius2, const float distance) {
			return sRadius1 + sRadius2 > distance ? std::move(true) : std::move(false);
		}
		inline static bool CheckCollision(const float sRadius1, const float sRadius2, const Complex firstPos, const Complex secondPos) {
			return sRadius1 + sRadius2 > Complex::SquaredAbs(secondPos - firstPos) ? std::move(true) : std::move(false);
		}
		HitboxArgs hitbox;

};

#endif
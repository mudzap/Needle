#ifndef _PROJ_
#define _PROJ_

#define STOPNGO 0
#define SINE 1
#define ZIGZAG 2

#include "video/Mesh.h"
#include "math/Complex.h"

#include "Transform.h"
#include "Hitbox.h"


//////////////////////////////////////////////////////////////
//				PROJECTILE PROTOTYPE DEFINITION				//
//////////////////////////////////////////////////////////////

enum ProjFlags {
	PFLAG_AIMED_AT_PLAYER = 1 << 0,
	PFLAG_FOLLOW_PLAYER = 1 << 2,
	PFLAG_GRAZEABLE = 1 << 3,
	PFLAG_ENEMY = 1 << 4,
	PFLAG_PLAYER = 1 << 5,
	PFLAG_DEAD = 1 << 6,
	PFLAG_CANCELLABLE = 1 << 8,
	DefaultProj = PFLAG_GRAZEABLE | PFLAG_ENEMY | PFLAG_CANCELLABLE
};
typedef enum ProjFlags ProjFlags;

inline ProjFlags operator |(ProjFlags a, ProjFlags b) {
	return static_cast<ProjFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inline ProjFlags operator &(ProjFlags a, ProjFlags b) {
	return static_cast<ProjFlags>(static_cast<int>(a) & static_cast<int>(b));
}
inline ProjFlags& operator |=(ProjFlags& a, ProjFlags b) {
	return a = a | b;
}
inline ProjFlags& operator &=(ProjFlags& a, ProjFlags b) {
	return a = a & b;
}

struct ProjArgs {

	ProjFlags flags = DefaultProj;
	Complex initialVel = { 1.f, 0.f };
	Complex acceleration = { 0.f, 0.f };
	float stopandgoFrequency = -1.f;
	float sineFrequency = -1.f;
	float zigzagFrequency = -1.f;
	float decayRate = 1.0f;
	float sineAmplitude = 0.f;
	float zigzagAmplitude = 0.f;
	float angle = 0.f;
	float angleDelta = 0.f;
	Complex gravityConstant = Complex(0.f, 0.f);

	bool operator==(const ProjArgs& other) const
	{
		return (
			decayRate == other.decayRate &&
			angleDelta == other.angleDelta &&
			gravityConstant == other.gravityConstant
			);
	}

};
const ProjArgs defaultProjectileArgs;

enum CommonFlags {
	PFLAG_IS_SAKURA = 1 << 0,
	PFLAG_SPAWN_PARTICLE = 1 << 1,
	PFLAG_DESTROY_PARTICLE = 1 << 2,
	PFLAG_ROTATES_TOWARDS = 1 << 3,
	PFLAG_ROTATES_PASSIVELY = 1 << 4,
	DefaultCommon = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE
};
typedef enum CommonFlags CommonFlags;

inline CommonFlags operator |(CommonFlags a, CommonFlags b) {
	return static_cast<CommonFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inline CommonFlags operator &(CommonFlags a, CommonFlags b) {
	return static_cast<CommonFlags>(static_cast<int>(a) & static_cast<int>(b));
}
inline CommonFlags& operator |=(CommonFlags& a, CommonFlags b) {
	return a = a | b;
}
inline CommonFlags& operator &=(CommonFlags& a, CommonFlags b) {
	return a = a & b;
}

struct CommonArgs {
	CommonFlags flags = DefaultCommon;
	HitboxArgs hitbox = { 32, 1024 };
	Quad quad = { 96, 64, 32, 32 };

	bool operator==(const CommonArgs& other) const
	{
		return (
			flags == other.flags &&
			hitbox == other.hitbox &&
			quad == other.quad
			);
	}

};
const CommonArgs defaultCommonProjectileArgs;


//////////////////////////////////////////////////////////////////////////////////////////7/////////////////////////////////////////////


class Projectile {
	
	public:

		Projectile();
		Projectile(const ProjArgs& projectile, const CommonArgs& common);
		~Projectile();
		ProjArgs projectile;
		CommonArgs common;
		
};

const Projectile defaultProjectile = Projectile(defaultProjectileArgs, defaultCommonProjectileArgs);

#endif
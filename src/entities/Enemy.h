#ifndef _ENEMY_
#define _ENEMY_


#include "video/Mesh.h"
#include "math/Complex.h"
//#include "util/Timer.h"
#include "util/Log.h"

#include "Hitbox.h"
//#include "Projectile.h"
#include "Spawner.h"
#include "Animation.h"
#include "Player.h"
#include "Transform.h"
#include "Entity.h"

#include <vector>
#include <memory>

#define MAX_ENEMIES 64

enum EnemyState {
	MOVE_TOWARDS,
	SPRING_TOWARDS,
	SLOW_TOWARDS,
	STOP,
	FULL_STOP,
	DRIFT
};

template<typename T, int N>
class Pool;

//class Player;
class Projectile;

class Enemy:
	public Transform,
	public Animation,
	public Hitbox,
	public Entity,
	public Children<Spawner, MAX_SPAWNERS> {

	public:

		//TAKES ITEM, SPAWNER, 
		Enemy();

		/* GENERALLY MEANT FOR LUA */
		void RecreateTransform(const TransformArgs& entity, const AnimationArgs& animArgs, const unsigned int health);
		void RecreatePosition(const Complex position, const AnimationArgs& animArgs, const unsigned int health);

		void SetPosition(Complex position);
		void Destroy();
		void MoveTowards1(Complex position, float speed, float accel);
		void MoveTowards2(float angle, Complex speed, Complex accel);
		void SpringTowards1(Complex position, float accelPerUnit);
		void SpringTowards2(float angle, float distance, float accelPerUnit);
		void SlowTowards1(Complex position, float velocityPerUnit);
		void SlowTowards2(float angle, float distance, float velocityPerUnit);
		void Stop1();
		void Stop2(float brakeFactor);
		void Drift1();
		void Drift2(float friction);
		void CircleAround(Complex positon);
		/* END LUA FUNCS */

		void Handle(Player& player);

		void ScissorTest();

		void DrawBullets();
		void Draw();

		void CheckCollideable(Spawner& spawner);

		EnemyState state = STOP;
		bool dead = true;

	private:

		//MOVE TO TRANSFORM?
		Complex direction;
		float exponent;
		float velocityPerUnit;
		float accelPerUnit;
		Complex targetPosition;
		float brakeConstant;
		int health = 10;

};

#endif
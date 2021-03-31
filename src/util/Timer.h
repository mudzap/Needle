#ifndef _TIMER_
#define _TIMER_


//#include "entities/Enemy.h"
#include "entities/Laser.h"
#include "entities/Spawner.h"
//#include "entities/Pool.h"


extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <vector>
#include <sol/sol.hpp>
#include <algorithm>
#include <functional>


//class Enemy;
class Laser;
class Spawner;
//class Pool;

struct LuaTimeEvent {
	unsigned int startTime;
	unsigned int duration;
	sol::protected_function function;
	//sol::state luaState;
};

template<typename T>
struct TimeEvent {
	unsigned int startTime;
	unsigned int duration;
	void (T::* function)();
	T* typePointer;
};


class Timer {

	public:

		Timer() {};

		~Timer() {
			Timer::spawnerTimers.clear();
			Timer::laserTimers.clear();
			Timer::luaTimers.clear();
		}

		static uint32_t countedFrames;
		static uint32_t lifeTime;
		static uint32_t lifeTimeFrames;

		static inline void AddTimer(unsigned int time, void(Laser::* callback)(), Laser* t) {
			Timer::laserTimers.push_back({ Timer::countedFrames, time, callback });
		};
		/*
		static inline void AddTimer(unsigned int time, void(Enemy::* callback)(), Enemy* t) {
			Timer::enemyTimers.push_back({ Timer::countedFrames, time, callback });
		};*/
		static inline void AddTimer(unsigned int time, void(Spawner::* callback)(), Spawner* t) {
			Timer::spawnerTimers.push_back({ Timer::countedFrames, time, callback });
		};
		/*
		static inline void AddTimer(unsigned int time, void(Pool::* callback)(), Pool* t) {
			Timer::poolTimers.push_back({ Timer::countedFrames, time, callback });
		};*/
		static inline void AddLuaTimer(unsigned int time, sol::function luaFunc) { //sol::state luaState
			Timer::luaTimers.push_back({ Timer::countedFrames, time, luaFunc }); //luaState
		};

		
		inline void Process() {

			loopThroughTimers<Laser>(Timer::laserTimers);
			//loopThroughTimers<Enemy>(Timer::enemyTimers);
			loopThroughTimers<Spawner>(Timer::spawnerTimers);
			//loopThroughTimers<Pool>(Timer::poolTimers);

			loopThroughTimers(Timer::luaTimers);

		}


	private:
		
		static std::vector<TimeEvent<Spawner>> spawnerTimers;
		//static std::vector<TimeEvent<Pool>> poolTimers;
		//static std::vector<TimeEvent<Enemy>> enemyTimers;
		static std::vector<TimeEvent<Laser>> laserTimers;

		static std::vector<LuaTimeEvent> luaTimers;

		template<typename T>
		inline void loopThroughTimers(std::vector<TimeEvent<T>>& timers) {

			for (int i = 0; i < timers.size(); i++) {
				if (Timer::countedFrames - timers[i].startTime >= timers[i].duration) {
					(*(timers[i].typePointer).*(timers[i].function))();
					timers.erase(timers.begin() + i);
					i--;
				}
			}

		};
		
		inline void loopThroughTimers(std::vector<LuaTimeEvent>& luaTimers) {

			for (int i = 0; i < luaTimers.size(); i++) {
				if (Timer::countedFrames - luaTimers[i].startTime >= luaTimers[i].duration) {
					luaTimers[i].function();
					luaTimers.erase(luaTimers.begin() + i);
					i--;
				}
			}

		};

};

#endif
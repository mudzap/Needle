#include "Timer.h"
//#include "Enemy.h"
#include "entities/Laser.h"
#include "entities/Spawner.h"
//#include "Pool.h"

uint32_t Timer::countedFrames = 0;
uint32_t Timer::lifeTime = 0;
uint32_t Timer::lifeTimeFrames = 0;

std::vector<TimeEvent<Spawner>> Timer::spawnerTimers;
//std::vector<TimeEvent<Pool>> Timer::poolTimers;
//std::vector<TimeEvent<Enemy>> Timer::enemyTimers;
std::vector<TimeEvent<Laser>> Timer::laserTimers;
std::vector<LuaTimeEvent> Timer::luaTimers;

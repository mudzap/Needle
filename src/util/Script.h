#ifndef _SCRIPT_
#define _SCRIPT_

extern "C"
{
#include "lua/include/lua.h"
#include "lua/include/lauxlib.h"
#include "lua/include/lualib.h"
}
#include "sol/sol.hpp"
#include <vector>

#include "Mesh.h"
#include "Complex.h"
#include "Hitbox.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Pool.h"
#include "Animation.h"
#include "Camera.h"

enum ScriptIdentifiers {
    PLAYER = 1,
    ENEMY = 2,
    STAGE = 3,
    UI = 4
};


struct Script
{
    public:

        static void CreateUsertypes(sol::state& luaState);
        
};

#endif
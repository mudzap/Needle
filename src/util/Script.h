#ifndef _SCRIPT_
#define _SCRIPT_

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "sol/sol.hpp"
#include <vector>

#include "video/Mesh.h"
#include "math/Complex.h"
#include "entities/Hitbox.h"
#include "entities/Projectile.h"
#include "entities/Enemy.h"
#include "game/Pool.h"
#include "entities/Animation.h"
#include "game/Camera.h"

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
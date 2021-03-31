#ifndef _LUA_DATA_
#define _LUA_DATA_

#include "util/Script.h"

#include "ProjData.h"
#include "AnimData.h"

struct LuaData {

	inline static void LoadProjData(sol::state& lua) {

		//ARROW
		lua["arrow_black"] = ArrowBlack;
		lua["arrow_gray"] = ArrowGray;
		lua["arrow_darkred"] = ArrowDarkRed;
		lua["arrow_red"] = ArrowRed;
		lua["arrow_darkmagenta"] = ArrowDarkMagenta;
		lua["arrow_magenta"] = ArrowMagenta;
		lua["arrow_darkblue"] = ArrowDarkBlue;
		lua["arrow_blue"] = ArrowBlue;
		lua["arrow_darkcyan"] = ArrowDarkCyan;
		lua["arrow_cyan"] = ArrowCyan;
		lua["arrow_darkgreen"] = ArrowDarkGreen;
		lua["arrow_green"] = ArrowGreen;
		lua["arrow_lime"] = ArrowLime;
		lua["arrow_darklime"] = ArrowDarkYellow;
		lua["arrow_darkyellow"] = ArrowYellow;
		lua["arrow_orange"] = ArrowOrange;

		//RINGED
		lua["ringed_black"] = RingedBulletBlack;
		lua["ringed_gray"] = RingedBulletGray;
		lua["ringed_darkred"] = RingedBulletDarkRed;
		lua["ringed_red"] = RingedBulletRed;
		lua["ringed_darkmagenta"] = RingedBulletDarkMagenta;
		lua["ringed_magenta"] = RingedBulletMagenta;
		lua["ringed_darkblue"] = RingedBulletDarkBlue;
		lua["ringed_blue"] = RingedBulletBlue;
		lua["ringed_darkcyan"] = RingedBulletDarkCyan;
		lua["ringed_cyan"] = RingedBulletCyan;
		lua["ringed_darkgreen"] = RingedBulletDarkGreen;
		lua["ringed_green"] = RingedBulletGreen;
		lua["ringed_lime"] = RingedBulletLime;
		lua["ringed_darklime"] = RingedBulletDarkYellow;
		lua["ringed_darkyellow"] = RingedBulletYellow;
		lua["ringed_orange"] = RingedBulletOrange;

		//BULLET
		lua["bullet_black"] = BulletBlack;
		lua["bullet_gray"] = BulletGray;
		lua["bullet_darkred"] = BulletDarkRed;
		lua["bullet_red"] = BulletRed;
		lua["bullet_darkmagenta"] = BulletDarkMagenta;
		lua["bullet_magenta"] = BulletMagenta;
		lua["bullet_darkblue"] = BulletDarkBlue;
		lua["bullet_blue"] = BulletBlue;
		lua["bullet_darkcyan"] = BulletDarkCyan;
		lua["bullet_cyan"] = BulletCyan;
		lua["bullet_darkgreen"] = BulletDarkGreen;
		lua["bullet_green"] = BulletGreen;
		lua["bullet_lime"] = BulletLime;
		lua["bullet_darklime"] = BulletDarkYellow;
		lua["bullet_darkyellow"] = BulletYellow;
		lua["bullet_orange"] = BulletOrange;

		//KNIFE
		lua["knife_black"] = KnifeBlack;
		lua["knife_red"] = KnifeRed;
		lua["knife_magenta"] = KnifeMagenta;
		lua["knife_blue"] = KnifeBlue;
		lua["knife_cyan"] = KnifeCyan;
		lua["knife_green"] = KnifeGreen;
		lua["knife_darkyellow"] = KnifeYellow;
		lua["knife_gray"] = KnifeGray;

		//BIG_BULLET
		lua["bigbullet_black"] = BigBulletBlack;
		lua["bigbullet_red"] = BigBulletRed;
		lua["bigbullet_magenta"] = BigBulletMagenta;
		lua["bigbullet_blue"] = BigBulletBlue;
		lua["bigbullet_cyan"] = BigBulletCyan;
		lua["bigbullet_green"] = BigBulletGreen;
		lua["bigbullet_darkyellow"] = BigBulletYellow;
		lua["bigbullet_gray"] = BigBulletGray;

		//HUGE_BULLET
		lua["hugebullet_red"] = HugeBulletRed;
		lua["hugebullet_blue"] = HugeBulletBlue;
		lua["hugebullet_green"] = HugeBulletGreen;
		lua["hugebullet_yellow"] = HugeBulletYellow;

	}

	inline static void LoadAnimData(sol::state& lua) {

		//PLAYER
		lua["player"] = playerAnimation;

		//ENEMY
		lua["ran"] = ranAnimation;
		lua["yukari"] = yukariAnimation;
		lua["chen"] = chenAnimation;
		lua["shangai"] = shangaiAnimation;

		//BOSS
		lua["boss"] = bossAnimation;

	}

};

#endif
#ifndef _PDATA_
#define _PDATA_

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "sol/sol.hpp"

#include "entities/Projectile.h"

//STRAIGHT BEAMS
const HitboxArgs BeamHitbox{ 36, 1024 };
const Quad BeamBlack{ 0, 0, 32, 32 };
const Quad BeamGray{ 32, 0, 32, 32 };
const Quad BeamDarkRed{ 64, 0, 32, 32 };
const Quad BeamRed{ 96, 0, 32, 32 };
const Quad BeamDarkMagenta{ 128, 0, 32, 32 };
const Quad BeamMagenta{ 160, 0, 32, 32 };
const Quad BeamDarkBlue{ 192, 0, 32, 32 };
const Quad BeamBlue{ 224, 0, 32, 32 };
const Quad BeamDarkCyan{ 256, 0, 32, 32 };
const Quad BeamCyan{ 288, 0, 32, 32 };
const Quad BeamDarkGreen{ 320, 0, 32, 32 };
const Quad BeamGreen{ 352, 0, 32, 32 };
const Quad BeamLime{ 384, 0, 32, 32 };
const Quad BeamDarkYellow{ 416, 0, 32, 32 };
const Quad BeamYellow{ 448, 0, 32, 32 };
const Quad BeamOrange{ 512, 0, 32, 32 };

//BULLET PROTOS

//ARROWS
const CommonFlags ArrowCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE | PFLAG_ROTATES_TOWARDS;
const HitboxArgs ArrowHitbox = { 40, 1024 };
const CommonArgs ArrowBlack{ ArrowCosmeticFlags, ArrowHitbox, {0, 32, 32, 32} };
const CommonArgs ArrowGray{ ArrowCosmeticFlags, ArrowHitbox, {32, 32, 32, 32} };
const CommonArgs ArrowDarkRed{ ArrowCosmeticFlags, ArrowHitbox, {64, 32, 32, 32} };
const CommonArgs ArrowRed{ ArrowCosmeticFlags, ArrowHitbox, {96, 32, 32, 32} };
const CommonArgs ArrowDarkMagenta{ ArrowCosmeticFlags, ArrowHitbox, {128, 32, 32, 32} };
const CommonArgs ArrowMagenta{ ArrowCosmeticFlags, ArrowHitbox, {160, 32, 32, 32} };
const CommonArgs ArrowDarkBlue{ ArrowCosmeticFlags, ArrowHitbox, {192, 32, 32, 32} };
const CommonArgs ArrowBlue{ ArrowCosmeticFlags, ArrowHitbox, {224, 32, 32, 32} };
const CommonArgs ArrowDarkCyan{ ArrowCosmeticFlags, ArrowHitbox, {256, 32, 32, 32} };
const CommonArgs ArrowCyan{ ArrowCosmeticFlags, ArrowHitbox, {288, 32, 32, 32} };
const CommonArgs ArrowDarkGreen{ ArrowCosmeticFlags, ArrowHitbox, {320, 32, 32, 32} };
const CommonArgs ArrowGreen{ ArrowCosmeticFlags, ArrowHitbox, {352, 32, 32, 32} };
const CommonArgs ArrowLime{ ArrowCosmeticFlags, ArrowHitbox, {384, 32, 32, 32} };
const CommonArgs ArrowDarkYellow{ ArrowCosmeticFlags, ArrowHitbox, {416, 32, 32, 32} };
const CommonArgs ArrowYellow{ ArrowCosmeticFlags, ArrowHitbox, {448, 32, 32, 32} };
const CommonArgs ArrowOrange{ ArrowCosmeticFlags, ArrowHitbox, {512, 32, 32, 32} };

//RINGED BULLET	
const CommonFlags RingedBulletCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE;
const HitboxArgs RingedBulletHitbox = { 74, 1024 };
const CommonArgs RingedBulletBlack{ RingedBulletCosmeticFlags, RingedBulletHitbox, {0, 64, 32, 32} };
const CommonArgs RingedBulletGray{ RingedBulletCosmeticFlags, RingedBulletHitbox, {32, 64, 32, 32} };
const CommonArgs RingedBulletDarkRed{ RingedBulletCosmeticFlags, RingedBulletHitbox, {64, 64, 32, 32} };
const CommonArgs RingedBulletRed{ RingedBulletCosmeticFlags, RingedBulletHitbox, {96, 64, 32, 32} };
const CommonArgs RingedBulletDarkMagenta{ RingedBulletCosmeticFlags, RingedBulletHitbox, {128, 64, 32, 32} };
const CommonArgs RingedBulletMagenta{ RingedBulletCosmeticFlags, RingedBulletHitbox, {160, 64, 32, 32} };
const CommonArgs RingedBulletDarkBlue{ RingedBulletCosmeticFlags, RingedBulletHitbox, {192, 64, 32, 32} };
const CommonArgs RingedBulletBlue{ RingedBulletCosmeticFlags, RingedBulletHitbox, {224, 64, 32, 32} };
const CommonArgs RingedBulletDarkCyan{ RingedBulletCosmeticFlags, RingedBulletHitbox, {256, 64, 32, 32} };
const CommonArgs RingedBulletCyan{ RingedBulletCosmeticFlags, RingedBulletHitbox, {288, 64, 32, 32} };
const CommonArgs RingedBulletDarkGreen{ RingedBulletCosmeticFlags, RingedBulletHitbox, {320, 64, 32, 32} };
const CommonArgs RingedBulletGreen{ RingedBulletCosmeticFlags, RingedBulletHitbox, {352, 64, 32, 32} };
const CommonArgs RingedBulletLime{ RingedBulletCosmeticFlags, RingedBulletHitbox, {384, 64, 32, 32} };
const CommonArgs RingedBulletDarkYellow{ RingedBulletCosmeticFlags, RingedBulletHitbox, {416, 64, 32, 32} };
const CommonArgs RingedBulletYellow{ RingedBulletCosmeticFlags, RingedBulletHitbox, {448, 64, 32, 32} };
const CommonArgs RingedBulletOrange{ RingedBulletCosmeticFlags, RingedBulletHitbox, {512, 64, 32, 32} };

//BULLET
const CommonFlags BulletCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE;
const HitboxArgs BulletHitbox = { 74, 1024 };
const CommonArgs BulletBlack{ BulletCosmeticFlags, BulletHitbox, {0, 96, 32, 32} };
const CommonArgs BulletGray{ BulletCosmeticFlags, BulletHitbox, {32, 96, 32, 32} };
const CommonArgs BulletDarkRed{ BulletCosmeticFlags, BulletHitbox, {64, 96, 32, 32} };
const CommonArgs BulletRed{ BulletCosmeticFlags, BulletHitbox, {96, 96, 32, 32} };
const CommonArgs BulletDarkMagenta{ BulletCosmeticFlags, BulletHitbox, {128, 96, 32, 32} };
const CommonArgs BulletMagenta{ BulletCosmeticFlags, BulletHitbox, {160, 96, 32, 32} };
const CommonArgs BulletDarkBlue{ BulletCosmeticFlags, BulletHitbox, {192, 96, 32, 32} };
const CommonArgs BulletBlue{ BulletCosmeticFlags, BulletHitbox, {224, 96, 32, 32} };
const CommonArgs BulletDarkCyan{ BulletCosmeticFlags, BulletHitbox, {256, 96, 32, 32} };
const CommonArgs BulletCyan{ BulletCosmeticFlags, BulletHitbox, {288, 96, 32, 32} };
const CommonArgs BulletDarkGreen{ BulletCosmeticFlags, BulletHitbox, {320, 96, 32, 32} };
const CommonArgs BulletGreen{ BulletCosmeticFlags, BulletHitbox, {352, 96, 32, 32} };
const CommonArgs BulletLime{ BulletCosmeticFlags, BulletHitbox, {384, 96, 32, 32} };
const CommonArgs BulletDarkYellow{ BulletCosmeticFlags, BulletHitbox, {416, 96, 32, 32} };
const CommonArgs BulletYellow{ BulletCosmeticFlags, BulletHitbox, {448, 96, 32, 32} };
const CommonArgs BulletOrange{ BulletCosmeticFlags, BulletHitbox, {512, 96, 32, 32} };

//KNIFE
const CommonFlags KnifeCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE | PFLAG_ROTATES_TOWARDS;
const HitboxArgs KnifeHitbox = { 160, 1024 };
const CommonArgs KnifeBlack{ KnifeCosmeticFlags, KnifeHitbox, {0, 160, 64, 64} };
const CommonArgs KnifeRed{ KnifeCosmeticFlags, KnifeHitbox, {64, 160, 64, 64} };
const CommonArgs KnifeMagenta{ KnifeCosmeticFlags, KnifeHitbox, {128, 160, 64, 64} };
const CommonArgs KnifeBlue{ KnifeCosmeticFlags, KnifeHitbox, {192, 160, 64, 64} };
const CommonArgs KnifeCyan{ KnifeCosmeticFlags, KnifeHitbox, {256, 160, 64, 64} };
const CommonArgs KnifeGreen{ KnifeCosmeticFlags, KnifeHitbox, {320, 160, 64, 64} };
const CommonArgs KnifeYellow{ KnifeCosmeticFlags, KnifeHitbox, {384, 160, 64, 64} };
const CommonArgs KnifeGray{ KnifeCosmeticFlags, KnifeHitbox, {448, 160, 64, 64} };

//BIG_BULLET
const CommonFlags BigBulletCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE;
const HitboxArgs BigBulletHitbox = { 305, 1024 };
const CommonArgs BigBulletBlack{ BigBulletCosmeticFlags, BigBulletHitbox, {0, 224, 64, 64} };
const CommonArgs BigBulletRed{ BigBulletCosmeticFlags, BigBulletHitbox, {64, 224, 64, 64} };
const CommonArgs BigBulletMagenta{ BigBulletCosmeticFlags, BigBulletHitbox, {128, 224, 64, 64} };
const CommonArgs BigBulletBlue{ BigBulletCosmeticFlags, BigBulletHitbox, {192, 224, 64, 64} };
const CommonArgs BigBulletCyan{ BigBulletCosmeticFlags, BigBulletHitbox, {256, 224, 64, 64} };
const CommonArgs BigBulletGreen{ BigBulletCosmeticFlags, BigBulletHitbox, {320, 224, 64, 64} };
const CommonArgs BigBulletYellow{ BigBulletCosmeticFlags, BigBulletHitbox, {384, 224, 64, 64} };
const CommonArgs BigBulletGray{ BigBulletCosmeticFlags, BigBulletHitbox, {448, 224, 64, 64} };

//HUGE_BULLET
const CommonFlags HugeBulletCosmeticFlags = PFLAG_SPAWN_PARTICLE | PFLAG_DESTROY_PARTICLE;
const HitboxArgs HugeBulletHitbox = { 1296, 1024 }; //MAYBE CHANGE LATER
const CommonArgs HugeBulletRed{ HugeBulletCosmeticFlags, HugeBulletHitbox, {512, 192, 128, 128} };
const CommonArgs HugeBulletBlue{ HugeBulletCosmeticFlags, HugeBulletHitbox, {640, 192, 128, 128} };
const CommonArgs HugeBulletGreen{ HugeBulletCosmeticFlags, HugeBulletHitbox, {768, 192, 128, 128} };
const CommonArgs HugeBulletYellow{ HugeBulletCosmeticFlags, HugeBulletHitbox, {896, 192, 128, 128} };

#endif
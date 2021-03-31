#ifndef _ANIMDATA_
#define _ANIMDATA_

#include "video/Mesh.h"
#include "entities/Animation.h"
#include "entities/Hitbox.h"

//PLAYER
const HitboxArgs playerHitbox = { 36, 1024 };
const Quad playerDrawQuad = { 0, 0, 64, 96 };
const ipair playerAnimStates = { 7, 1 };
const unsigned int playerFrameTime = 6;
const AnimationArgs playerAnimation = { playerHitbox ,playerDrawQuad, playerAnimStates, playerFrameTime, { 512, 736, 512, 288 } };

//ENEMY
const HitboxArgs enemyHitbox = { 64, 1024 };
const Quad enemyDrawQuad = { 0, 0, 32, 32 };
const ipair enemyAnimStates = { 3, 1 };
const unsigned int enemyFrameTime = 10;
const AnimationArgs ranAnimation = { enemyHitbox ,enemyDrawQuad, enemyAnimStates, enemyFrameTime, { 512, 544, 96, 96 } };
const AnimationArgs yukariAnimation = { enemyHitbox ,enemyDrawQuad, enemyAnimStates, enemyFrameTime, { 608, 544, 96, 96 } };
const AnimationArgs chenAnimation = { enemyHitbox ,enemyDrawQuad, enemyAnimStates, enemyFrameTime, { 704, 544, 96, 96 } };
const AnimationArgs shangaiAnimation = { enemyHitbox ,enemyDrawQuad, enemyAnimStates, enemyFrameTime, { 800, 544, 96, 96 } };


//BOSS
const HitboxArgs bossHitbox = { 1024, 1024 };
const Quad bossDrawQuad = { 0, 0, 128, 128 };
const ipair bossAnimStates = { 3, 1 };
const unsigned int bossFrameTime = 6;
const AnimationArgs bossAnimation = { bossHitbox, bossDrawQuad, bossAnimStates, bossFrameTime, { 0, 0, 128, 128 } };

#endif
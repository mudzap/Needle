#include "Script.h"

void Script::CreateUsertypes(sol::state& lua) {

    sol::usertype<Quad> quadUT = lua.new_usertype<Quad>("quad",
        sol::constructors <Quad>(),
        "x", &Quad::x,
        "y", &Quad::y,
        "w", &Quad::w,
        "h", &Quad::h
        );

    sol::usertype<Complex> cmplxUT = lua.new_usertype<Complex>("cmplx",
        sol::constructors <Complex(float, float)>(),
        "x", &Complex::x,
        "y", &Complex::y,
        "dot", &Complex::Dot,
        "conj", &Complex::Conjugate,
        "abs", &Complex::Abs,
        "sqrdabs", &Complex::SquaredAbs,
        "angle", &Complex::Angle,
        "rot", &Complex::FromAngle,
        "norm", &Complex::Normal,
        "proj", &Complex::Projection
        );

    sol::usertype<TransformArgs> transformUT = lua.new_usertype<TransformArgs>("transform",
        sol::constructors <TransformArgs>(),
        "pos", &TransformArgs::position,
        "vel", &TransformArgs::velocity,
        "accel", &TransformArgs::acceleration,
        "rot", &TransformArgs::rotation,
        "rot_d", &TransformArgs::rotationDelta,
        "vis_rot", &TransformArgs::visualRotation
        );

    sol::usertype<HitboxArgs> hitboxUT = lua.new_usertype<HitboxArgs>("hargs",
        sol::constructors <HitboxArgs>(),
        "hit", &HitboxArgs::squaredHitRadius,
        "hurt", &HitboxArgs::squaredHurtRadius
        );

    sol::usertype<CommonFlags> commonFlagsUT = lua.new_usertype<CommonFlags>("cflag",
        sol::constructors <CommonFlags>()
        );
    lua["C_IS_SAKURA"] = PFLAG_IS_SAKURA;
    lua["C_SPAWN_PARTICLE"] = PFLAG_SPAWN_PARTICLE;
    lua["C_DESTROY_PARTICLE"] = PFLAG_DESTROY_PARTICLE;
    lua["C_ROTATES_TO"] = PFLAG_ROTATES_TOWARDS;
    lua["C_ROTATES"] = PFLAG_ROTATES_PASSIVELY;
    sol::usertype<CommonArgs> commonArgsUT = lua.new_usertype<CommonArgs>("cargs",
        sol::constructors <CommonArgs>(),
        "flags", &CommonArgs::flags,
        "hitbox", &CommonArgs::hitbox,
        "quad", &CommonArgs::quad
        );

    sol::usertype<ipair> ipairUT = lua.new_usertype<ipair>("ipair",
        sol::constructors <ipair>(),
        "x", &ipair::x,
        "y", &ipair::y
        );

    sol::usertype<AnimationArgs> animationArgsUT = lua.new_usertype<AnimationArgs>("anim_args",
        sol::constructors <AnimationArgs>(),
        "hitbox", &AnimationArgs::hitbox,
        "draw_quad", &AnimationArgs::drawQuad,
        "states", &AnimationArgs::states,
        "frames", &AnimationArgs::frameTime,
        "spritesheet_quad", &AnimationArgs::spriteSheetQuad
        );

    sol::usertype<ProjFlags> projFlagsUT = lua.new_usertype<ProjFlags>("pflag",
        sol::constructors <ProjFlags>()
        );
    lua["P_AIMED_AT_PLAYER"] = PFLAG_AIMED_AT_PLAYER;
    lua["P_FOLLOW_PLAYER"] = PFLAG_FOLLOW_PLAYER;
    lua["P_GRAZEABLE"] = PFLAG_GRAZEABLE;
    lua["P_ENEMY"] = PFLAG_ENEMY;
    lua["P_PLAYER"] = PFLAG_PLAYER;
    lua["P_DEAD"] = PFLAG_DEAD;
    lua["P_CANCELLABLE"] = PFLAG_CANCELLABLE;
    sol::usertype<ProjArgs> projArgsUT = lua.new_usertype<ProjArgs>("pargs",
        sol::constructors <ProjArgs>(),
        "flags", &ProjArgs::flags,
        "vel", &ProjArgs::initialVel,
        "accel", &ProjArgs::acceleration,
        "sng_freq", &ProjArgs::stopandgoFrequency,
        "sine_freq", &ProjArgs::sineFrequency,
        "zigzag_freq", &ProjArgs::zigzagFrequency,
        "exp", &ProjArgs::decayRate,
        "sine_amp", &ProjArgs::sineAmplitude,
        "zigzag_amp", &ProjArgs::zigzagAmplitude,
        "angle", &ProjArgs::angle,
        "andle_d", &ProjArgs::angleDelta,
        "gravity", &ProjArgs::gravityConstant
        );

    sol::usertype<Projectile> projUT = lua.new_usertype<Projectile>("proj",
        sol::constructors <Projectile(ProjArgs, CommonArgs)>(),
        "proj_args", &Projectile::projectile,
        "common_args", &Projectile::common
        );

    sol::usertype<Enemy> enemyUT = lua.new_usertype<Enemy>("enemy",
        sol::constructors<Enemy(Complex, AnimationArgs, int)>(),
        "recreateTransform", &Enemy::RecreateTransform,
        "recreatePos", &Enemy::RecreatePosition,
        "setPos", &Enemy::SetPosition,
        "destroy", &Enemy::Destroy,
        "moveToXY", &Enemy::MoveTowards1,
        "moveToMA", &Enemy::MoveTowards2,
        "springToXY", &Enemy::SpringTowards1,
        "springToMA", &Enemy::SpringTowards2,
        "slowToXY", &Enemy::SlowTowards1,
        "slowToMA", &Enemy::SlowTowards2,
        "stopFull", &Enemy::Stop1,
        "stopBrake", &Enemy::Stop2,
        "drift", &Enemy::Drift1,
        "driftBrake", &Enemy::Drift2,
        "circle", &Enemy::CircleAround,
        "addSpawner", &Enemy::AddSpawner,
        "addManual", &Enemy::AddSpawnerManual,
        "addConstant", &Enemy::AddSpawnerConstant,
        "addBarrage", &Enemy::AddSpawnerBarrage,
        "addRandom", &Enemy::AddSpawnerRandom
        );

    sol::usertype<KeyContainer<Enemy>> eKeyUT = lua.new_usertype<KeyContainer<Enemy>>("enemyKey",
        sol::constructors <KeyContainer<Enemy>()>(),
        "enemy_p", &KeyContainer<Enemy>::type_ptr,
        "id", &KeyContainer<Enemy>::ID
    );

    sol::usertype<KeyContainer<Spawner>> sKeyUT = lua.new_usertype<KeyContainer<Spawner>>("spawnerKey",
        sol::constructors <KeyContainer<Spawner>()>(),
        "spawner_p", &KeyContainer<Spawner>::type_ptr,
        "id", &KeyContainer<Spawner>::ID
    );

    sol::usertype<EnemPool<MAX_ENEMIES>> enemyPoolUT = lua.new_usertype<EnemPool<MAX_ENEMIES>>("enemyPool",
        sol::constructors<>(),
        "getEnemy", &EnemPool<MAX_ENEMIES>::GetHandle,
        "getEnemies", &EnemPool<MAX_ENEMIES>::GetHandles,
        "getEnemy", &EnemPool<MAX_ENEMIES>::FreeHandle,
        "getEnemies", &EnemPool<MAX_ENEMIES>::FreeHandles,
        "destroyAllEnemies", &EnemPool<MAX_ENEMIES>::FreeAllHandles
    );


    sol::usertype<EnemyPool> poolUT = lua.new_usertype<EnemyPool>("pool",
        sol::constructors<EnemyPool()>(),
        "createEnemy", &EnemyPool::CreateEnemyComplex,
        "destroyEnemy", &EnemyPool::DestroyEnemy,
        "destroyAll", &EnemyPool::DestroyAllEnemies
        );

    sol::usertype<SpawnerArgs> spawnerUT = lua.new_usertype<SpawnerArgs>("spawnerArgs",
        sol::constructors<SpawnerArgs()>(),
        "angle", &SpawnerArgs::angle,
        "angle_d", &SpawnerArgs::angleDelta,
        "angle_d2", &SpawnerArgs::angleAccel,
        "loop_start", &SpawnerArgs::loopingConeStart,
        "loop_end", &SpawnerArgs::loopingConeEnd
        );

    sol::usertype<ConstantArgs> constantUT = lua.new_usertype<ConstantArgs>("constantArgs",
        sol::constructors<ConstantArgs()>(),
        "base_proj", &ConstantArgs::baseProjectile,
        "aimed", &ConstantArgs::aimed,
        "initial_angle", &ConstantArgs::initialBulletAngle,
        "arrays", &ConstantArgs::bulletArrays,
        "array_angle", &ConstantArgs::angleBetweenArray,
        "bullets", &ConstantArgs::bulletPerArray,
        "bullet_angle", &ConstantArgs::angleBetweenBullets,
        "radius", &ConstantArgs::bulletOffsetRadius,
        "start_time", &ConstantArgs::startTimer,
        "stop_time", &ConstantArgs::stopTimer,
        "shot_time", &ConstantArgs::bulletShotTimer,
        "perarray_accel", &ConstantArgs::perArrayAccel,
        "perbullet_accel", &ConstantArgs::perBulletAccel
        );

    sol::usertype<BarrageArgs> barrageUT = lua.new_usertype<BarrageArgs>("barrageArgs",
        sol::constructors<BarrageArgs()>(),
        "base_proj", &BarrageArgs::baseProjectile,
        "aimed", &BarrageArgs::aimed,
        "initial_angle", &BarrageArgs::initialBulletAngle,
        "arrays", &BarrageArgs::bulletArrays,
        "array_angle", &BarrageArgs::angleBetweenArray,
        "bullets", &BarrageArgs::bulletPerArray,
        "bullet_angle", &BarrageArgs::angleBetweenBullets,
        "radius", &BarrageArgs::bulletOffsetRadius,
        "start_time", &BarrageArgs::startTimer,
        "stop_time", &BarrageArgs::stopTimer,
        "shot_time", &BarrageArgs::bulletShotTimer,
        "perarray_accel", &BarrageArgs::perArrayAccel,
        "perbullet_accel", &BarrageArgs::perBulletAccel,
        "perbarrage_accel", &BarrageArgs::perBarrageAccel,
        "barrage_time", &BarrageArgs::barrageTimer,
        "barrage_bullets", &BarrageArgs::bulletsPerBarrage
        );

    sol::usertype<RandomArgs> randomUT = lua.new_usertype<RandomArgs>("randomArgs",
        sol::constructors<RandomArgs()>(),
        "base_proj", &RandomArgs::baseProjectile,
        "aimed", &RandomArgs::aimed,
        "initial_angle", &RandomArgs::initialBulletAngle,
        "arrays", &RandomArgs::bulletArrays,
        "array_angle", &RandomArgs::angleBetweenArray,
        "bullets", &RandomArgs::bulletPerArray,
        "radius", &RandomArgs::bulletOffsetRadius,
        "start_time", &RandomArgs::startTimer,
        "stop_time", &RandomArgs::stopTimer,
        "shot_time", &RandomArgs::bulletShotTimer,
        "perbullet_accel", &RandomArgs::perBulletAccel,
        "speed_range", &RandomArgs::speedRange,
        "accel_range", &RandomArgs::accelRange,
        "angle_range", &RandomArgs::coneRange
        );

    sol::usertype<Camera> cameraUT = lua.new_usertype<Camera>("camera",
        sol::constructors<Camera()>(),
        "set_pos", &Camera::SetPosition,
        "lerp", &Camera::LinearInterpTo,
        "cerp", &Camera::CubicInterpBoth,
        "cerp_in", &Camera::CubicInterpStart,
        "cerp_out", &Camera::CubicInterpEnd,
        "qerp_in", &Camera::QuadraticInterpStart,
        "qerp_out", &Camera::QuadraticInterpEnd,
        "stop", &Camera::CameraStop,
        "quad_bezier", &Camera::QuadraticBezier,
        "cubic_bezier", &Camera::CubicBezier,
        "set_angle", &Camera::SetAngle,
        "a_lerp", &Camera::LinearInterpTo,
        "a_cerp", &Camera::CubicInterpBoth,
        "a_cerp_in", &Camera::CubicInterpStart,
        "a_cerp_out", &Camera::CubicInterpEnd,
        "a_qerp_in", &Camera::QuadraticInterpStart,
        "a_qerp_out", &Camera::QuadraticInterpEnd,
        "a_quad_bezier", &Camera::QuadraticBezier,
        "a_cubic_bezier", &Camera::CubicBezier,
        "fix_on", & Camera::CameraFixViewOn,
        "follow", & Camera::CameraFollow,
        "follow_yaw", &Camera::CameraFollowYaw,
        "follow_pitch", &Camera::CameraFollowPitch,
        "float", & Camera::CameraFloat,
        "a_stop", &Camera::CameraFloat,         //ALIASES
        "a_fix_on", &Camera::CameraFixViewOn,   //ALIASES
        "a_follow", &Camera::CameraFollow       //ALIASES
        );

}
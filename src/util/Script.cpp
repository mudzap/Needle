#include "Script.h"

void Script::CreateUsertypes(sol::state& lua) {

    sol::usertype<Quad> quadUT = lua.new_usertype<Quad>("Quad",
        sol::constructors <Quad>(),
        "x", &Quad::x,
        "y", &Quad::y,
        "w", &Quad::w,
        "h", &Quad::h
    );

    sol::usertype<Complex> cmplxUT = lua.new_usertype<Complex>("Cmplx",
        sol::constructors <Complex(float, float)>(),
        "x", &Complex::x,
        "y", &Complex::y,
        "dot", &Complex::Dot,
        "conj", &Complex::Conjugate,
        "abs", &Complex::Abs,
        "sqrd_abs", &Complex::SquaredAbs,
        "angle", &Complex::Angle,
        "rot", &Complex::FromAngle,
        "norm", &Complex::Normal,
        "proj", &Complex::Projection
    );

    sol::usertype<TransformArgs> transformUT = lua.new_usertype<TransformArgs>("Transform",
        sol::constructors <TransformArgs>(),
        "pos", &TransformArgs::position,
        "vel", &TransformArgs::velocity,
        "accel", &TransformArgs::acceleration,
        "rot", &TransformArgs::rotation,
        "rot_d", &TransformArgs::rotationDelta,
        "vis_rot", &TransformArgs::visualRotation
    );

    sol::usertype<HitboxArgs> hitboxUT = lua.new_usertype<HitboxArgs>("Hitbox_Args",
        sol::constructors <HitboxArgs>(),
        "hit", &HitboxArgs::squaredHitRadius,
        "hurt", &HitboxArgs::squaredHurtRadius
    );

    sol::usertype<CommonFlags> commonFlagsUT = lua.new_usertype<CommonFlags>("Common_Ent_Flag",
        sol::constructors <CommonFlags>()
    );
    lua["C_IS_SAKURA"] = PFLAG_IS_SAKURA;
    lua["C_SPAWN_PARTICLE"] = PFLAG_SPAWN_PARTICLE;
    lua["C_DESTROY_PARTICLE"] = PFLAG_DESTROY_PARTICLE;
    lua["C_ROTATES_TO"] = PFLAG_ROTATES_TOWARDS;
    lua["C_ROTATES"] = PFLAG_ROTATES_PASSIVELY;
    sol::usertype<CommonArgs> commonArgsUT = lua.new_usertype<CommonArgs>("Common_Ent_Args",
        sol::constructors <CommonArgs>(),
        "flags", &CommonArgs::flags,
        "hitbox", &CommonArgs::hitbox,
        "quad", &CommonArgs::quad
    );

    sol::usertype<ipair> ipairUT = lua.new_usertype<ipair>("Ipair",
        sol::constructors <ipair>(),
        "x", &ipair::x,
        "y", &ipair::y
    );

    sol::usertype<AnimationArgs> animationArgsUT = lua.new_usertype<AnimationArgs>("Anim_Args",
        sol::constructors <AnimationArgs>(),
        "hitbox", &AnimationArgs::hitbox,
        "draw_quad", &AnimationArgs::drawQuad,
        "states", &AnimationArgs::states,
        "frames", &AnimationArgs::frameTime,
        "spritesheet_quad", &AnimationArgs::spriteSheetQuad
    );

    sol::usertype<ProjFlags> projFlagsUT = lua.new_usertype<ProjFlags>("Proj_Flag",
        sol::constructors <ProjFlags>()
    );
    lua["P_AIMED_AT_PLAYER"] = PFLAG_AIMED_AT_PLAYER;
    lua["P_FOLLOW_PLAYER"] = PFLAG_FOLLOW_PLAYER;
    lua["P_GRAZEABLE"] = PFLAG_GRAZEABLE;
    lua["P_ENEMY"] = PFLAG_ENEMY;
    lua["P_PLAYER"] = PFLAG_PLAYER;
    lua["P_DEAD"] = PFLAG_DEAD;
    lua["P_CANCELLABLE"] = PFLAG_CANCELLABLE;

    sol::usertype<ProjArgs> projArgsUT = lua.new_usertype<ProjArgs>("Proj_Args",
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

    sol::usertype<Projectile> projUT = lua.new_usertype<Projectile>("Proj",
        sol::constructors <Projectile(ProjArgs, CommonArgs)>(),
        "proj_args", &Projectile::projectile,
        "common_args", &Projectile::common
    );

    sol::usertype< KeyContainer<Enemy> > eKeyUT = lua.new_usertype< KeyContainer<Enemy> >("Enemy_Key",
        sol::constructors < KeyContainer<Enemy> >(),
        "enemy_ptr", &KeyContainer<Enemy>::type_ptr,
        "enemy_id", &KeyContainer<Enemy>::ID
    );

    sol::usertype<KeyContainer<Spawner>> sKeyUT = lua.new_usertype< KeyContainer<Spawner> >("Spawner_Key",
        sol::constructors < KeyContainer<Spawner> >(),
        "spawner_ptr", &KeyContainer<Spawner>::type_ptr,
        "spawner_id", &KeyContainer<Spawner>::ID
    );

    sol::usertype<Enemy> enemyUT = lua.new_usertype<Enemy>("Enemy",
        sol::constructors<Enemy()>(),
        "recreate_transform", &Enemy::RecreateTransform,
        "recreate_pos", &Enemy::RecreatePosition,
        "set_pos", &Enemy::SetPosition,
        "destroy", &Enemy::Destroy,
        "move_to_XY", &Enemy::MoveTowards1,
        "move_to_MA", &Enemy::MoveTowards2,
        "spring_to_XY", &Enemy::SpringTowards1,
        "spring_to_MA", &Enemy::SpringTowards2,
        "slow_to_XY", &Enemy::SlowTowards1,
        "slow_to_MA", &Enemy::SlowTowards2,
        "stop_full", &Enemy::Stop1,
        "stop_brake", &Enemy::Stop2,
        "drift", &Enemy::Drift1,
        "drift_brake", &Enemy::Drift2,
        "circle", &Enemy::CircleAround,
        "add_spawner", &Enemy::AddChild
    );

    sol::usertype<Spawner> spawnerUT = lua.new_usertype<Spawner>("Spawner",
        sol::constructors<Spawner()>(),
        "recreate_manual", &Spawner::RecreateManual,
        "recreate_constant", &Spawner::RecreateConstant,
        "recreate_barrage", &Spawner::RecreateBarrage,
        "recreate_random", &Spawner::RecreateRandom,
        "recreate_empty", &Spawner::RecreateEmpty,
        "sleep", &Spawner::Sleep,
        "wake_up", &Spawner::WakeUp
    );

    sol::usertype<EnemPool<MAX_ENEMIES>> enemyPoolUT = lua.new_usertype<EnemPool<MAX_ENEMIES>>("Enemy_Pool",
        sol::constructors<>(),
        "get_enemy", &EnemPool<MAX_ENEMIES>::GetHandle,
        "get_enemies", &EnemPool<MAX_ENEMIES>::GetHandles,
        "kill_enemy", &EnemPool<MAX_ENEMIES>::FreeHandle,
        "kill_enemies", &EnemPool<MAX_ENEMIES>::FreeHandles,
        "destroy_all", &EnemPool<MAX_ENEMIES>::FreeAllHandles
    );

    sol::usertype<Pool<Spawner, MAX_SPAWNERS>> spawnerPoolUT = lua.new_usertype<Pool<Spawner, MAX_SPAWNERS>>("Spawner_Pool",
        sol::constructors<>(),
        "get_spawner", &Pool<Spawner, MAX_SPAWNERS>::GetHandle,
        "get_spawners", &Pool<Spawner, MAX_SPAWNERS>::GetHandles,
        "kill_spawner", &Pool<Spawner, MAX_SPAWNERS>::FreeHandle,
        "kill_spawners", &Pool<Spawner, MAX_SPAWNERS>::FreeHandles,
        "destroy_all", &Pool<Spawner, MAX_SPAWNERS>::FreeAllHandles
    );

    sol::usertype<SpawnerArgs> spawnerArgsUT = lua.new_usertype<SpawnerArgs>("Spawner_Args",
        sol::constructors<SpawnerArgs()>(),
        "angle", &SpawnerArgs::angle,
        "angle_d", &SpawnerArgs::angleDelta,
        "angle_d2", &SpawnerArgs::angleAccel,
        "loop_start", &SpawnerArgs::loopingConeStart,
        "loop_end", &SpawnerArgs::loopingConeEnd
    );

    sol::usertype<ConstantArgs> constantUT = lua.new_usertype<ConstantArgs>("Constant_Args",
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

    sol::usertype<BarrageArgs> barrageUT = lua.new_usertype<BarrageArgs>("Barrage_Args",
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

    sol::usertype<RandomArgs> randomUT = lua.new_usertype<RandomArgs>("Random_Args",
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

    sol::usertype<Camera> cameraUT = lua.new_usertype<Camera>("Camera",
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

--function doNext()
--	print("LUA: resuming coroutine")
--	coroutine.resume(movement)
--end

--movement = coroutine.create(function()
--	print("LUA: yielding coroutine")
--	coroutine.yield()
--end)


function main()

	print("LUA: Setting spawner args")

	-- SPAWNER ARGS
	test_spawner = Spawner_Args.new()
	test_spawner['angle'] = 0
	test_spawner['angle_d'] = 2.0

	print("LUA: Setting projectile args")

	-- PROJ ARGS, AND PROJECTILE
	temp_pargs = Proj_Args.new()
	temp_pargs['vel'] = Cmplx.new(3, 0)
	--testBullet2 = Proj.new(temp_pargs, hugebullet_red)
	test_bullet = Proj.new(temp_pargs, knife_magenta)
	test_bullet2 = Proj.new(temp_pargs, knife_green)

	print("LUA: creating spawner pattern")

	-- PATTERN, CONST, BARR, RANDOM...
	test_pattern = Constant_Args.new()
	test_pattern['base_proj'] = test_bullet
	--test_pattern['arrays'] = 72
	--test_pattern['array_angle'] = 5
	--test_pattern['shot_time'] = 1
	test_pattern['arrays'] = 8
	test_pattern['array_angle'] = 45
	test_pattern['shot_time'] = 600

	test_pattern2 = Constant_Args.new()
	test_pattern2['base_proj'] = test_bullet2
	--test_pattern2['arrays'] = 72
	--test_pattern2['array_angle'] = 5
	--test_pattern2['shot_time'] = 1
	test_pattern2['arrays'] = 8
	test_pattern2['array_angle'] = 45
	test_pattern2['shot_time'] = 600

	print("LUA: getting spawner keys:")

	s_key_1 = spawners:get_spawner()
	s_key_2 = spawners:get_spawner()
	s_key_3 = spawners:get_spawner()
	s_key_4 = spawners:get_spawner()

	print("LUA:", s_key_1, s_key_2, s_key_3, s_key_4)

	s_key_1.spawner_ptr:recreate_constant(Cmplx.new(100, 0), 100, test_spawner, test_pattern)
	s_key_2.spawner_ptr:recreate_constant(Cmplx.new(-100, 0), 100, test_spawner, test_pattern)
	--s_key_3.spawner_ptr:recreate_constant(test_pattern2)
	--s_key_4.spawner_ptr:recreate_constant(test_pattern2)

	-- ENEMY CREATION
	print("LUA: Creating enemy")
	e_key_1 = enemies:get_enemy()
	print("LUA: Adding spawners")
	e_key_1.enemy_ptr:add_spawner(s_key_1)
	e_key_1.enemy_ptr:add_spawner(s_key_2)
	print("LUA: Setting enemy position", e_key_1, e_key_1.enemy_ptr)
	e_key_1.enemy_ptr:recreate_pos(Cmplx.new(-200, 0), ran, 10)
	--testEnemy = enemies:create_enemy(cmplx.new(-200, 0), ran, 10)
	--testEnemy:addConstant(cmplx.new(100, 0), 100, testSpawner, testPattern2)
	--testEnemy:addConstant(cmplx.new(-100, 0), 100, testSpawner, testPattern2)

	--e_key_2.enemy_ptr
	--testEnemy2 = enemies:createEnemy(cmplx.new(200, 0), chen, 10)
	--testEnemy2:addConstant(cmplx.new(100, 0), 100, testSpawner, testPattern)
	--testEnemy2:addConstant(cmplx.new(-100, 0), 100, testSpawner, testPattern)

	-- CAMERA
	print("LUA: init camera")
	cam:set_pos(-64, 20, 0)
	cam:follow_yaw(5)
	cam:lerp(240, 80, 40, 0)

	print("LUA: programming camera")
	do_after_frames(240, function(self) cam:quad_bezier(160,	24, 12, 0,		0, 12, 24) end)
	do_after_frames(400, function(self) cam:lerp(240, 0, 40, 80) end)

	-- TIMING
	--doAfterFrames(0, function() testEnemy2:slowToXY(cmplx.new(0, 150), 0.02) end)
	--doAfterFrames(180, function() testEnemy2:slowToXY(cmplx.new(0, -150), 0.02) end)
	--doAfterFrames(600, function() enemies:destroyEnemy(testEnemy2) end)

	print("LUA: programming enemy script")
	e_key_1.enemy_ptr:slow_to_XY(Cmplx.new(-150, -50), 0.06)
	do_after_frames(60, function(self) e_key_1.enemy_ptr:slow_to_XY(Cmplx.new(50, 300), 0.06) end)	
	do_after_frames(120, function(self) e_key_1.enemy_ptr:move_to_XY(Cmplx.new(100, -250), 6, 0) end)
	do_after_frames(130, function(self) e_key_1.enemy_ptr:circle(Cmplx.new(200, 50)) end)
	do_after_frames(220, function(self) e_key_1.enemy_ptr:drift_brake(0.95) end)
	do_after_frames(280, function(self) e_key_1.enemy_ptr:slow_to_XY(Cmplx.new(-350, -300), 0.04) end)

	print("LUA: programming enemy destruction")
	do_after_frames(600, function(self) enemies:destroy_enemy(e_key_1.enemy_id) end)

end

--start script
--print("LUA: script start\n")
--main()

--Add:	Bullet instantiation
--		Bullet modifying
--		Laser control
--		Restart spawner
--		Spawner movement
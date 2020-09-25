
function doNext()
	coroutine.resume(movement)
end

movement = coroutine.create(function()
	coroutine.yield()
end)


function main()

	-- SPAWNER ARGS
	testSpawner = spawnerArgs.new()
	testSpawner['angle'] = 0
	testSpawner['angle_d'] = 0.5

	-- PROJ ARGS, AND PROJECTILE
	temp_pargs = pargs.new()
	temp_pargs['vel'] = cmplx.new(3, 0)
	testBullet = proj.new(temp_pargs, knife_green)
	testBullet2 = proj.new(temp_pargs, knife_magenta)


	-- PATTERN, CONST, BARR, RANDOM...
	testPattern = constantArgs.new()
	testPattern['base_proj'] = testBullet
	testPattern['arrays'] = 10
	testPattern['array_angle'] = 36
	testPattern['shot_time'] = 500

	testPattern2 = constantArgs.new()
	testPattern2['base_proj'] = testBullet2
	testPattern2['arrays'] = 10
	testPattern2['array_angle'] = 36
	testPattern2['shot_time'] = 500


	-- ENEMY CREATION
	testEnemy = enemies:createEnemy(cmplx.new(-200, 0), ran)
	testEnemy:addConstant(cmplx.new(100, 0), 100, testSpawner, testPattern2)
	testEnemy:addConstant(cmplx.new(-100, 0), 100, testSpawner, testPattern2)

	--testEnemy2 = enemies:createEnemy(cmplx.new(200, 0), chen)
	--testEnemy2:addConstant(cmplx.new(100, 0), 100, testSpawner, testPattern)
	--testEnemy2:addConstant(cmplx.new(-100, 0), 100, testSpawner, testPattern)

	-- CAMERA
	cam:set_pos(-64, 20, 0)
	cam:follow_yaw(5)
	cam:lerp(240, 80, 40, 0)
	doAfterFrames(240, function() cam:quad_bezier(160,	24, 12, 0,		0, 12, 24) end)
	doAfterFrames(400, function() cam:lerp(240, 0, 40, 80) end)

	-- TIMING
	--doAfterFrames(0, function() testEnemy2:slowToXY(cmplx.new(0, 150), 0.02) end)
	--doAfterFrames(180, function() testEnemy2:slowToXY(cmplx.new(0, -150), 0.02) end)
	--doAfterFrames(300, function() enemies:destroyEnemy(testEnemy2) end)

	doAfterFrames(0, function() testEnemy:slowToXY(cmplx.new(-150, -50), 0.06) end)
	doAfterFrames(60, function() testEnemy:slowToXY(cmplx.new(50, 300), 0.06) end)
	doAfterFrames(120, function() testEnemy:moveToXY(cmplx.new(100, -250), 6, 0) end)
	doAfterFrames(130, function() testEnemy:circle(cmplx.new(200, 50)) end)
	doAfterFrames(220, function() testEnemy:driftBrake(0.95) end)
	doAfterFrames(280, function() testEnemy:slowToXY(cmplx.new(-350, -300), 0.04) end)
	doAfterFrames(300, function() enemies:destroyEnemy(testEnemy) end)

end

--start script
main()

--Add:	Bullet instantiation
--		Bullet modifying
--		Laser control
--		Restart spawner
--		Spawner movement
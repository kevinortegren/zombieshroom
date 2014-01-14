ACTION_CREATE = 0;
ACTION_COLLIDE = 1;

--Set table name
AbilityTest = {};

function AbilityTest.Process (action, ...)
	
	local args = table.pack(...);

	if action == ACTION_CREATE then
		AbilityTest.OnCreate();
	elseif action == ACTION_COLLIDE then
		AbilityTest.OnCollide(args);
	end
	
end

function AbilityTest.OnCreate ()
		local playerEnt = Entity.GetEntityByTag("Player");
		local frontVec  	= Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetOrient():GetFront();
		local handle = playerEnt:GetCollision():GetHandle();
		playerEnt:GetPhysics():KnockBack(handle, Vec3.New(0,0.1,0)+frontVec, 100);
	
end

function AbilityTest.OnCollide (args)
	
end

function AbilityTest.OnDestroy ()

end
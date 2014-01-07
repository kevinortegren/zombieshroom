ACTION_CREATE = 0;
ACTION_COLLIDE = 1;

Player = {}

function Player.Process (action, ...)
	
	local args = table.pack(...);

	if action == ACTION_CREATE then
		Player.OnCreate();
	elseif action == ACTION_COLLIDE then
		Player.OnCollide(args);
	end
	
end

function Player.OnCreate ()


end


function Player.OnCollide (args)
	if args[1] ~= 2 then
		local playerEnt = Entity.GetEntityByTag("Player");
		local handle = playerEnt:GetCollision():GetHandle();
		playerEnt:GetPhysics():KnockBack(handle, Vec3.New(0,1,0), 10);
	end
end

function Player.OnDestroy ()
	
end

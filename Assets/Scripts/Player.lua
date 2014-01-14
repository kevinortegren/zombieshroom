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

end

function Player.OnDestroy ()
	
end

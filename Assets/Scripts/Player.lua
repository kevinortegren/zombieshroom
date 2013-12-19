
Player = {}

function Player.Process (action, ...)

	local arg = table.pack(...)

	if action == 1 then

		if arg[1] ~= 3 then
			--print("Player collided with " .. arg[1])
		end
	end

end

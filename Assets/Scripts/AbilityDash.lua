ACTION_CREATE = 0;
ACTION_COLLIDE = 1;

--Set table name
AbilityDash = {};
AbilityDash.cooldown = 3;
AbilityDash.chargeTime = 0.0;
AbilityDash.channelingTime = 0.0;
AbilityDash.power = 50;

function AbilityDash.ChargeDone(time, userId, actionId)
end

function AbilityDash.ChannelingDone(time, userId, actionId)
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local aimingEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1);
	local frontVec  = aimingEnt:GetTransformation():GetOrient():GetFront();

	local handle = playerEnt:GetCollision():GetHandle();
	playerEnt:GetPhysics():KnockBack(handle, Vec3.New(0,0.1,0)+frontVec, AbilityDash.power);
end
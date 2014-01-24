ACTION_CREATE = 0;
ACTION_COLLIDE = 1;

--Set table name
AbilityDash = {};

function AbilityDash.OnCreate(userId, actionId)
	--local playerEnt = Entity.GetEntityByTag("Player");
	--local frontVec  = Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetOrient():GetFront();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local aimingEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1);
	local frontVec  = aimingEnt:GetTransformation():GetOrient():GetFront();

	local handle = playerEnt:GetCollision():GetHandle();
	playerEnt:GetPhysics():KnockBack(handle, Vec3.New(0,0.1,0)+frontVec, 100);
end

function AbilityDash.OnCollide (entityId)
	
end

function AbilityDash.OnDestroy ()

end
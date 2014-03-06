Boost = {};
Boost.damage = 0;
Boost.knockback = 0;
Boost.cooldown = 0;
Boost.charges = 1;
Boost.chargeTime = 0;
Boost.channelingTime = 0;
Boost.duration = 0;

function Boost.ChargeDone (time, userId, actionId)
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local statComp = casterEnt:GetStatChange();
	statComp:SetSpeed(2, 10);
	statComp:SetJumpHeight(2, 10);
end

function Boost.ChannelingDone (time, userId, actionId)
end

Boost = {};
Boost.damage = 0;
Boost.knockback = 0;
Boost.cooldown = 0;
Boost.charges = 1;
Boost.chargeTime = 0;
Boost.channelingTime = 0;
Boost.duration = 0;

function Boost.ChargeDone (time, userId, actionId)
	local self = Entity.New();
	local networkComp = Network.New(self, userId, actionId);
	local dakComp = DamageAndKnockback.New(self, Boost.damage , Boost.knockback);
	Boost.OnCreate(userId, actionId);
end

function Boost.ChannelingDone (time, userId, actionId)
end

function Boost.OnCreate (userId, actionId)
	--Entities
	local self = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);

	local scriptComp = Script.New(self, "Boost");
	local statComp = casterEnt:GetStatChange();
	statComp:SetSpeed(2, 10);
	statComp:SetJumpHeight(2, 10);

end

function Boost.OnCollide (self, entity)
--if entity:DoesExist() then
--	local dakComp = self:GetDamageAndKnockback();
--	local hitCol = entity:GetCollision();
--	local hitPhys = entity:GetPhysics();
--	local type = hitPhys:GetType(hitCol);
--end
end

function Boost.OnDestroy (self)
	Entity.Remove(self);
end

AbilityRay = {};
AbilityRay.damage = 0;
AbilityRay.pushback = 0;
AbilityRay.cooldown = 5;
AbilityRay.chargeTime = 0.0;
AbilityRay.channelingTime = 0.0;

function AbilityRay.ChargeStart(userId, actionId)
end

function AbilityRay.ChargeDone(time, userId, actionId)
end

function AbilityRay.ChannelingDone(time, userId, actionId)
	AbilityRay.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOT1, true);
end

function AbilityRay.Interrupted(time, userId, actionId)
end

function AbilityRay.OnCreate (userId, actionId)
	--Logging.Log(LogLevel.DEBUG_PRINT, "Creating Missile");
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	collisionComp:CreateHandle(self, 1, true);
	local scriptComp = Script.New(self, "AbilityRay");
	colRespComp:SetContainer(collisionComp);
	physicsComp:BindNoShape(collisionComp, posVec, Quat.New(0,0,0,1));
	--Logging.Log(LogLevel.DEBUG_PRINT, "Missile handle: "..collisionComp:GetHandle());
	physicsComp:ShootRay(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), Vec3.New(frontVec.x, frontVec.y, frontVec.z), 1000);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 50, frontVec.y * 50, frontVec.z * 50));
	transformComp:SetPos(posVec);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "AbilityRay");
	end
	local playerComponent = playerEnt:GetPlayerComponent();
	playerComponent:StartCooldown(playerComponent:GetSelectedAbility(), AbilityRay.cooldown);
end

function AbilityRay.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	local network = self:GetNetwork();
	local transComp = self:GetTransformation();
	transComp:SetPos(self:GetCollisionResponder():GetCollisionPosition(entity));
	--local targetPlayerComponent = entity:GetPlayerComponent();
	--local abilityOwnerNetwork = self:GetNetwork();
	--local abilityOwnerId = abilityOwnerNetwork:GetUserId();
	--local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
	--local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
	if self ~= entity then
		Explosion.OnCreate(network:GetUserId(), network:GetActionId());
	end
end

function AbilityRay.OnDestroy (self)
	--local network = self:GetNetwork();
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end

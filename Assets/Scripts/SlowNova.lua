SlowNova = {};
SlowNova.cooldown = 7;
SlowNova.charges = 3;
SlowNova.chargeTime = 0;
SlowNova.channelingTime = 0;
SlowNova.duration = 0.6;

function SlowNova.OnLoad()
	ResourceManager.LoadParticle("frostBlast");
end

function SlowNova.ChargeDone (time, userId, actionId)
	SlowNova.OnCreate(userId, actionId);
end

function SlowNova.ChannelingDone (time, userId, actionId)
end

function FireBall.Interrupted (time, userId, actionId)
end

function SlowNova.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	--Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "SlowNova");
	TimerEntity.StartTimer(userId, actionId, SlowNova.duration, "SlowNova", "OnDestroy", self);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local facePos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Quat.New(0,0,0,1);
	local startPos = casterEnt:GetTransformation():GetPos();
	transformComp:SetPos(startPos);
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 15, 1, false, false);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 0, dirVec.y * 0, dirVec.z * 0));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));

	if Global.IsClient then
		ParticleEmitter.New(self, "frostBlast");
	end
end

function SlowNova.OnCollide (self, entity)
if entity:DoesExist() then
	local dakComp = self:GetDamageAndKnockback();
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	if type == PhysicsType.TYPE_PLAYER then
		local targetPlayerComponent = entity:GetPlayerComponent();
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			local statComp = entity:GetStatChange();
			statComp:SetSpeed(0.5);
			statComp:SetJumpHeight(0);
			statComp:SetKnockbackResistance(2);
			statComp:SetDamageResistance(2);
			TimerEntity.StartTimer(abilityOwnerId, abilityOwnerNetwork:GetActionId(), 5, "SlowNova", "Reset", entity);
		end
	end
end
end

function SlowNova.OnDestroy (self)
	Entity.Remove(self);
end


function SlowNova.Reset(self)
	local statComp = self:GetStatChange();
	statComp:SetSpeed(1);
	statComp:SetJumpHeight(1);
	statComp:SetKnockbackResistance(1);
	statComp:SetDamageResistance(1);
end
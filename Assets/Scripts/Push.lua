Push = {};
Push.knockback = 40;
Push.currentKnockback = 40;
Push.cooldown = 1;
Push.charges = -1;
Push.chargeTime = 1;
Push.channelingTime = 0;
Push.duration = 0.5;

function Push.OnLoad()
	ResourceManager.LoadParticle("fireball");
end

function Push.ChargeDone (time, userId, actionId)
	--if time >= Push.chargeTime * 0.5 then
		Push.currentKnockback = Push.knockback * ((time) / Push.chargeTime);
		Push.OnCreate(userId, actionId);
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local animComp	= casterEnt:GetAnimation();
	animComp:SetUpperAnimClip(AnimClip.SHOOT, true);
		
	--end
end

function Push.ChannelingDone (time, userId, actionId)
end

function Push.Interrupted (time, userId, actionId)
end

function Push.OnCreate (userId, actionId)
	--Logging.Log(LogLevel.DEBUG_PRINT, "Crating push.lua");
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "Push");
	TimerEntity.StartTimer(userId, actionId, Push.duration, "Push", "OnDestroy", self);
	Follower.New(self, casterEnt, 3);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();
	transformComp:GetOrient():SetOrientation(rotQuat);
	--transformComp:GetOrient():Pitch(-90);
	rotQuat = transformComp:GetOrient():GetQuaternion();
	local startPos = casterEnt:GetTransformation():GetPos();
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 8, 1, false, false);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 0, dirVec.y * 0, dirVec.z * 0));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "PushMeMaybe");
	end
end

function Push.OnCollide (self, entity)
	if entity:DoesExist() then
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
				local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				local health = entity:GetHealth();
				hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), Push.currentKnockback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
	end
end

function Push.OnDestroy (self)
	Entity.Remove(self);
end

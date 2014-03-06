MachineGun = {};
MachineGun.damage = 5;
MachineGun.knockback = 0;
MachineGun.cooldown = 0.01;
MachineGun.charges = 99;
MachineGun.chargeTime = 0;
MachineGun.channelingTime = 0;
MachineGun.duration = 5;

function MachineGun.OnLoad()
	--ResourceManager.LoadParticle("MachineGun");
end

function MachineGun.ChargeDone (time, userId, actionId)
	MachineGun.OnCreate(userId, actionId);
end

function MachineGun.ChannelingDone (time, userId, actionId)
end

function FireBall.Interrupted (time, userId, actionId)
end

function MachineGun.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	--Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "MachineGun");
	local timerComp = Timer.New(self, MachineGun.duration);
	local dakComp = DamageAndKnockback.New(self, MachineGun.damage , MachineGun.knockback);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local facePos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();
	transformComp:GetOrient():SetOrientation(rotQuat);
	transformComp:GetOrient():Pitch(-90);
	rotQuat = transformComp:GetOrient():GetQuaternion();
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	transformComp:SetPos(startPos);
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 0.3, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -0, 0));

	if Global.IsClient then
		--local particleComp = ParticleEmitter.New(self, "MachineGun");
		local waterComp = WaterCollider.New(self);
		waterComp:SetDisturbPower(1);
		waterComp:SetDisturbInterval(0.3);
		waterComp:SetRadius(1);
	end
end

function MachineGun.OnCollide (self, entity)
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
			local health = entity:GetHealth();
			if not health:IsDead() then
				local network = entity:GetNetwork();
				local receiverId = network:GetUserId();
				health:Damage(abilityOwnerId, dakComp:GetDamage(), receiverId);
			end
		end
	end
end
end

function MachineGun.OnDestroy (self)
	Entity.Remove(self);
end

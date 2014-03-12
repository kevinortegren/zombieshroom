MachineGun = {};
MachineGun.damage = 5;
MachineGun.knockback = 0;
MachineGun.cooldown = 0.01;
MachineGun.charges = 99;
MachineGun.chargeTime = 0;
MachineGun.channelingTime = 0;
MachineGun.duration = 5;
MachineGun.crosshair = "";

function MachineGun.OnLoad()
	--ResourceManager.LoadParticle("MachineGun");
end

function MachineGun.ChargeStart(userId, actionId)
end

function MachineGun.ChargeDone (time, userId, actionId)
	MachineGun.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTLEFT1, true);
end

function MachineGun.ChannelingDone (time, userId, actionId)
end

function MachineGun.Interrupted (time, userId, actionId)
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
	TimerEntity.StartTimer(userId, actionId, MachineGun.duration, "MachineGun", "OnDestroy", self);
	local dakComp = DamageAndKnockback.New(self, MachineGun.damage , MachineGun.knockback);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local facePos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();
	transformComp:GetOrient():SetOrientation(rotQuat);
	transformComp:GetOrient():Pitch(-270);
	rotQuat = transformComp:GetOrient():GetQuaternion();
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	transformComp:SetPos(startPos);
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 0.3, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -0, 0));

	if Global.IsClient then
		--local particleComp = ParticleEmitter.New(self, "MachineGun");
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Thorn");
		renderComp:SetMaterial("MachineGunMaterialOfPower");
		renderComp:SetMaterialDiffuse("sandTiles");
		renderComp:SetMaterialEffect("Mesh");
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
	local type = hitCol:GetType();
	if type == PhysicsType.TYPE_PLAYER then
		local targetPlayerComponent = entity:GetPlayerComponent();
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			local health = entity:GetHealth();
			if not health:IsDead() then
				health:Damage(abilityOwnerId, dakComp:GetDamage(), "MachineGun");
			end
		end
	end
	MachineGun.OnDestroy(self);
end
end

function MachineGun.OnDestroy (self)
	Entity.Remove(self);
end

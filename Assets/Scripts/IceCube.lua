IceCube = {};
IceCube.damage = 0;
IceCube.knockback = 50;
IceCube.cooldown = 5;
IceCube.charges = 0;
IceCube.chargeTime = 0;
IceCube.channelingTime = 0;
IceCube.duration = 5;
IceCube.charges = -1;
IceCube.crosshair = "";

function IceCube.OnLoad()
	ResourceManager.LoadModel("PentagonSphere");
	ResourceManager.LoadEffect("Mesh_Refractive_Clear");
	ResourceManager.LoadSound("Abilities/Bubble/bubblefire1-1.wav", bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
end

function IceCube.ChargeStart(userId, actionId)
	
end

function IceCube.ChargeDone (time, userId, actionId)
	IceCube.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.DOUBLEOVERTHROW1, true);
end

function IceCube.ChannelingDone (time, userId, actionId)
end

function IceCube.Interrupted (time, userId, actionId)
end

function IceCube.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "IceCube");
	TimerEntity.StartTimer(userId, actionId, IceCube.duration, "IceCube", "OnDestroy", self);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3))
	
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 4, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);
	transformComp:SetScale(Vec3.New(5, 5, 5));
	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("HexagonSphere");
		renderComp:SetMaterial("IceCubeer");
		renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_NONE);
		renderComp:SetMaterialEffect("Mesh_Refractive_Clear");
		Static.Play3DSound("Abilities/Bubble/bubblefire1-1.wav", 1.0, startPos, 10.0, 100.0);
		local waterCollider = WaterCollider.New(self);
      	waterCollider:SetDisturbPower(0.5);
      	waterCollider:SetDisturbInterval(2.5);
      	waterCollider:SetRadius(7);
	end
end

function IceCube.OnCollide (self, entity)
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local type = hitCol:GetType();
		if type == PhysicsType.TYPE_PLAYER then
			local targetPlayerComponent = entity:GetPlayerComponent();
			local abilityOwnerNetwork = self:GetNetwork();
			local abilityOwnerId = abilityOwnerNetwork:GetUserId();
			local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
			local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
			local health = entity:GetHealth();
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				if not health:IsDead() then
					health:Damage(abilityOwnerId, IceCube.damage, "IceCube");
				end
			end
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				Static.KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), IceCube.knockback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
	end
end

function IceCube.OnDestroy (self)
	Entity.Remove(self);
end

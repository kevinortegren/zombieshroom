AbilityBall = {};
AbilityBall.damage = 20;
AbilityBall.knockback = 20;
AbilityBall.cooldown = 3;
AbilityBall.charges = 0;
AbilityBall.chargeTime = 0;
AbilityBall.channelingTime = 0;
AbilityBall.duration = 2;
AbilityBall.charges = -1;
AbilityBall.crosshair = "";

function AbilityBall.OnLoad()
	ResourceManager.LoadModel("Primitives/sphereTangents");
	ResourceManager.LoadTexture("Fireball");
	ResourceManager.LoadTexture("fireballDiffuse");
	ResourceManager.LoadTexture("fireballSpecular");
	ResourceManager.LoadTexture("fireballNormal");
	ResourceManager.LoadTexture("fireballGlow");
	ResourceManager.LoadEffect("Mesh_NormalMap");
	ResourceManager.LoadParticle("SmockeochElden");
end

function AbilityBall.ChargeStart(userId, actionId)
end

function AbilityBall.ChargeDone (time, userId, actionId)
	AbilityBall.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTDOUBLE2, true);
end

function AbilityBall.ChannelingDone (time, userId, actionId)
end

function AbilityBall.Interrupted(time, userId, actionId)
end

function AbilityBall.Explode(self)
	if Global.IsClient then
		self:RemoveRenderable();
		self:RemovePointLight();
	end
	local network = self:GetNetwork();
	Explosion.OnCreate(network:GetUserId(), network:GetActionId());
	self:RemovePhysics();
	self:RemoveCollision();
	self:RemoveCollisionResponder();
	self:GetParticleEmitter():SetAlive(-1.0);
end

function AbilityBall.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "AbilityBall");
	local networkEnt = Network.New(self, userId, actionId);
	TimerEntity.StartTimer(userId, actionId, AbilityBall.duration, "AbilityBall", "Explode", self);
	TimerEntity.StartTimer(userId, actionId, AbilityBall.duration + 2, "AbilityBall", "OnDestroy", self);

	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3))
	
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 0.5, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);
	transformComp:SetScale(Vec3.New(0.5, 0.5, 0.5));

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Fireball");
		renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_DYNAMIC);
		renderComp:SetMaterialDiffuse("fireballDiffuse");
		renderComp:SetMaterialSpecular("fireballSpecular");
		renderComp:SetMaterialNormal("fireballNormal");
		renderComp:SetMaterialGlow("fireballGlow");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
		local particleComp = ParticleEmitter.New(self, "SmockeochElden");
		local pointlightComp = PointLight.New(self);
		pointlightComp:SetColor(Vec4.New(1.0, 0.5, 0.0, 1.0));
		pointlightComp:SetRange(10.0);
		pointlightComp:SetAttenuation(Vec3.New(0.5, 0.15, 0.005));
	end
end

function AbilityBall.OnCollide (self, entity)
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
			local health = entity:GetHealth();
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				
				if not health:IsDead() then
					health:Damage(abilityOwnerId, AbilityBall.damage * entity:GetStatChange():GetDamageResistance(), "AbilityBall");
				end
			end
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityBall.knockback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
	end
end

function AbilityBall.OnDestroy (self)
	Entity.Remove(self);
end

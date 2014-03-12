--Gustav's cannonball ability script, copy but do not edit! Make your own abilities dammit!

Cannonball = {};
Cannonball.damage = 20;
Cannonball.knockback = 20;
Cannonball.cooldown = 3;
Cannonball.charges = 0;
Cannonball.chargeTime = 0;
Cannonball.channelingTime = 0;
Cannonball.duration = 4;
Cannonball.charges = -1;
Cannonball.crosshair = "";

function Cannonball.OnLoad()
	ResourceManager.LoadModel("Primitives/sphereTangents");
	ResourceManager.LoadTexture("Cannonball");
	ResourceManager.LoadTexture("Cannonball_Diffuse");
	ResourceManager.LoadTexture("Cannonball_Specular");
	ResourceManager.LoadTexture("Cannonball_Normal");
	ResourceManager.LoadEffect("Mesh_NormalMap");
	ResourceManager.LoadParticle("CannonballTrail");
	ResourceManager.LoadScript("BigExplosion");
	BigExplosion.OnLoad();
end

function Cannonball.ChargeStart(userId, actionId)
end

function Cannonball.ChargeDone (time, userId, actionId)
	Cannonball.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTRIGHT1, true);
end

function Cannonball.ChannelingDone (time, userId, actionId)
end

function Cannonball.Interrupted(time, userId, actionId)
end

function Cannonball.Explode(self)
	if Global.IsClient then
		self:RemoveRenderable();
	end
	local network = self:GetNetwork();
	BigExplosion.OnCreate(network:GetUserId(), network:GetActionId());
	self:RemovePhysics();
	self:RemoveCollision();
	self:RemoveCollisionResponder();
	self:GetParticleEmitter():SetAlive(-1.0);

	TimerEntity.StartTimer(network:GetUserId(), network:GetActionId(), 4, "Cannonball", "OnDestroy", self);
end

function Cannonball.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "Cannonball");
	local networkEnt = Network.New(self, userId, actionId);

	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3))
	
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 1, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Cannonballs");
		renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_DYNAMIC);
		renderComp:SetMaterialDiffuse("Cannonball_Diffuse");
		renderComp:SetMaterialSpecular("Cannonball_Specular");
		renderComp:SetMaterialNormal("Cannonball_Normal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
		local particleComp = ParticleEmitter.New(self, "CannonballTrail");
	end
end

function Cannonball.OnUpdate (self)
	local transform = self:GetTransformation();
	local physics = self:GetPhysics();
	if physics ~= nil then
		local direction = physics:GetVelocity();
		transform:GetOrient():LookAt(direction, Vec3.New(0.0, 1.0, 0.0));
	end
end

function Cannonball.OnCollide (self, entity)
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
				
			if not health:IsDead() then
				health:Damage(abilityOwnerId, Cannonball.damage * entity:GetStatChange():GetDamageResistance(), "Cannonball");
			end
			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			local hitPhys = entity:GetPhysics();
			if hitPhys ~= nil then
				hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), Cannonball.knockback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end

		if type ~= PhysicsType.TYPE_ABILITYSPAWN then
			Logging.Log(LogLevel.DEBUG_PRINT, "Type: " .. type);
			Cannonball.Explode(self);
		end
	end
end

function Cannonball.OnDestroy (self)
	Entity.Remove(self);
end

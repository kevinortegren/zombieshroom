AbilityTotem = {};
AbilityTotem.cooldown = 5;
AbilityTotem.charges = 1;
AbilityTotem.chargeTime = 0;
AbilityTotem.channelingTime = 0;
AbilityTotem.duration = 30;
AbilityTotem.crosshair = "";

function AbilityTotem.OnLoad()
	ResourceManager.LoadModel("TotemPole");
	ResourceManager.LoadTexture("TotemPoleDiffuse");
	ResourceManager.LoadTexture("TotemPoleNormal");
	ResourceManager.LoadEffect("Mesh_NormalMap");
end

function AbilityTotem.ChargeStart(userId, actionId)
end

function AbilityTotem.ChargeDone (time, userId, actionId)
	AbilityTotem.OnCreate(userId, actionId);

	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTDOUBLE2, true);
end

function AbilityTotem.ChannelingDone (time, userId, actionId)
end

function AbilityTotem.Interrupted(time, userId, actionId)
end

function AbilityTotem.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "AbilityTotem");
	local networkComp = Network.New(self, userId, actionId);
    
    -- Setup timers
    for i = 1, AbilityTotem.duration, 1 do
        TimerEntity.StartTimer(userId, actionId, i, "AbilityTotem", "Fire", self);
    end
    
	TimerEntity.StartTimer(userId, actionId, AbilityTotem.duration, "AbilityTotem", "Disappear", self);
	TimerEntity.StartTimer(userId, actionId, AbilityTotem.duration + 2, "AbilityTotem", "OnDestroy", self);

	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	
	local tempPos = casterEnt:GetTransformation():GetPos();
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	local originPos = Vec3.New(startPos.x, startPos.y, startPos.z);

	physicsComp:BindCylinderShape(collisionComp, originPos, Quat.New(0,0,0,1), 1.5, 1, 1, true, true);
    physicsComp:LockYOrientation(collisionComp);
	--physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	--physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("TotemPole");
		renderComp:SetMaterial("TotemPole");
		renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_DYNAMIC);
		renderComp:SetMaterialDiffuse("TotemPoleDiffuse");
		renderComp:SetMaterialNormal("TotemPoleNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
end

function AbilityTotem.Disappear(self)
	if Global.IsClient then
		self:RemoveRenderable();
	end

	local network = self:GetNetwork();
	Explosion.OnCreate(network:GetUserId(), network:GetActionId());

	self:RemovePhysics();
	self:RemoveCollision();
	self:RemoveCollisionResponder();
end

function AbilityTotem.Fire(self)
    Logging.Log(LogLevel.DEBUG_PRINT, "Totem is firing!");

    local networkComp = self:GetNetwork();
    TotemProjectile.direction = 0;
    TotemProjectile.OnCreate(networkComp:GetUserId(), networkComp:GetActionId());
    
    TotemProjectile.direction = 1;
    TotemProjectile.OnCreate(networkComp:GetUserId(), networkComp:GetActionId());
    
    TotemProjectile.direction = 2;
    TotemProjectile.OnCreate(networkComp:GetUserId(), networkComp:GetActionId());
    
    TotemProjectile.direction = 3;
    TotemProjectile.OnCreate(networkComp:GetUserId(), networkComp:GetActionId());
end

function AbilityTotem.OnCollide (self, entity)
end

function AbilityTotem.OnDestroy (self)
	Entity.Remove(self);
end

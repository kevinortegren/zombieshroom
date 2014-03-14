TotemProjectile = {};
TotemProjectile.direction = 0;
TotemProjectile.duration = 1;
TotemProjectile.damage = 20;
TotemProjectile.knockback = 0;
TotemProjectile.speed = 20;

function TotemProjectile.OnLoad()
	ResourceManager.LoadModel("blowdart");
	ResourceManager.LoadTexture("blowdartDiffuse");
	ResourceManager.LoadEffect("Mesh");
    ResourceManager.LoadParticle("Acid");
end

function TotemProjectile.OnCreate (userId, actionId)
    -- Create/Get entities
    local self = Entity.New();
    local parent = Entity.GetEntityByNetworkID(userId, actionId, 0);
    
    -- Create components
    local transformComp = Transformation.New(self);
    local collisionComp = Collision.New(self);
    local colRespComp = CollisionResponder.New(self);
    local physicsComp = Physics.New(self);
    local scriptComp = Script.New(self, "TotemProjectile");
    local networkComp = Network.New(self, userId, actionId);
    local dakComp = DamageAndKnockback.New(self, TotemProjectile.damage, TotemProjectile.knockback);
    
    if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("blowdart");
		renderComp:SetMaterial("TotemProjectile");
		renderComp:SetMaterialDiffuse("blowdartDiffuse");
		renderComp:SetMaterialEffect("Mesh");
        
        local particleComp = ParticleEmitter.New(self, "Acid");
	end
    
    -- Setup timers
    TimerEntity.StartTimer(userId, actionId, TotemProjectile.duration, "TotemProjectile", "Disappear", self);
    TimerEntity.StartTimer(userId, actionId, TotemProjectile.duration + 0.5, "TotemProjectile", "OnDestroy", self);
    
    -- Set component values
    collisionComp:CreateHandle(self, 1, false);
    colRespComp:SetContainer(collisionComp);
    
    if parent:DoesExist() then
        local spawnPosition = parent:GetTransformation():GetPos();
        local spawnOrientation = parent:GetTransformation():GetOrient();

        spawnOrientation:Yaw(TotemProjectile.direction * 90);
        local direction = spawnOrientation:GetFront();
        
        spawnPosition = Vec3.New((spawnPosition.x + direction.x * 1.35), (0.5 + spawnPosition.y + direction.y * 1.35), (spawnPosition.z + direction.z * 1.35));
        
        transformComp:SetPos(spawnPosition);
        --transformComp:GetOrient():Pitch(90);
        transformComp:GetOrient():LookAt(direction, Vec3.New(0, 1, 0));
        
        physicsComp:BindSphereShape(collisionComp, spawnPosition, transformComp:GetOrient():GetQuaternion(), 0.3, 1, true, true);
        --physicsComp:SetVelocity(collisionComp, Vec3.New(50, 0, 0));
        physicsComp:SetVelocity(collisionComp, Vec3.New(direction.x * TotemProjectile.speed, direction.y * TotemProjectile.speed, direction.z * TotemProjectile.speed));
        physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));
    end
end

function TotemProjectile.OnCollide (self, entity)
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
					health:Damage(abilityOwnerId, 10 * entity:GetStatChange():GetDamageResistance(), "AbilityTotem");
				end
                
                local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				Static.KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), 20 * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
        
        TotemProjectile.Disappear(self);
	end
end

function TotemProjectile.Disappear(self)
    if Global.IsClient then
		self:RemoveRenderable();
	end

    self:RemovePhysics();
	self:RemoveCollision();
	self:RemoveCollisionResponder();
    self:RemoveDamageAndKnockback();

	local emitter = self:GetParticleEmitter();
	if emitter ~= nil then
		self:GetParticleEmitter():SetAlive(-1.0);
	end
end

function TotemProjectile.OnDestroy (self)
	Entity.Remove(self);
end

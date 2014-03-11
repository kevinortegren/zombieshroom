TotemProjectile = {};
TotemProjectile.damage = 10;
TotemProjectile.knockback = 0;

function TotemProjectile.OnLoad()
	ResourceManager.LoadModel("TotemPole");
	ResourceManager.LoadTexture("TotemPoleDiffuse");
	ResourceManager.LoadTexture("TotemPoleNormal");
	ResourceManager.LoadEffect("Mesh_NormalMap");
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
    local dakComp = DamageAndKnockback.New(self, MachineGun.damage, MachineGun.knockback);
    
    -- Set component values
end

function TotemProjectile.OnCollide (self, entity)
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
					health:Damage(abilityOwnerId, 10 * entity:GetStatChange():GetDamageResistance());
				end
			end
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), 20 * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
	end
end

function TotemProjectile.OnDestroy (self)
	Entity.Remove(self);
end

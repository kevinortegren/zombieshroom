AbilityBall = {};
AbilityBall.damage = 20;
AbilityBall.pushback = 20;
AbilityBall.cooldown = 1;

function AbilityBall.OnCreate (userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating FireBall");
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local playerAimingDeviceEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1)
	local posVec = playerEnt:GetTransformation():GetPos();
	local frontVec = playerAimingDeviceEnt:GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);

	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	collisionComp:CreateHandle(self, 1, false);
	local transformComp = Transformation.New(self);
	--local particleComp = ParticleEmitter.New(self, "fireball");
	local scriptComp = Script.New(self, "AbilityBall");
	
	physicsComp:BindSphereShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 5, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 50, frontVec.y * 50, frontVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	colRespComp:SetContainer(collisionComp);
	transformComp:SetPos(posVec);
	
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "fireball");
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Fireball");
		renderComp:SetMaterialDiffuse("fireballDiffuse");
		renderComp:SetMaterialSpecular("fireballSpecular");
		renderComp:SetMaterialNormal("fireballNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
	local playerComponent = playerEnt:GetPlayerComponent();
	playerComponent:StartCooldown(playerComponent:GetSelectedAbility(), AbilityBall.cooldown);
end

function AbilityBall.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	
	if type == PhysicsType.TYPE_PLAYER and self ~= entity then
		local hitPos = entity:GetTransformation():GetPos();
		local selfPos = self:GetTransformation():GetPos();
		hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityBall.pushback);
		
		-- Deal damage to the colliding player
		local health = entity:GetHealth();
		local targetPlayerComponent = entity:GetPlayerComponent();
		
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		
		if not health:IsDead() and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			local network = entity:GetNetwork();
			local receiverId = network:GetUserId();
			
			health:Damage(abilityOwnerId, AbilityBall.damage, receiverId);
		end
	end
end

function AbilityBall.OnDestroy (self)
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end

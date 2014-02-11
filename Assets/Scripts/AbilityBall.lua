AbilityBall = {};
AbilityBall.damage = 20;
AbilityBall.pushback = 20;
AbilityBall.cooldown = 1.0;
AbilityBall.chargeTime = 1.0;
AbilityBall.channelingTime = 1.0;

function AbilityBall.ChargeDone(time, userId, actionId)
end

function AbilityBall.ChannelingDone(time, userId, actionId)
	if time >= AbilityBall.chargeTime then
		AbilityBall.OnCreate(userId, actionId);
	end
end

function AbilityBall.OnCreate (userId, actionId)
	--Logging.Log(LogLevel.DEBUG_PRINT, "Creating FireBall");
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local playerAimingDeviceEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1)
	local posVec = playerEnt:GetTransformation():GetPos();
	local frontVec = playerAimingDeviceEnt:GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);
	local waterCollider = WaterCollider.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);

	collisionComp:CreateHandle(self, 1, false);
	local transformComp = Transformation.New(self);
	--local particleComp = ParticleEmitter.New(self, "fireball");
	local scriptComp = Script.New(self, "AbilityBall");

	posVec = Vec3.New((posVec.x + frontVec.x * 3), (2 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3));
	physicsComp:BindSphereShape(collisionComp, posVec, Quat.New(0,0,0,1), 1, 5, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 50, frontVec.y * 50, frontVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	colRespComp:SetContainer(collisionComp);
	transformComp:SetPos(posVec);
	waterCollider:SetDisturbPower(1.0);
	waterCollider:SetDisturbInterval(0.3);
	waterCollider:SetRadius(4);
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
end

function AbilityBall.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);

	local targetPlayerComponent = entity:GetPlayerComponent();
	local abilityOwnerNetwork = self:GetNetwork();
	local abilityOwnerId = abilityOwnerNetwork:GetUserId();
	local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
	local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();

	if type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
		local hitPos = entity:GetTransformation():GetPos();
		local selfPos = self:GetTransformation():GetPos();
		hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityBall.pushback);
		
		-- Deal damage to the colliding player
		local health = entity:GetHealth();
		
		if not health:IsDead() then
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

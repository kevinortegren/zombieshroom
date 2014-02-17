AbilityTest = {};
AbilityTest.damage = 5;
AbilityTest.knockback = 5;
AbilityTest.cooldown = 1;
AbilityTest.charges = 5;
AbilityTest.chargeTime = 0;
AbilityTest.channelingTime = 0;
AbilityTest.duration = 0;

function AbilityTest.ChargeDone (time, userId, actionId)
	AbilityTest.OnCreate(userId, actionId);
end

function AbilityTest.ChannelingDone (time, userId, actionId)
end

function AbilityTest.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "AbilityTest");
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetUp();
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	local entityAtAim = physicsComp:GetPlayerAtAim(collisionComp:GetHandle(), tempPos, Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront(), 10000);
	if entityAtAim:DoesExist() then
		Logging.Log(LogLevel.DEBUG_PRINT, "EXISTS");
		if self:GetPlayerComponent():GetTeamId() ~= entityAtAim:GetPlayerComponent():GetTeamId() then
			Logging.Log(LogLevel.DEBUG_PRINT, "NOT FRIEND");
			startPos = entityAtAim:GetTransformation():GetPos();
		end
	end
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 1, 1, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 1, dirVec.y * 1, dirVec.z * 1));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Fireball");
		renderComp:SetMaterialDiffuse("fireballDiffuse");
		renderComp:SetMaterialSpecular("fireballSpecular");
		renderComp:SetMaterialNormal("fireballNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
		local particleComp = ParticleEmitter.New(self, "fireball");
	end
end

function AbilityTest.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	if type == PhysicsType.TYPE_PLAYER then
		local targetPlayerComponent = entity:GetPlayerComponent();
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then
			local health = entity:GetHealth();
			if not health:IsDead() then
				local network = entity:GetNetwork();
				local receiverId = network:GetUserId();
				health:Damage(abilityOwnerId, AbilityBall.damage, receiverId);
			end
		end
		if abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then
			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityTest.knockback);
		end
	end
end

function AbilityTest.OnDestroy (self)
	--local network = self:GetNetwork();
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end
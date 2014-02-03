AbilityBall = {};
AbilityBall.damage = 25;
AbilityBall.pushback = 10;
AbilityBall.cooldown = 2.5;

function AbilityBall.OnCreate (userId, actionId)
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);

	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	collisionComp:CreateHandle(self, 1, false);
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "AbilityBall");
	physicsComp:BindSphereShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 5, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 50, frontVec.y * 50, frontVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	colRespComp:SetContainer(collisionComp);
	transformComp:SetPos(posVec);
	if Global.IsClient then
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
	local targetPlayerComponent = entity:GetPlayerComponent();
	local abilityOwnerNetwork = self:GetNetwork();
	local abilityOwnerId = abilityOwnerNetwork:GetUserId();
	local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
	local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
	if type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
		if not health:IsDead() then
			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityBall.pushback);
			local health = entity:GetHealth();
			local network = entity:GetNetwork();
			local receiverId = network:GetUserId();
			health:Damage(abilityOwnerId, AbilityBall.damage, receiverId);
		end
		AbilityBall.OnDestroy(self);
	end
end

function AbilityBall.OnDestroy (self)
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end

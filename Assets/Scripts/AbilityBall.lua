AbilityBall = {};
AbilityBall.damage = -1;
AbilityBall.pushback = 0;
AbilityBall.cooldown = 0;

function AbilityBall.OnCreate (userId, actionId)
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);

	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	collisionComp:CreateHandle(self, 1, true);
	local scriptComp = Script.New(self, "AbilityBall");
	colRespComp:SetContainer(collisionComp);
	local handle = collisionComp:GetHandle();
	physicsComp:ShootRay(handle, Vec3.New(posVec.x, posVec.y, posVec.z), Vec3.New(frontVec.x, frontVec.y, frontVec.z), 100);
	
	if Global.IsClient then
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

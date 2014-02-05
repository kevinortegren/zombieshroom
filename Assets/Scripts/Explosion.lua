Explosion = {};
Explosion.damage = 60;
Explosion.pushback = 20;
Explosion.cooldown = 0;

function Explosion.OnCreate (userId, actionId)
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetCollisionResponder():GetCollisionPosition(playerEnt);
	local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);
	--Logging.Log(LogLevel.DEBUG_PRINT, "ColPos: ");
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "Explosion");
	collisionComp:CreateHandle(self, 1, true);
	transformComp:SetPos(posVec);
	colRespComp:SetContainer(collisionComp);
	physicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), 20);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "fireball");
	end
end

function Explosion.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	local network = entity:GetNetwork();
	local targetPlayerComponent = entity:GetPlayerComponent();
	local abilityOwnerNetwork = self:GetNetwork();
	local abilityOwnerId = abilityOwnerNetwork:GetUserId();
	local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
	local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
	if type == PhysicsType.TYPE_PLAYER and abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
		local hitPos = entity:GetTransformation():GetPos();
		local selfPos = self:GetTransformation():GetPos();
		hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), Explosion.pushback);
		local health = entity:GetHealth();
		if not health:IsDead() then
			local receiverId = network:GetUserId();
			health:Damage(abilityOwnerId, Explosion.damage, receiverId);
		end
	end
end

function Explosion.OnDestroy (self)
	local network = self:GetNetwork();
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end

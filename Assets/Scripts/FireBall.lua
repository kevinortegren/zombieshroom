FireBall = {};
FireBall.damage = 20;
FireBall.knockback = 20;
FireBall.cooldown = 3;
FireBall.charges = 0;
FireBall.chargeTime = 0;
FireBall.channelingTime = 0;
FireBall.duration = 0;
FireBall.charges = -1;

function FireBall.ChargeDone (time, userId, actionId)
	FireBall.OnCreate(userId, actionId);
end

function FireBall.ChannelingDone (time, userId, actionId)
end

function FireBall.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "FireBall");
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3))
	
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 1, 1, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		local particleComp = ParticleEmitter.New(self, "fireball");
	end
end

function FireBall.OnCollide (self, entity)
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local hitPhys = entity:GetPhysics();
		local network = self:GetNetwork();
		local transComp = self:GetTransformation();
		transComp:SetPos(self:GetCollisionResponder():GetCollisionPosition(entity));
		if self ~= entity then
	 		Explosion.OnCreate(network:GetUserId(), network:GetActionId());
			FireBall.OnDestroy(self);
		end
	end
end

function FireBall.OnDestroy (self)
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
	Entity.Remove(self);
end

PowerRay = {};
PowerRay.damage = 10;
PowerRay.knockback = 3;
PowerRay.cooldown = 0.5;
PowerRay.charges = 40;
PowerRay.chargeTime = 0;
PowerRay.channelingTime = 0;
PowerRay.duration = 1;

function PowerRay.ChargeDone (time, userId, actionId)
	local self = Entity.New();
	local networkComp = Network.New(self, userId, actionId);
	local dakComp = DamageAndKnockback.New(self, PowerRay.damage , PowerRay.knockback);
	PowerRay.OnCreate(userId, actionId);
end

function PowerRay.ChannelingDone (time, userId, actionId)
end

function PowerRay.OnCreate (userId, actionId)
	--Entities
	local self = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	--Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "PowerRay");
	local timerComp = Timer.New(self, PowerRay.duration);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local facePos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetUp();
	local rotQuat = Quat.New(0,0,0,1);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	physicsComp:BindNoShape(collisionComp, facePos, rotQuat);
	local dirVecForward = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rayStartPos = Vec3.New((tempPos.x + dirVecForward.x * 3), (2 + tempPos.y + dirVecForward.y * 3), (tempPos.z + dirVecForward.z * 3));
	local rayComp = Ray.New(self, collisionComp:GetHandle(), rayStartPos, dirVecForward, 2000, false, false);
	rayComp = Ray.New(self, collisionComp:GetHandle(), facePos, rayComp:GetHitPos() - facePos, 2000, true, true);
	--physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 0, dirVec.y * 0, dirVec.z * 0));
	--physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(facePos);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "FallingLeafsParticle");
		local waterComp = WaterCollider.New(self);
		waterComp:SetDisturbPower(1);
		waterComp:SetDisturbInterval(0.3);
		waterComp:SetRadius(4);
	end
end

function PowerRay.OnCollide (self, entity)
if entity:DoesExist() then
	local dakComp = self:GetDamageAndKnockback();
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
			if not health:IsDead() then
				local network = entity:GetNetwork();
				local receiverId = network:GetUserId();
				health:Damage(abilityOwnerId, dakComp:GetDamage() * entity:GetStatChange():GetDamageResistance(), receiverId);
			end

			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), dakComp:GetKnockback() * entity:GetStatChange():GetKnockbackResistance());
	end
end
end

function PowerRay.OnDestroy (self)
	Entity.Remove(self);
end

Push = {};
Push.knockback = 10;
Push.cooldown = 3;
Push.charges = -1;
Push.chargeTime = 1;
Push.channelingTime = 0;
Push.duration = 0.2;

function Push.ChargeDone (time, userId, actionId)
	if time >= Push.chargeTime * 0.5 then
		Push.OnCreate(userId, actionId);
	end
end

function Push.ChannelingDone (time, userId, actionId)
end

function Push.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "Push");
	local timerComp = Timer.New(self, Push.duration);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();
	transformComp:GetOrient():SetOrientation(rotQuat);
	transformComp:GetOrient():Pitch(-90);
	rotQuat = transformComp:GetOrient():GetQuaternion();
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	physicsComp:BindConeShape(collisionComp, startPos, rotQuat, 6, 3, 1, false);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 0, dirVec.y * 0, dirVec.z * 0));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "fireball");
	end
end

function Push.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
end

function Push.OnDestroy (self)
end

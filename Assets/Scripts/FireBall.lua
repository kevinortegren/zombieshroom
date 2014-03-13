FireBall = {};
FireBall.knockback = 20;
FireBall.cooldown = 0.5;
FireBall.chargeTime = 2;
FireBall.channelingTime = 0;
FireBall.duration = 0;
FireBall.charges = 5;
FireBall.damage = 1.0;
FireBall.damageIncrease = 3.0;
FireBall.crosshair = "";

function FireBall.OnLoad()
	ResourceManager.LoadParticle("SmockeochElden");
	ResourceManager.LoadScript("FireBallExplosion");
	FireBallExplosion.OnLoad();
end

function FireBall.ChargeStart(userId, actionId)
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperChargingAnimClip(AnimClip.CHARGING1);
end

function FireBall.ChargeDone (time, userId, actionId)
  FireBall.OnCreate(userId, actionId);
  FireBall.damageIncrease = FireBall.damage * time;
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTDOUBLE1, true);
end

function FireBall.ChannelingDone (time, userId, actionId)
end

function FireBall.Interrupted (time, userId, actionId)
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
	
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 1, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "SmockeochElden");
		local pointlightComp = PointLight.New(self);
		pointlightComp:SetColor(Vec4.New(0.2, 0.05, 1.0, 1.0));
		pointlightComp:SetRange(5.0);
		pointlightComp:SetAttenuation(Vec3.New(0.5, 0.15, 0.005));
	end
end

function FireBall.OnCollide (self, entity)
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local type = hitCol:GetType();
		if type == PhysicsType.TYPE_PLAYER then
			local targetPlayerComponent = entity:GetPlayerComponent();
			local abilityOwnerNetwork = self:GetNetwork();
			local abilityOwnerId = abilityOwnerNetwork:GetUserId();
			local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
			local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
			local health = entity:GetHealth();
			if abilityOwnerNetwork:GetUserId() ~= entity:GetNetwork():GetUserId() then
				FireBallExplosion.OnCreate(abilityOwnerNetwork:GetUserId(), abilityOwnerNetwork:GetActionId(),FireBall.damageIncrease);
				FireBall.OnDestroy(self);
			end
		else
			FireBallExplosion.OnCreate(self:GetNetwork():GetUserId(), self:GetNetwork():GetActionId(),FireBall.damageIncrease);
			FireBall.OnDestroy(self);
		end
	end
end

function FireBall.OnDestroy (self)
	Entity.Remove(self);
end

Needler = {};
Needler.damage = 0;
Needler.knockback = 0;
Needler.cooldown = 1;
Needler.charges = 15;
Needler.duration = 60;
Needler.crosshair = "";

function Needler.OnLoad()
	ResourceManager.LoadParticle("magic_missile_01");
end

function Needler.ChargeStart(userId, actionId)
	
end

function Needler.ChargeDone (time, userId, actionId)
	Needler.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTRIGHT1, true);
end

function Needler.ChannelingDone (time, userId, actionId)
end

function Needler.Interrupted (time, userId, actionId)
end

function Needler.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);

	--Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "Needler");

	TimerEntity.StartTimer(userId, actionId, Needler.duration, "Needler", "OnDestroy", self);

	local dakComp = DamageAndKnockback.New(self, Needler.currentDamage , Needler.currentKnockback);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local facePos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetQuaternion();
	transformComp:GetOrient():SetOrientation(rotQuat);
	transformComp:GetOrient():Pitch(-90);
	rotQuat = transformComp:GetOrient():GetQuaternion();
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	transformComp:SetPos(startPos);
	local NeedlerComp = NeedlerComponent.New(self, 0.07, 32);
	local dirVecForward = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rayStartPos = Vec3.New((tempPos.x + dirVecForward.x * 3), (2 + tempPos.y + dirVecForward.y * 3), (tempPos.z + dirVecForward.z * 3));
	local rayComp = Ray.New(self, collisionComp:GetHandle(), rayStartPos, dirVecForward, 10000, false, false);
	rayComp = Ray.New(self, collisionComp:GetHandle(), facePos, rayComp:GetHitPos() - facePos, 10000, false, false);
	local entityAtAim = rayComp:GetHitEntity();
	if entityAtAim:DoesExist() then
		local type = entityAtAim:GetPhysics():GetType(entityAtAim:GetCollision());
		if type == PhysicsType.TYPE_PLAYER then
			local abilityOwnerNetwork = self:GetNetwork();
			local abilityOwnerId = abilityOwnerNetwork:GetUserId();
			local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
			local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
			if abilityOwnerPlayerComponent:GetTeamId() ~= entityAtAim:GetPlayerComponent():GetTeamId() then
				NeedlerComp:SetTargetEntity(entityAtAim);
			else
				NeedlerComp:SetTargetPosition(rayComp:GetHitPos());
			end
		else
			NeedlerComp:SetTargetPosition(rayComp:GetHitPos());
		end
	else
		NeedlerComp:SetTargetPosition(rayComp:GetHitPos());
	end
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 1, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "magic_missile_01");
		local pointlightComp = PointLight.New(self);
		pointlightComp:SetColor(Vec4.New(0.2, 0.05, 1.0, 1.0));
		pointlightComp:SetRange(5.0);
		pointlightComp:SetAttenuation(Vec3.New(0.5, 0.15, 0.005));
	end
end

function Needler.OnCollide (self, entity)
if entity:DoesExist() then
	local dakComp = self:GetDamageAndKnockback();
	local hitCol = entity:GetCollision();
	local type = hitCol:GetType();
	if type == PhysicsType.TYPE_PLAYER then
		local targetPlayerComponent = entity:GetPlayerComponent();
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			local health = entity:GetHealth();
			if not health:IsDead() then
				health:Damage(abilityOwnerId, dakComp:GetDamage() * entity:GetStatChange():GetDamageResistance(), "Needler");
			end
			Needler.OnDestroy(self);
		end
	end
end
end

function Needler.OnDestroy (self)
	Entity.Remove(self);
end

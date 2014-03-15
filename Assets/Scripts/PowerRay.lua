PowerRay = {};
PowerRay.damage = 50;
PowerRay.knockback = 3;
PowerRay.currentDamage = 50;
PowerRay.currentKnockback = 3;
PowerRay.cooldown = 1;
PowerRay.charges = 10;
PowerRay.chargeTime = 0;
PowerRay.channelingTime = 0;
PowerRay.duration = 1;
PowerRay.crosshair = "crosshairPrecision";

function PowerRay.OnLoad()
	ResourceManager.LoadSound("CC-BY3.0/qubodupElectricityDamage01.wav", 0x00400011);
end

function PowerRay.ChargeStart(userId, actionId)
	
end

function PowerRay.ChargeDone (time, userId, actionId)
	PowerRay.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOTRIGHT1, true);
end

function PowerRay.ChannelingDone (time, userId, actionId)
end

function PowerRay.Interrupted (time, userId, actionId)
end

function PowerRay.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	--Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "PowerRay");
	TimerEntity.StartTimer(userId, actionId, PowerRay.duration, "PowerRay", "OnDestroy", self);
	local dakComp = DamageAndKnockback.New(self, PowerRay.currentDamage , PowerRay.currentKnockback);

	local soundable = Soundable.New(self);
	
	--Setting stuff
	soundable:SetSound("CC-BY3.0/qubodupElectricityDamage01.wav", bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
	soundable:SetRange(10.0, 50.0);
	soundable:SetVolume(0.4);
	soundable:Play();

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
		--local particleComp = ParticleEmitter.New(self, "FallingLeafsParticle");
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
				health:Damage(abilityOwnerId, dakComp:GetDamage() * entity:GetStatChange():GetDamageResistance(), "PowerRay");
			end

			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			Static.KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), dakComp:GetKnockback() * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
		end
	end

	local hitPositionEntity = Entity.New();
	local hitPositionTransform = Transformation.New(hitPositionEntity);
	hitPositionTransform:SetPos(entity:GetTransformation():GetPos());
	local soundable = Soundable.New(hitPositionEntity);
    local networkComp = Network.New(hitPositionEntity, self:GetNetwork():GetUserId(), self:GetNetwork():GetActionId());
	soundable:SetSound("CC-BY3.0/qubodupElectricityDamage01.wav", bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
	soundable:SetRange(10.0, 50.0);
	soundable:SetVolume(0.6);
	soundable:Play();
	TimerEntity.StartTimer(networkComp:GetUserId(), networkComp:GetActionId(), PowerRay.duration, "PowerRay", "StopHitSound", hitPositionEntity);
end
end

function PowerRay.StopHitSound(hitPositionEntity)
	Entity.Remove(hitPositionEntity);
end

function PowerRay.OnDestroy (self)
	Entity.Remove(self);
end

RefractiveBall = {};
RefractiveBall.damage = 0;
RefractiveBall.knockback = 40;
RefractiveBall.cooldown = 3;
RefractiveBall.charges = 0;
RefractiveBall.chargeTime = 0;
RefractiveBall.channelingTime = 5;
RefractiveBall.duration = 0;
RefractiveBall.charges = -1;

function RefractiveBall.ChargeDone (time, userId, actionId)
	RefractiveBall.OnCreate(userId, actionId);
end

function RefractiveBall.ChannelingDone (time, userId, actionId)
	RefractiveBall.OnDestroy(Entity.GetEntityByNetworkID(userId, actionId, 0));
end

function RefractiveBall.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "RefractiveBall");
	--local timerComp = Timer.New(self, RefractiveBall.duration);
	Follower.New(self, casterEnt, 0);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local tempPos = casterEnt:GetTransformation():GetPos();
	
	physicsComp:BindSphereShape(collisionComp, tempPos, Quat.New(0,0,0,1), 5.0, 10000, true, false);
	physicsComp:SetVelocity(collisionComp, Vec3.New(0, 0, 0));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, 0, 0));
	transformComp:SetPos(tempPos);
	transformComp:SetScale(Vec3.New(5, 5, 5));

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("RefractiveBaller");
		renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_NONE);
		renderComp:SetMaterialEffect("Mesh_Refractive");
	end
end

function RefractiveBall.OnCollide (self, entity)
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local hitPhys = entity:GetPhysics();
		local type = hitPhys:GetType(hitCol);
		if type == PhysicsType.TYPE_PLAYER then
			local targetPlayerComponent = entity:GetPlayerComponent();
			local abilityOwnerNetwork = self:GetNetwork();
			local abilityOwnerId = abilityOwnerNetwork:GetUserId();
			local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
			local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				local health = entity:GetHealth();
				if not health:IsDead() then
					local network = entity:GetNetwork();
					local receiverId = network:GetUserId();
					health:Damage(abilityOwnerId, RefractiveBall.damage, receiverId);
				end
			end
			if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
				local hitPos = entity:GetTransformation():GetPos();
				local selfPos = self:GetTransformation():GetPos();
				hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), RefractiveBall.knockback);
			end
		end
	end
end

function RefractiveBall.OnDestroy (self)
	Entity.Remove(self);
end

ExplodingShroom = {};
ExplodingShroom.cooldown = 3;
ExplodingShroom.charges = 3;
ExplodingShroom.chargeTime = 0;
ExplodingShroom.channelingTime = 0;
ExplodingShroom.duration = 10;

function ExplodingShroom.ChargeDone (time, userId, actionId)
	ExplodingShroom.OnCreate(userId, actionId);
end

function ExplodingShroom.ChannelingDone (time, userId, actionId)
end

function ExplodingShroom.Interrupted (time, userId, actionId)
end

function ExplodingShroom.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "ExplodingShroom");
	local timerComp = Timer.New(self, ExplodingShroom.duration);
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
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 0.25, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 20, dirVec.y * 20, dirVec.z * 20));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("XplodingMushroom");
		renderComp:SetMaterial("XplodingMushroomMaterial");
		renderComp:SetMaterialDiffuse("XplodingMushroomDiffuse");
		renderComp:SetMaterialGlow("XplodingMushroomGlow");
		renderComp:SetMaterialNormal("XplodingMushroomNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
end

function ExplodingShroom.OnCollide (self, entity)
if entity:DoesExist() then
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	if type == PhysicsType.TYPE_STATIC then
		local network = self:GetNetwork();
		XplodingMushroomPlanted.OnCreate(network:GetUserId(), network:GetActionId());
		ExplodingShroom.OnDestroy(self);
	end
end
end

function ExplodingShroom.OnDestroy (self)
	Entity.Remove(self);
end

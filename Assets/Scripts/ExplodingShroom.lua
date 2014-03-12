ExplodingShroom = {};
ExplodingShroom.cooldown = 3;
ExplodingShroom.charges = 3;
ExplodingShroom.chargeTime = 0;
ExplodingShroom.channelingTime = 0;
--ExplodingShroom.duration = 10;

function ExplodingShroom.OnLoad()
	ResourceManager.LoadModel("XplodingMushroom");
	ResourceManager.LoadTexture("XplodingMushroomDiffuse");
	ResourceManager.LoadTexture("XplodingMushroomGlow");
	ResourceManager.LoadTexture("XplodingMushroomNormal");
	ResourceManager.LoadEffect("Mesh_NormalMap");
	ResourceManager.LoadScript("XplodingMushroomPlanted");
	XplodingMushroomPlanted.OnLoad();
end

function ExplodingShroom.ChargeStart(userId, actionId)
end

function ExplodingShroom.ChargeDone (time, userId, actionId)
	ExplodingShroom.OnCreate(userId, actionId);
	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.SHOOT1, true);
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
	--TimerEntity.StartTimer(userId, actionId, ExplodingShroom.duration, "ExplodingShroom", "OnDestroy", self);
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
	local type = hitCol:GetType();
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

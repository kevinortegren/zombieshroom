XplodingMushroomPlanted = {};
XplodingMushroomPlanted.cooldown = 0;
XplodingMushroomPlanted.charges = 0;
XplodingMushroomPlanted.chargeTime = 0;
XplodingMushroomPlanted.channelingTime = 0;
XplodingMushroomPlanted.duration = 0;

function XplodingMushroomPlanted.ChargeDone (time, userId, actionId)
	XplodingMushroomPlanted.OnCreate(userId, actionId);
end

function XplodingMushroomPlanted.ChannelingDone (time, userId, actionId)
end

function XplodingMushroomPlanted.Interrupted (time, userId, actionId)
end

function XplodingMushroomPlanted.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "XplodingMushroomPlanted");
	local timerComp = Timer.New(self, XplodingMushroomPlanted.duration);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Quat.New(0,0,0,1);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = tempPos;
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 2, 1, false, false);
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("XplodingMushroom");
		renderComp:SetMaterial("XplodingMushroomMaterial");
		renderComp:SetMaterialDiffuse("XplodingMushroomDiffuse");
		renderComp:SetMaterialNormal("XplodingMushroomNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
end

function XplodingMushroomPlanted.OnCollide (self, entity)
if entity:DoesExist() then
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
end
end

function XplodingMushroomPlanted.OnDestroy (self)
end

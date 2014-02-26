XplodingMushroomPlanted.dae = {};
XplodingMushroomPlanted.dae.cooldown = 0;
XplodingMushroomPlanted.dae.charges = 0;
XplodingMushroomPlanted.dae.chargeTime = 0;
XplodingMushroomPlanted.dae.channelingTime = 0;
XplodingMushroomPlanted.dae.duration = 0;

function XplodingMushroomPlanted.dae.ChargeDone (time, userId, actionId)
	XplodingMushroomPlanted.dae.OnCreate(userId, actionId);
end

function XplodingMushroomPlanted.dae.ChannelingDone (time, userId, actionId)
end

function XplodingMushroomPlanted.dae.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "XplodingMushroomPlanted.dae");
	local timerComp = Timer.New(self, XplodingMushroomPlanted.dae.duration);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Quat.New(0,0,0,1);
	local tempPos = casterEnt:GetTransformation():GetPos();
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 2, 1, false);
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("XplodingMushroom");
		renderComp:SetMaterial("XplodingMushroomMaterial");
		renderComp:SetMaterialDiffuse("XplodingMushroomDiffuse");
		renderComp:SetMaterialSpecular("");
		renderComp:SetMaterialNormal("XplodingMushroomNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
end

function XplodingMushroomPlanted.dae.OnCollide (self, entity)
if entity:DoesExist()
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
end
end

function XplodingMushroomPlanted.dae.OnDestroy (self)
end

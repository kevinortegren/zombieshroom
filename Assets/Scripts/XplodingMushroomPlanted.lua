XplodingMushroomPlanted = {};
XplodingMushroomPlanted.cooldown = 0;
XplodingMushroomPlanted.charges = 0;
XplodingMushroomPlanted.channelingTime = 0;
XplodingMushroomPlanted.duration = 120;

function XplodingMushroomPlanted.OnLoad()
	ResourceManager.LoadModel("XplodingMushroom");
	ResourceManager.LoadTexture("XplodingMushroomDiffuse");
	ResourceManager.LoadTexture("XplodingMushroomNormal");
	ResourceManager.LoadEffect("Mesh_NormalMap");
	ResourceManager.LoadScript("ShroomExplosion");
	ShroomExplosion.OnLoad();
end

function XplodingMushroomPlanted.ChargeStart(userId, actionId)
end

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
	TimerEntity.StartTimer(userId, actionId, XplodingMushroomPlanted.duration, "XplodingMushroomPlanted", "OnDestroy", self);
	
    --Setting stuff
	collisionComp:CreateHandle(self, 1, true);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rotQuat = Quat.New(0,0,0,1);
	
	local startPos = Vec3.New(0,0,0);
	if casterEnt:DoesExist() then
		startPos = casterEnt:GetTransformation():GetPos();
	end

	physicsComp:BindSphereShape(collisionComp, startPos, rotQuat, 4, 1, false, true);
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("XplodingMushroom");
		renderComp:SetMaterial("XplodingMushroomMaterial");
		renderComp:SetMaterialDiffuse("XplodingMushroomDiffuse");
		renderComp:SetMaterialNormal("XplodingMushroomNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end

	-- This is a test of script storage
	scriptComp:SetEntity("ShroomEntity", self);
end

function XplodingMushroomPlanted.OnUpdate(self)
	-- This is a test of script storage
	local scriptComp = self:GetScript();
	scriptComp:SetNumber("ShroomNumber", scriptComp:GetNumber("ShroomNumber") + 1);
	scriptComp:SetString("ShroomString", scriptComp:GetString("ShroomString") .. "a");
	
	Logging.Log(LogLevel.DEBUG_PRINT, "Exploding shroom number storage: " .. scriptComp:GetNumber("ShroomNumber"));
	Logging.Log(LogLevel.DEBUG_PRINT, "Exploding shroom string storage: " .. scriptComp:GetString("ShroomString"));

	local entity = scriptComp:GetEntity("ShroomEntity");
	local network = entity:GetNetwork();
	Logging.Log(LogLevel.DEBUG_PRINT, "Exploding shroom entity storage: (" .. network:GetUserId() .. ", " .. network:GetActionId() .. ", " .. network:GetSequenceId() .. ")");
end

function XplodingMushroomPlanted.OnCollide (self, entity)
if entity:DoesExist() then
	local hitCol = entity:GetCollision();
	local type = hitCol:GetType();
	if type == PhysicsType.TYPE_PLAYER then
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		local targetPlayerComponent = entity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			ShroomExplosion.OnCreate(abilityOwnerId, abilityOwnerNetwork:GetActionId());
			XplodingMushroomPlanted.OnDestroy(self);
		end
	end
end
end

function XplodingMushroomPlanted.OnDestroy (self)
	Entity.Remove(self);
end

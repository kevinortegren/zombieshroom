Identiray = {};
Identiray.damage = 10;
Identiray.knockback = 3;
Identiray.currentDamage = 10;
Identiray.currentKnockback = 3;
Identiray.cooldown = 0.5;
Identiray.charges = 40;
Identiray.chargeTime = 0;
Identiray.channelingTime = 0;
Identiray.duration = 1;

function Identiray.ChargeDone (time, userId, actionId)
	Identiray.OnCreate(userId, actionId);
end

function Identiray.ChannelingDone (time, userId, actionId)
end

function Identiray.Interrupted (time, userId, actionId)
end

function Identiray.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	
    --Components
	local networkComp = Network.New(self, userId, actionId);
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "Identiray");
	local timerComp = Timer.New(self, Identiray.duration);
    
	--Setting stuff
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
	
	transformComp:SetPos(facePos);
	if Global.IsClient then
		--local particleComp = ParticleEmitter.New(self, "FallingLeafsParticle");
		local waterComp = WaterCollider.New(self);
		waterComp:SetDisturbPower(1);
		waterComp:SetDisturbInterval(0.3);
		waterComp:SetRadius(4);
	end
end

function Identiray.OnCollide (self, entity)
if entity:DoesExist() then
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
    
    if type == PhysicsType.TYPE_ABILITY then
        local hitNetwork = entity:GetNetwork();
        if hitNetwork ~= nil then
            Logging.Log(LogLevel.DEBUG_PRINT, "Identiray: User: " .. hitNetwork:GetUserId() .. ", Action: " .. hitNetwork:GetActionId() .. ", Sequence: " .. hitNetwork:GetSequenceId());
        end
        
        local hitScript = entity:GetScript();
        if hitScript ~= nil then
            Logging.Log(LogLevel.DEBUG_PRINT, "Identiray: Script: " .. hitScript:GetName());
        end
    end
end
end

function Identiray.OnDestroy (self)
	Entity.Remove(self);
end

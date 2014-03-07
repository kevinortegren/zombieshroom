BigExplosion = {};
BigExplosion.damage = 40;
BigExplosion.pushback = 20;
BigExplosion.cooldown = 0;
BigExplosion.chargeTime = 0.0;
BigExplosion.channelingTime = 0.0;
BigExplosion.duration = 4.0;

function BigExplosion.OnLoad()
	ResourceManager.LoadParticle("BigExplosion");
end

function BigExplosion.OnCreate (userId, actionId)
	--Logging.Log(LogLevel.DEBUG_PRINT, "Creating BigExplosion");
	local self = Entity.New();
	local fatherEntity = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local posVec = Vec3.New(0,0,0);
	if fatherEntity:DoesExist() then
		--local prevHandle = fatherEntity:GetCollision():GetHandle();
		--Logging.Log(LogLevel.DEBUG_PRINT, "Handle in Expl: "..prevHandle);
		posVec = fatherEntity:GetTransformation():GetPos(); --playerEnt:GetCollisionResponder():GetCollisionPosition(playerEnt);
	end
	
	--local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);
	--Logging.Log(LogLevel.DEBUG_PRINT, "ColPos: ");
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "BigExplosion");
	collisionComp:CreateHandle(self, 1, true);
	transformComp:SetPos(posVec);
	transformComp:SetScale(Vec3.New(25,25,25));
	colRespComp:SetContainer(collisionComp);
	--Logging.Log(LogLevel.DEBUG_PRINT, "BigExplosion handle: "..collisionComp:GetHandle());
	physicsComp:BindNoShape(collisionComp, posVec, Quat.New(0,0,0,1));
	--Logging.Log(LogLevel.DEBUG_PRINT, "After bind call");
	physicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), 25);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "BigExplosion");
	end
	--Logging.Log(LogLevel.DEBUG_PRINT, "End of Oncreate");

	TimerEntity.StartTimer(userId, actionId, BigExplosion.duration, "BigExplosion", "OnDestroy", self);
end

function BigExplosion.OnCollide (self, entity)
	--Logging.Log(LogLevel.DEBUG_PRINT, "OnCollide");
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local hitPhys = entity:GetPhysics();
		local type = hitPhys:GetType(hitCol);
	 	if type == PhysicsType.TYPE_PLAYER then
		    local abilityOwnerNetwork = self:GetNetwork();
		    local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		    local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		   	local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		   	local targetPlayerComponent = entity:GetPlayerComponent();
		    if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			    local network = entity:GetNetwork();
			    local hitPos = entity:GetTransformation():GetPos();
			    local selfPos = self:GetTransformation():GetPos();
			    local health = entity:GetHealth();
          if not health:IsDead() then
				    health:Damage(abilityOwnerId, BigExplosion.damage * entity:GetStatChange():GetDamageResistance());
			    end
			    hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), BigExplosion.pushback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
			end
		end
	end
end
function BigExplosion.OnDestroy (self)
	Entity.Remove(self);
end

FireBallExplosion = {};
FireBallExplosion.damage = 20.0;
FireBallExplosion.damageIncrease = 1.0;
FireBallExplosion.pushback = 20;
FireBallExplosion.cooldown = 0.5;
FireBallExplosion.chargeTime = 0.0;
FireBallExplosion.channelingTime = 0.0;
FireBallExplosion.duration = 1.0;

function FireBallExplosion.OnLoad()
	ResourceManager.LoadParticle("Explosion_G-stuf");
	ResourceManager.LoadSound("Abilities/Fireball/fireballfire2-4.wav");
end

function FireBallExplosion.OnCreate (userId, actionId, damage)
	local self = Entity.New();
	local fatherEntity = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local posVec = Vec3.New(0,0,0);
  FireBallExplosion.damageIncrease = damage;
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
	local scriptComp = Script.New(self, "FireBallExplosion");
	collisionComp:CreateHandle(self, 1, true);
	transformComp:SetPos(posVec);
	transformComp:SetScale(Vec3.New(10,10,10));
	colRespComp:SetContainer(collisionComp);
	--Logging.Log(LogLevel.DEBUG_PRINT, "FireBallExplosion handle: "..collisionComp:GetHandle());
	physicsComp:BindNoShape(collisionComp, posVec, Quat.New(0,0,0,1));
	--Logging.Log(LogLevel.DEBUG_PRINT, "After bind call");
	physicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), 10);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "Explosion_G-stuf");
		Static.PlaySound("Abilities/Fireball/fireballfire2-4.wav", 1.0, posVec, 10.0, 100.0, bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
	end
	--Logging.Log(LogLevel.DEBUG_PRINT, "End of Oncreate");

	TimerEntity.StartTimer(userId, actionId, FireBallExplosion.duration, "FireBallExplosion", "OnDestroy", self);
end

function FireBallExplosion.OnCollide (self, entity)
	--Logging.Log(LogLevel.DEBUG_PRINT, "OnCollide");
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
			    local network = entity:GetNetwork();
			    local hitPos = entity:GetTransformation():GetPos();
			    local selfPos = self:GetTransformation():GetPos();
			    local health = entity:GetHealth();
          if not health:IsDead() then
				    health:Damage(abilityOwnerId, (FireBallExplosion.damage * FireBallExplosion.damageIncrease) * entity:GetStatChange():GetDamageResistance(), "FireBall");
			    end
			    Static.KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), FireBallExplosion.pushback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth()); 
			end
		end
	end
end
function FireBallExplosion.OnDestroy (self)
	Entity.Remove(self);
end

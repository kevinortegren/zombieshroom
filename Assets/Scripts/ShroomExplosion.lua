ShroomExplosion = {};
ShroomExplosion.damage = 60;
ShroomExplosion.pushback = 5;
ShroomExplosion.cooldown = 0;
ShroomExplosion.chargeTime = 0.0;
ShroomExplosion.channelingTime = 0.0;
ShroomExplosion.duration = 1.0;

function ShroomExplosion.OnLoad()
	ResourceManager.LoadParticle("Explosion_G-stuf");
	ResourceManager.LoadSound("Abilities/Mine/mineexplode1-1.wav");
end

function ShroomExplosion.OnCreate (userId, actionId)
	local self = Entity.New();
	local fatherEntity = Entity.GetEntityByNetworkID(userId, actionId, 1);
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
	local scriptComp = Script.New(self, "ShroomExplosion");
	TimerEntity.StartTimer(userId, actionId, ShroomExplosion.duration, "ShroomExplosion", "OnDestroy", self);
	--Scalable.New(self, 20, 400);
	collisionComp:CreateHandle(self, 1, true);
	transformComp:SetPos(posVec);
	transformComp:SetScale(Vec3.New(10,10,10));
	colRespComp:SetContainer(collisionComp);
	--Logging.Log(LogLevel.DEBUG_PRINT, "ShroomExplosion handle: "..collisionComp:GetHandle());
	physicsComp:BindNoShape(collisionComp, posVec, Quat.New(0,0,0,1));
	--Logging.Log(LogLevel.DEBUG_PRINT, "After bind call");
	physicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), 10);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "Explosion_G-stuf");
		--local renderComp = Renderable.New(self);
		--renderComp:SetModel("Primitives/sphere");
		--renderComp:SetMaterial("ExplosiveMat");
		--renderComp:SetShadowTechnique(ShadowTechnique.SHADOW_NONE);
		--renderComp:SetMaterialEffect("Mesh_Refractive_Clear");
	end
end

function ShroomExplosion.OnCollide (self, entity)
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
				    health:Damage(abilityOwnerId, ShroomExplosion.damage * entity:GetStatChange():GetDamageResistance(), "ExplodingShroom");
			    end
			   	Static.KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), ShroomExplosion.pushback * entity:GetStatChange():GetKnockbackResistance(), health:GetHealth());
				Static.PlaySound("Abilities/Mine/mineexplode1-1.wav", 1.0, selfPos, 10.0, 100.0, bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));

			end
		end
	end
end
function ShroomExplosion.OnDestroy (self)
	Entity.Remove(self);
end

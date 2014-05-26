HealCharges = {};
HealCharges.damage = 0;
HealCharges.knockback = 0;
HealCharges.cooldown = 0;
HealCharges.chargeTime = 0;
HealCharges.channelingTime = 0;
HealCharges.duration = 2;
HealCharges.charges = 1;
HealCharges.crosshair = "crosshairNone";

function HealCharges.OnLoad()
	ResourceManager.LoadParticle("HealCharges");
	ResourceManager.LoadSound("Abilities/heal1-1.wav");
end

function HealCharges.ChargeStart(userId, actionId)
end

function HealCharges.ChargeDone (time, userId, actionId)
	HealCharges.OnCreate(userId, actionId);
end

function HealCharges.ChannelingDone (time, userId, actionId)
	--HealCharges.OnDestroy(Entity.GetEntityByNetworkID(userId, actionId, 0));
end

function HealCharges.Interrupted (time, userId, actionId)
end

function HealCharges.EndEffect(self)
	self:GetParticleEmitter():SetAlive(-1.0);
end

function HealCharges.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "HealCharges");
	TimerEntity.StartTimer(userId, actionId, HealCharges.duration, "HealCharges", "EndEffect", self);
	TimerEntity.StartTimer(userId, actionId, HealCharges.duration*2, "HealCharges", "OnDestroy", self);
	Follower.New(self, casterEnt, 0);
	--Setting stuff
	local tempPos = casterEnt:GetTransformation():GetPos();
  
	transformComp:SetPos(tempPos);
  
  local health = casterEnt:GetHealth();
  health:IncreaseHealth(50);
	
  if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "HealCharges");
		Static.PlaySound("Abilities/heal1-1.wav", 1.0, tempPos, 10.0, 100.0, bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));

  end
end

function HealCharges.OnCollide (self, entity)
end

function HealCharges.OnDestroy (self)
	Entity.Remove(self);
end

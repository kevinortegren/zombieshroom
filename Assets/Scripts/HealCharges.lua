HealCharges = {};
HealCharges.damage = 0;
HealCharges.knockback = 0;
HealCharges.cooldown = 6;
HealCharges.charges = 0;
HealCharges.chargeTime = 0;
HealCharges.channelingTime = 0;
HealCharges.duration = 1;
HealCharges.charges = 3;

function HealCharges.OnLoad()
	ResourceManager.LoadParticle("HealCharges");
end

function HealCharges.ChargeDone (time, userId, actionId)
	HealCharges.OnCreate(userId, actionId);
end

function HealCharges.ChannelingDone (time, userId, actionId)
	HealCharges.OnDestroy(Entity.GetEntityByNetworkID(userId, actionId, 0));
end

function HealCharges.Interrupted (time, userId, actionId)
end

function HealCharges.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "HealCharges");
	TimerEntity.StartTimer(userId, actionId, HealCharges.duration, "HealCharges", "OnDestroy", self);
	Follower.New(self, casterEnt, 0);
	--Setting stuff
	local tempPos = casterEnt:GetTransformation():GetPos();
  
	transformComp:SetPos(tempPos);
  
  local health = casterEnt:GetHealth();
  health:IncreaseHealth(25);
	
  if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "HealCharges");
  end
end

function HealCharges.OnCollide (self, entity)
end

function HealCharges.OnDestroy (self)
	Entity.Remove(self);
end

Boost = {};
Boost.damage = 0;
Boost.knockback = 0;
Boost.cooldown = 0;
Boost.charges = 1;
Boost.chargeTime = 0;
Boost.channelingTime = 0;
Boost.duration = 5;
Boost.crosshair = "crosshairNone";

function Boost.OnLoad()
	ResourceManager.LoadSound("Abilities/powerup-speedrun1-1.wav");
end

function Boost.ChargeStart(userId, actionId)
end

function Boost.ChargeDone (time, userId, actionId)
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local statComp = casterEnt:GetStatChange();
	statComp:SetSpeed(1.5);
	statComp:SetJumpHeight(1.5);
	TimerEntity.StartTimer(userId, actionId, Boost.duration, "Boost", "Reset", casterEnt);
	Static.PlaySound("Abilities/powerup-speedrun1-1.wav", 0.5, casterEnt:GetTransformation():GetPos(), 10.0, 100.0, bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
end

function Boost.Reset(self)
	local statComp = self:GetStatChange();
	statComp:SetSpeed(1);
	statComp:SetJumpHeight(1);
end

function Boost.ChannelingDone (time, userId, actionId)
end

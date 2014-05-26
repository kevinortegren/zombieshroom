
--Set table name
AbilityDash = {};
AbilityDash.cooldown = 3;
AbilityDash.chargeTime = 0.0;
AbilityDash.channelingTime = 0.0;
AbilityDash.power = 25;
AbilityDash.crosshair = "crosshairNone";

function AbilityDash.OnLoad()
	ResourceManager.LoadSound("Movement/JumpingOneshots/jump2-7.wav", bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
end

function AbilityDash.ChargeStart(userId, actionId)
end

function AbilityDash.ChargeDone(time, userId, actionId)
end

function AbilityDash.ChannelingDone(time, userId, actionId)

	--Animation clip
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetUpperAnimClip(AnimClip.DASH, true);
	Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetAnimation():SetLowerAnimClip(AnimClip.DASH, true);

	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local aimingEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1);
	local frontVec  = aimingEnt:GetTransformation():GetOrient():GetFront();

	local handle = playerEnt:GetCollision():GetHandle();
	
    Static.KnockBack(handle, Vec3.New(0,0.4,0)+frontVec, AbilityDash.power, 100);

    Static.Play3DSound("Movement/JumpingOneshots/jump2-7.wav", 1.0, playerEnt:GetTransformation():GetPos(), 10.0, 100.0);
end

function AbilityDash.Interrupted(time, userId, actionId)
end
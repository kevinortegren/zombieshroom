AbilitySpawnPoint = {}

function AbilitySpawnPoint.OnLoad()
  --This function is not triggered...
  ResourceManager.LoadSound("Feedback/switch1-1.wav");
end

function AbilitySpawnPoint.OnCreate (userId, actionId)
end

function AbilitySpawnPoint.OnCollide (self, entity)
  local hitCol = entity:GetCollision();
  local type = hitCol:GetType();
   --Logging.Log(LogLevel.DEBUG_PRINT, "Entity collided");
  	if type == PhysicsType.TYPE_PLAYER then
      local tryPickup = entity:GetTryPickupComponent();
      --Logging.Log(LogLevel.DEBUG_PRINT, "collided with player! TryPickup: "..tostring(tryPickup:GetTryPickup()));
      if tryPickup:GetTryPickup() then
        local abilitySpawn = self:GetAbilitySpawn();
        local playerId = entity:GetNetwork():GetUserId();
        abilitySpawn:SetClaimed(playerId);
        tryPickup:SetTryPickup(false);
        Logging.Log(LogLevel.DEBUG_PRINT, "Pick up successful");
        Static.PlaySound("Feedback/switch1-1.wav", 1.0, self:GetTransformation():GetPos(), 10.0, 100.0, bit32.bor(SoundMode.SOUND_LOOP_OFF, SoundMode.SOUND_3D, SoundMode.SOUND_3D_LINEARSQUAREROLLOFF));
      end
    end
end

function AbilitySpawnPoint.OnDestroy (self)
	self:RemoveCollision();
end
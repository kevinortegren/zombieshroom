AbilitySpawnPoint = {}

function AbilitySpawnPoint.OnCreate (userId, actionId)
end

function AbilitySpawnPoint.OnCollide (self, entity)
  local hitCol = entity:GetCollision();
  local hitPhys = entity:GetPhysics();
  local type = hitPhys:GetType(hitCol);
   Logging.Log(LogLevel.DEBUG_PRINT, "Entity collided");
  	if type == PhysicsType.TYPE_PLAYER then
      Logging.Log(LogLevel.DEBUG_PRINT, "collided with player!");
      local action = entity:GetPlayerAction();
      if action:TryPickup() then
        local abilitySpawn = self:GetAbilitySpawn();
        local playerId = entity:GetNetwork():GetUserId();
        abilitySpawn:SetClaimed(playerId);
        action:SetTryPickup(false);
        Logging.Log(LogLevel.DEBUG_PRINT, "Pick up succesfull");
      end
    end
end
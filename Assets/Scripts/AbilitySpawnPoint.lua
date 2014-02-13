
function AbilitySpawnPoint.OnCreate (userId, actionId)

end

function AbilitySpawnPoint.OnCollide (self, entity)
  local hitCol = entity:GetCollision();
  local hitPhys = entity:GetPhysics();
  local type = hitPhys:GetType(hitCol);
  
  	if type == PhysicsType.TYPE_PLAYER then
      local action = self:GetPlayerAction();
      local abilitySpawn = self:GetAbilitySpawn();
      local playerId = entity:GetNetwork():GetUserId();
      action:SetTryPickup(true);
      abilitySpawn:SetClaimed(playerId);
    end
end
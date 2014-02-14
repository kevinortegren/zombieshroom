
function AbilitySpawnPoint.OnCreate (userId, actionId)

end

function AbilitySpawnPoint.OnCollide (self, entity)
  local hitCol = entity:GetCollision();
  local hitPhys = entity:GetPhysics();
  local type = hitPhys:GetType(hitCol);
  
  	if type == PhysicsType.TYPE_PLAYER then
      local action = self:GetPlayerAction();
      if action:TryPickup() then
        local abilitySpawn = self:GetAbilitySpawn();
        local playerId = entity:GetNetwork():GetUserId();
        abilitySpawn:SetClaimed(playerId);
        action:SetTryPickup(false);
      end
    end
end
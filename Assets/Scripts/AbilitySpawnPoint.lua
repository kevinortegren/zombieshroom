AbilitySpawnPoint = {}

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
        Static.Play3DSound("CC-BY3.0/sfx_fly.wav", 1.0, self:GetTransformation():GetPos(), 10.0, 100.0);
      end
    end
end


function AbilitySpawnPoint.OnDestroy (self)
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end
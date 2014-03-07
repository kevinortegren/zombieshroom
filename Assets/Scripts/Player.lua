Player = {}

function Player.OnCreate(userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating player (userId: "..tostring(userId)..", actionId: "..tostring(actionId)..")");
	
	local player = Entity.New();
	local playerComponent = PlayerComponent.New(player);
 	local network = Network.New(player, userId, actionId);
	local transform = Transformation.New(player);
	local script = Script.New(self, "Player");
	
	transform:SetPos(Vec3.New(100,10,0));

	playerComponent:SetAbility(3, "Push", -1);
	playerComponent:SelectAbility(0);
 	playerComponent:SetDeaths(0);
	playerComponent:SetScore(0);
	playerComponent:SetTeamId(0);
	
  Entity.RegisterGroup("NonExport", player);

  -- Create an aiming device
  local aimingEntity = Entity.New();
  local aimingTransform = Transformation.New(aimingEntity);
  local aimingNetwork = Network.New(aimingEntity, userId, actionId);

	Entity.RegisterGroup("NonExport", aimingEntity);
  
	if Global.UserID == userId then
		local playerControl = PlayerControl.New(player);
		playerControl:SetMouseSensitivity(0.2);
		Entity.RegisterTag("Player", player);
		Entity.RegisterTag("AimingDevice", aimingEntity);
	end
end

function Player.OnTeamSelect(self, teamId)
  local playerComponent = self:GetPlayerComponent();
	local prevTeamId = playerComponent:GetTeamId();
	playerComponent:SetTeamId(teamId);
	
  if prevTeamId == 0 and teamId ~= 0 then
    local playerPhysics = PlayerPhysics.New(self);
    local health = Health.New(self);
    local physics = Physics.New(self);
    local collision = Collision.New(self);
    local collisionResponder = CollisionResponder.New(self);
    local statComp = StatChange.New(self);
    local playerAction = PlayerAction.New(self);
    local stateComponent = StateComponent.New(self);
    local tryPickup = TryPickupComponent.New(self);
		
		local transform = self:GetTransformation();

    stateComponent:SetPreviousPosition(transform:GetPos());
    stateComponent:SetCurrentState(EntityState.DESCENDING);
		
		playerComponent:SetDeaths(0);
		playerComponent:SetScore(0);

    playerAction:SetJumpTime(0.0);
    playerAction:SetMovePower(0);
    playerAction:SetStrafePower(0);
    playerAction:SetAngle(Vec2.New(0, 0));
    playerAction:SetAbilityTime(0.0);
    playerAction:SelectAbility(0);
		playerAction:SetWantRespawn(true);

    playerPhysics:SetMovementSpeed(25);
    playerPhysics:SetJumpForce(10); --Do not fucking change without good reason or I will hunt you down //Kim
    playerPhysics:SetJumpBoostForce(30); --See comment above //Kim I do what i want //Wingly

    collision:SetMeshHandle("testchar0");
    Collision.AddPlayerObjectToWorld(self, collision, transform, playerPhysics, collisionResponder);
    
    if Global.IsClient then
      local renderable = Renderable.New(self);
      local animation = Animation.New(self);
      local ragdoll = Ragdoll.New(self);
      renderable:SetPass(RenderPass.RENDERPASS_DEFAULT);
      renderable:SetShadowTechnique(ShadowTechnique.SHADOW_ANIMATED);
      renderable:SetModel("testchar");
      if teamId == 1 then
        renderable:SetMaterial("BlueSpirit");
        renderable:SetMaterialGlow("WSGlowBlue");
      else
        renderable:SetMaterial("RedSpirit");
        renderable:SetMaterialGlow("WSGlowRed");
      end
      renderable:SetMaterialDiffuse("WStexture");
      renderable:SetMaterialSpecular("WSSpecular");
      renderable:SetMaterialNormal("WSNormal");
      renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
      renderable:SetAnimation(animation);
      local waterCollider = WaterCollider.New(self);
      waterCollider:SetDisturbPower(0.5);
      waterCollider:SetDisturbInterval(0.5);
      waterCollider:SetRadius(5);
    end
  elseif teamId == 0 and prevTeamId ~= 0 then
		self:RemovePlayerPhysics();
		self:RemoveHealth();
		self:RemovePhysics();
		Collision.RemoveObjectFromWorld(self:GetCollision());
		self:RemoveCollision();
		self:RemoveCollisionResponder();
		self:RemoveScript();
		self:RemovePlayerAction();
		self:RemoveStateComponent();
		self:RemoveTryPickupComponent();
		self:RemoveRenderable();
		self:RemoveAnimation();
		self:RemoveRagdoll();
		self:RemoveWaterCollider();
  elseif teamId ~= prevTeamId then
		local renderable = self:GetRenderable();
		local health = self:GetHealth();
		
		playerComponent:SetDeaths(0);
		playerComponent:SetScore(0);
		
    health:SetHealth(0);
    health:SetIsDead(true);
		
		if teamId == 1 then
			renderable:SetMaterial("BlueSpirit");
			renderable:SetMaterialGlow("WSGlowBlue");
		else
			renderable:SetMaterial("RedSpirit");
			renderable:SetMaterialGlow("WSGlowRed");
		end
		renderable:SetMaterialDiffuse("WStexture");
		renderable:SetMaterialSpecular("WSSpecular");
		renderable:SetMaterialNormal("WSNormal");
		renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
  end
end

function Player.OnCollide (self, entity)
 	--local hitCol = entity:GetCollision();
 	--local hitPhys = entity:GetPhysics();
	--local type = hitPhys:GetType(hitCol);
  --if  type == PhysicsType.TYPE_ABILITYSPAWN then
  --end
  
end

function Player.OnDestroy (self)
	local network = self:GetNetwork();
	Logging.Log(LogLevel.DEBUG_PRINT, "Destroying player (userId: "..tostring(network:GetUserId())..", actionId: "..tostring(network:GetActionId())..")");
	Entity.Remove(self);
end
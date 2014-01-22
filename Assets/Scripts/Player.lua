Player = {}

function Player.OnCreate(self, userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating player");
	local transform = Transformation.New(self);
	local playerPhysics = PlayerPhysics.New(self);
	local health = Health.New(self);
	local player = PlayerComponent.New(self);
	local physics = Physics.New(self);
	local collision = Collision.New(self);
	local collisionResponder = CollisionResponder.New(self);
	local script = Script.New(self, "Player");
	local playerAction = PlayerAction.New(self);
	local stateComponent = StateComponent.New(self);
	local network = Network.New(self, userId, actionId);

	-- TODO: Decide where to put spawn logic
	transform:SetPos(Vec3.New(100,10,0));

	stateComponent:SetPreviousPosition(transform:GetPos());
	stateComponent:SetCurrentState(EntityState.DESCENDING);

	playerAction:SetJump(false);
	playerAction:SetMovePower(0);
	playerAction:SetStrafePower(0);
	playerAction:SetAngle(Vec2.New(0, 0));
	playerAction:SetActivateAbility(false);
	playerAction:SelectAbility(1);

	player:SetAbility(0, "AbilityTest");
	player:SelectAbility(0);

	playerPhysics:SetMovementSpeed(10);
	playerPhysics:SetJumpForce(20);

	physics:SetMass(5);

	collision:SetMeshHandle("testchar0");
	Collision.AddPlayerObjectToWorld(self, collision, transform, physics, playerPhysics, collisionResponder);

	health:SetHealth(100);
	health:SetIsDead(false);
	health:SetWantsRespawn(false);
	player:SetDeaths(0);
	player:SetScore(0);
	-- ToDo: Get and set a correct team id
	player:SetTeamId(0);

	Entity.RegisterGroup("NonExport", self);

	-- Create an aiming device
	local aimingEntity = Entity.New();
	local aimingTransform = Transformation.New(aimingEntity);
	local aimingNetwork = Network.New(aimingEntity, userId, actionId);

	Entity.RegisterGroup("NonExport", aimingEntity);
	-- ToDo: Move AimingDevice tag to local player only
	Entity.RegisterTag("AimingDevice", aimingEntity);
end

function Player.AddClientComponents(self)
	local renderable = Renderable.New(self);
	local animation = Animation.New(self);

	renderable:SetPass(RenderPass.RENDERPASS_DYNAMIC);
	renderable:SetModel("testchar");
	renderable:SetMaterial("testchar");
	renderable:SetMaterialDiffuse("WStexture");
	renderable:SetMaterialSpecular("WSSpecular");
	renderable:SetMaterialNormal("WSNormal");
	renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
	renderable:SetAnimation(animation);
end

function Player.AddLocalClientComponents(self)
	local playerControl = PlayerControl.New(self);

	playerControl:SetMouseSensitivity(0.3);

	Entity.RegisterTag("Player", self);
end

function Player.OnCollide (self, entity)
	-- Logging.Log(LogLevel.DEBUG_PRINT, "Entity collided");
end

function Player.OnDestroy ()
	
end
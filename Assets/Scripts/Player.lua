Player = {}

function Player.OnCreate(self, userId, actionId)
	local transform = Transformation.New(self);
	local playerPhysics = PlayerPhysics.New(self);
	local health = Health.New(self);
	local player = PlayerComponent.New(self);
	local physics = Physics.New(self);
	local collision = Collision.New(self);
	local collisionResponder = CollisionResponder.New(self);
	local script = Script.New(self, "Player");
	local playerAction = PlayerAction.New(self);
	local network = Network.New(self, userId, actionId);

	-- TODO: Decide where to put spawn logic
	player:SetAbility(0, "AbilityTest");
	player:SelectAbility(0);

	playerPhysics:SetMovementSpeed(10);
	playerPhysics:SetJumpForce(20);

	physics:SetMass(5);

	-- Create an aiming device
	local aimingEntity = Entity.New();
	local aimingTransform = Transformation.New(aimingEntity);
	local aimingNetwork = Network.New(aimingEntity, userId, actionId);
end

function Player.AddClientComponents(self)
	local renderable = Renderable.New(self);
	local animation = Animation.New(self);

	renderable:SetModel("testchar");
	renderable:SetMaterial("testchar");
	renderable:SetMaterialDiffuse("WStexture");
	renderable:SetMaterialSpecular("WSSpecular");
	renderable:SetMaterialNormal("WSNormal");
	renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
	renderable:SetAnimation(animation);
end

function Player.AddLocalClientComponents(self)
	
end

function Player.OnCollide (self, entity)
	Logging.Log("Entity collided");
end

function Player.OnDestroy ()
	
end
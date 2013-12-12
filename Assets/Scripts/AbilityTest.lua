-- C API

-- #ENTITY# --
-- CreateEntity()

-- #COMPONENT# --
-- CreateRenderable()
------ SetRenderableModel(string modelHandle, string effectHandle, Entity entity)
------ SetRenderableMaterial() - UNDER CONSTRUCTION
-- CreateTransformation()
------ SetTransformationPos(double x, double y, double z) - UNDER CONSTRUCTION
-- CreatePhysicsAccessor()
------ SetPhysicsAccessorInfo(PhysicsAccessor physiccomp, bool collideWorld, double dirX, double dirY, double dirZ, double gravX, double gravY, double gravZ, double orientX, double orientY, double orientZ, double orientW, double posX, double posY, double posZ, double height, double mass, double radius, double shape, double speed, double type)
---------- shape -> 0 = SHAPE_SPHERE, 1 = SHAPE_CONE, 2 = SHAPE_CYLINDER
---------- type  -> 0 = TYPE_STATIC, 1 = TYPE_ABILITY, 2 = TYPE_DYNAMIC, 3 = TYPE_PLAYER

--[[
	print( type(renderComp) )

	local meta = getmetatable(renderComp)
	for k,v in pairs(meta) do
	  print("    ", k, v)
	end
--]]

AbilityTest = {}
AbilityTest.cooldown = 5.0;

function AbilityTest.OnActivate (action)

	local entity 		= Entity.New();
	local playerEntity  = Entity.GetEntityByTag("Player");
	local playerTrans   = playerEntity:GetTransformation();
	x, y, z = playerTrans:GetPos();
	local renderComp 	= Renderable.New(entity);
	local transform 	= Transformation.New(entity);
	transform:SetPos(x, y, z);
	local physics 		= PhysicsAccessor.New(entity);
	renderComp:SetModel("Primitives/sphere", "Mesh");
	physics:SetInfo(
		true, --collideWorld
		0, --dirx
		1, --diry
		1, --dirz
		0, --gravx
		-9, --gravy
		0, --gravz
		0, --orientX
		0, --orientY
		0, --orientZ
		0, --orientW
		x, --posX
		y+1, --posY
		z+2, --posZ
		0.5, --height
		3, --mass
		1, --radius
		0, --shape, 0 = SHAPE_SPHERE, 1 = SHAPE_CONE, 2 = SHAPE_CYLINDER
		15, --speed
		1); --type, 0 = TYPE_STATIC, 1 = TYPE_ABILITY, 2 = TYPE_DYNAMIC, 3 = TYPE_PLAYER
end

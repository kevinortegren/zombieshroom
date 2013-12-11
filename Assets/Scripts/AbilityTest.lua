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

Ent = {}
function AbilityTestOnActivate ()
	--local entity 		= Meta.New();
	local renderComp 	= Ent.New();
	--renderComp.SetModel("Primitives/sphere", "Mesh");
end

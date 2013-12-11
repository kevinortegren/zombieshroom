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


function AbilityTestOnActivate ()
	local entity 		= CreateEntity();
	local transcomp 	= CreateTransformation(entity);
	local rendercomp 	= CreateRenderable(entity);
	SetRenderableModel("Primitives/sphere", "Mesh", rendercomp);
	local physiccomp	= CreatePhysicsAccessor(entity);
	SetPhysicsAccessorInfo(physiccomp, true, --[[dirX]]0, --[[dirY]]0, --[[dirZ]]1, --[[gravX]]0, --[[gravY]]-9, --[[gravZ]]0, --[[orientX]]0, --[[orientY]]0, --[[orientZ]]0, --[[orientW]]0, --[[posX]]0, --[[posY]]0, --[[posZ]]0, --[[height]]0.5, --[[mass]]1, --[[radius]]1, --[[shape]]0, --[[speed]]50, --[[type]]1);
end

function AbilityTestOnCollision (ent1, ent2)

end

function AbilityTestOnDestroy (ent1)

end

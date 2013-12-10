
function FireballOnActivate ()
	local entity 		= CreateEntity();
	local transcomp 	= CreateTransformation(entity);
	local rendercomp 	= CreateRenderable(entity);
	SetRenderableModel("sphere", "Mesh", rendercomp);
	local physiccomp	= CreatePhysicsAccessor(entity);
	SetPhysicsAccessorInfo(physiccomp, 
		true, --collideWorld
		0, --dirx
		1, --diry
		-1, --dirz
		0, --gravx
		-9, --gravy
		0, --gravz
		0, --orientX
		0, --orientY
		0, --orientZ
		0, --orientW
		0, --posX
		0, --posY
		-10, --posZ
		0.5, --height
		1, --mass
		1, --radius
		0, --shape, 0 = SHAPE_SPHERE, 1 = SHAPE_CONE, 2 = SHAPE_CYLINDER
		50, --speed
		1); --type, 0 = TYPE_STATIC, 1 = TYPE_ABILITY, 2 = TYPE_DYNAMIC, 3 = TYPE_PLAYER
end

function FireballOnCollision (ent1, ent2)

end

function FireballOnDestroy (ent1)

end
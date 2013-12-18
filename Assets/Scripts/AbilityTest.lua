--[[
	print( type(renderComp) )

	local meta = getmetatable(renderComp)
	for k,v in pairs(meta) do
	  print("    ", k, v)
	end
--]]

--Set table name
AbilityTest = {};

function AbilityTest.OnActivate (action)

	--Create entity
	local entity 		= Entity.New();

	--Get data from Player & AimingDevice entities
	local posVec  		= Entity.GetEntityByTag("Player"):GetTransformation():GetPos();
	local frontVec  	= Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetFront();

	--Create components and put them in our new entity
	local renderComp 	= Renderable.New(entity);
	local collisionComp = Collision.New(entity);
	local physicsComp 	= Physics.New(entity);
	local transformComp	= Transformation.New(entity);

	--Set data in our new components
	transformComp:SetPos(posVec);
	
	renderComp:SetModel("Primitives/sphereTangents");
	renderComp:SetMaterial("Fireball");
	renderComp:SetMaterialDiffuse("fireballDiffuse");
	renderComp:SetMaterialSpecular("fireballSpecular");
	renderComp:SetMaterialNormal("fireballNormal");
	renderComp:SetMaterialEffect("Mesh_NormalMap");
	
	physicsComp:SetInfo(
		collisionComp,
		true, --collideWorld
		frontVec.x, --dirx
		frontVec.y, --diry
		frontVec.z, --dirz
		0, --gravx
		-9.82, --gravy
		0, --gravz
		1, --orientX
		0, --orientY
		-1, --orientZ
		0, --orientW
		posVec.x + 		frontVec.x * 3, --posX
		4 + posVec.y + 	frontVec.y * 3, --posY
		posVec.z + 		frontVec.z * 3, --posZ
		entity:GetId(),
		0.5, --height
		3, --mass
		1, --radius
		0, --shape, 0 = SHAPE_SPHERE, 1 = SHAPE_CONE, 2 = SHAPE_CYLINDER
		40, --speed
		1); --type, 0 = TYPE_STATIC, 1 = TYPE_ABILITY, 2 = TYPE_DYNAMIC, 3 = TYPE_PLAYER
end

--[[
	print( type(renderComp) )

	local meta = getmetatable(renderComp)
	for k,v in pairs(meta) do
	  print("    ", k, v)
	end
--]]
ACTION_CREATE = 0;
ACTION_COLLIDE = 1;

--Set table name
AbilityTest = {};

function AbilityTest.Process (action, ...)
	
	local args = table.pack(...);

	if action == ACTION_CREATE then
		AbilityTest.OnCreate();
	elseif action == ACTION_COLLIDE then
		AbilityTest.OnCollide(args);
	end
	
end

function AbilityTest.OnCreate ()

	--Get data from Player & AimingDevice entities
	local posVec  		= Entity.GetEntityByTag("Player"):GetTransformation():GetPos();
	local frontVec  	= Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetFront();

	--Create entity
	local entity 		= Entity.New();

	--Create components and put them in our new entity
	local renderComp 	= Renderable.New(entity);
	local collisionComp = Collision.New(entity);
	local colRespComp	= CollisionResponder.New(entity);
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
	
	collisionComp:CreateHandle(entity:GetId(), 1, false);
	physicsComp:BindShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 30, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 40, frontVec.y * 40, frontVec.z * 40));
end

function AbilityTest.OnCollide (args)
end

function AbilityTest.OnDestroy ()
end
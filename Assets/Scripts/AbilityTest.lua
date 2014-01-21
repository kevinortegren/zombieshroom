ACTION_CREATE = 0;
ACTION_COLLIDE = 1;
ACTION_DESTROY = 2;

AbilityTest = {};

function AbilityTest.Process (action, ...)

	local args = table.pack(...);

	if action == ACTION_CREATE then
		AbilityTest.OnCreate();
	elseif action == ACTION_COLLIDE then
		AbilityTest.OnCollide(args);
	elseif action == ACTION_DESTROY then
		AbilityTest.OnDestroy(args);
	end
end

function AbilityTest.OnCreate ()
	local playerEnt = Entity.GetEntityByTag("Player");
	local posVec = Entity.GetEntityByTag("Player"):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetOrient():GetFront();

	local entity0 = Entity.New();

	local renderComp0 = Renderable.New(entity0);
	local collisionComp0 = Collision.New(entity0);
	local colRespComp0 = CollisionResponder.New(entity0);
	local physicsComp0 = Physics.New(entity0);
	collisionComp0:CreateHandle(entity0:GetId(), 1, false);
	local transformComp0 = Transformation.New(entity0);
	local scriptComp0 = Script.New(entity0, "AbilityTest");
	transformComp0:SetPos(posVec);
	renderComp0:SetModel("Primitives/sphereTangents");
	renderComp0:SetMaterial("Fireball");
	renderComp0:SetMaterialDiffuse("fireballDiffuse");
	renderComp0:SetMaterialSpecular("fireballSpecular");
	renderComp0:SetMaterialNormal("fireballNormal");
	renderComp0:SetMaterialEffect("Mesh_NormalMap");
	physicsComp0:BindShape(collisionComp0, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 5, true);
	physicsComp0:SetVelocity(collisionComp0, Vec3.New(frontVec.x * 20, frontVec.y * 20, frontVec.z * 20));
	colRespComp0:SetContainer(collisionComp0);
end

function AbilityTest.OnCollide (args)
end

function AbilityTest.OnDestroy (args)
end


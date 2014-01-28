AbilityBall = {};

function AbilityBall.OnCreate (userId, actionId)
	local self = Entity.New();
	local playerEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local posVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);

	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	collisionComp:CreateHandle(self, 1, false);
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "AbilityBall");
	physicsComp:BindSphereShape(collisionComp, Vec3.New((posVec.x + frontVec.x * 3), (2 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 5, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(frontVec.x * 50, frontVec.y * 50, frontVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	colRespComp:SetContainer(collisionComp);
	transformComp:SetPos(posVec);
	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Fireball");
		renderComp:SetMaterialDiffuse("fireballDiffuse");
		renderComp:SetMaterialSpecular("fireballSpecular");
		renderComp:SetMaterialNormal("fireballNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
	end
end

function AbilityBall.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	if type == 3 then
		local hitPos = entity:GetTransformation():GetPos();
		local selfPos = self:GetTransformation():GetPos();
		hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), 20);
	end
end

function AbilityBall.OnDestroy (self)
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end

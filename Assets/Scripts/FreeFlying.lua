--Free flying controls script. This is what a game-specific component should look like!

FreeFlying = {};
FreeFlying.MovementSpeed = 20.0;
FreeFlying.RotateSpeed = 45.0;

function FreeFlying.Setup(self)
	
	local controllerActions = self:GetControllerActions();
	controllerActions:Bind("Forward", 82);
	controllerActions:Bind("Back", 81);
	controllerActions:Bind("Left", 80);
	controllerActions:Bind("Right", 79);
	controllerActions:Bind("PitchUp", 96);
	controllerActions:Bind("PitchDown", 90);
	controllerActions:Bind("YawLeft", 92);
	controllerActions:Bind("YawRight", 94);

end

function FreeFlying.OnUpdate(self)
	local dt = Static.GetDeltaTime();
	local transform = self:GetTransformation();
	local controllerActions = self:GetControllerActions();

	if controllerActions:IsActivated("Forward") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetFront() * FreeFlying.MovementSpeed * dt);
	end
	if controllerActions:IsActivated("Back") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetFront() * -FreeFlying.MovementSpeed * dt);
	end
	if controllerActions:IsActivated("Left") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetRight() * -FreeFlying.MovementSpeed * dt);
	end
	if controllerActions:IsActivated("Right") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetRight() * FreeFlying.MovementSpeed * dt);
	end

	if controllerActions:IsActivated("PitchUp") then
		transform:GetOrient():Pitch(FreeFlying.RotateSpeed * dt);
	end
	if controllerActions:IsActivated("PitchDown") then
		transform:GetOrient():Pitch(-FreeFlying.RotateSpeed * dt);
	end
	if controllerActions:IsActivated("YawLeft") then
		transform:GetOrient():YawGlobal(FreeFlying.RotateSpeed * dt);
	end
	if controllerActions:IsActivated("YawRight") then
		transform:GetOrient():YawGlobal(-FreeFlying.RotateSpeed * dt);
	end

	local mouseSensitivity = 0.1;
--	local playerEntity = Entity.GetEntityByTag("Player");
--	if playerEntity ~= nil then
--		local playerControl = playerEntity:GetPlayerControl();
--		if playerControl ~= nil then
--			mouseSensitivity = playerControl:GetMouseSensitivity();
--		end
--	end

	local mouseDelta = Static.GetMouseDelta();
	transform:GetOrient():YawGlobal(-mouseDelta.x * mouseSensitivity);
	transform:GetOrient():Pitch(mouseDelta.y * mouseSensitivity);
end
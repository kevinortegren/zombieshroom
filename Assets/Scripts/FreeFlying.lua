--Free flying controls script. This is what a game-specific component should look like!

FreeFlying = {};
FreeFlying.MovementSpeed = 1.0;
FreeFlying.RotateSpeed = 1.0;

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
	local transform = self:GetTransformation();

	local controllerActions = self:GetControllerActions();

	if controllerActions:IsActivated("Forward") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetFront() * FreeFlying.MovementSpeed);
	end
	if controllerActions:IsActivated("Back") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetFront() * -FreeFlying.MovementSpeed);
	end
	if controllerActions:IsActivated("Left") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetRight() * -FreeFlying.MovementSpeed);
	end
	if controllerActions:IsActivated("Right") then
		transform:SetPos(transform:GetPos() + transform:GetOrient():GetRight() * FreeFlying.MovementSpeed);
	end

	if controllerActions:IsActivated("PitchUp") then
		transform:GetOrient():Pitch(FreeFlying.RotateSpeed);
	end
	if controllerActions:IsActivated("PitchDown") then
		transform:GetOrient():Pitch(-FreeFlying.RotateSpeed);
	end
	if controllerActions:IsActivated("YawLeft") then
		transform:GetOrient():YawGlobal(FreeFlying.RotateSpeed);
	end
	if controllerActions:IsActivated("YawRight") then
		transform:GetOrient():YawGlobal(-FreeFlying.RotateSpeed);
	end
end
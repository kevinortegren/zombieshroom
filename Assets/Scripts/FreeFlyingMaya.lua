--Free flying controls script. This is what a game-specific component should look like!

FreeFlyingMaya = {};
FreeFlyingMaya.MoveSpeed = 20.0;
FreeFlyingMaya.Displacement = 100.0;
FreeFlyingMaya.RotateSens = 0.1;
FreeFlyingMaya.PanSens = 0.08;
FreeFlyingMaya.ZoomSens = 0.05;
FreeFlyingMaya.Time = 1.1;
FreeFlyingMaya.From = nil;
FreeFlyingMaya.To = nil;

function FreeFlyingMaya.Setup(self)
	
	local controllerActions = self:GetControllerActions();
	controllerActions:Bind("Forward", 82);
	controllerActions:Bind("Back", 81);
	controllerActions:Bind("Left", 80);
	controllerActions:Bind("Right", 79);
	controllerActions:Bind("PitchUp", 96);
	controllerActions:Bind("PitchDown", 90);
	controllerActions:Bind("YawLeft", 92);
	controllerActions:Bind("YawRight", 94);
	controllerActions:Bind("Alt", 226);
	controllerActions:Bind("LeftClick", 490);
	controllerActions:Bind("MiddleClick", 491);
	controllerActions:Bind("RightClick", 492);
    controllerActions:Bind("F", 9);
    controllerActions:Bind("ScrollForward", 0);
    controllerActions:Bind("ScrollBackward", 0);


end

function FreeFlyingMaya.Target(target)

   FreeFlyingMaya.From = Entity.GetEntityByTag("AimingDevice")
   FreeFlyingMaya.To = target
   FreeFlyingMaya.Time = 0       

end

function FreeFlyingMaya.OnUpdate(self)
	local dt = Static.GetDeltaTime();

	local speedZoomFactor = FreeFlyingMaya.Displacement / 50.0;

    if FreeFlyingMaya.Time <= 1.0 then   
    
        FreeFlyingMaya.Time = FreeFlyingMaya.Time + dt
        
        -- lerp
        FreeFlyingMaya.From:GetTransformation():SetPos(Vec3.New(1.0 - FreeFlyingMaya.Time, 1.0 - FreeFlyingMaya.Time, 1.0 - FreeFlyingMaya.Time) * FreeFlyingMaya.From:GetTransformation():GetPos() + Vec3.New(FreeFlyingMaya.Time, FreeFlyingMaya.Time, FreeFlyingMaya.Time) * FreeFlyingMaya.To:GetTransformation():GetPos())       
    else             
        FreeFlyingMaya.From = nil
        FreeFlyingMaya.To = nil
    end
    
	local transform = self:GetTransformation();
	local controllerActions = self:GetControllerActions();
	
	local aimTransform = Entity.GetEntityByTag("AimingDevice"):GetTransformation();
	
	local worldDisplace = aimTransform:GetOrient():GetFront() * Vec3.New(-FreeFlyingMaya.Displacement, -FreeFlyingMaya.Displacement, -FreeFlyingMaya.Displacement);

	transform:SetPos(aimTransform:GetPos() + worldDisplace);

	if controllerActions:IsActivated("Alt") then
	
		local mouseDelta = Static.GetMouseDelta();
		
		if controllerActions:IsActivated("LeftClick") then
		
			aimTransform:GetOrient():YawGlobal(mouseDelta.x * FreeFlyingMaya.RotateSens);
			aimTransform:GetOrient():Pitch(-mouseDelta.y * FreeFlyingMaya.RotateSens);
		
		elseif controllerActions:IsActivated("MiddleClick") then
		
			local panDeltaY = -mouseDelta.y * FreeFlyingMaya.PanSens;
			local panDeltaX = mouseDelta.x * FreeFlyingMaya.PanSens;
		
			aimTransform:SetPos(aimTransform:GetPos() + transform:GetOrient():GetUp() * Vec3.New(panDeltaY, panDeltaY, panDeltaY));
			aimTransform:SetPos(aimTransform:GetPos() + transform:GetOrient():GetRight() * Vec3.New(panDeltaX, panDeltaX, panDeltaX));
		
		elseif controllerActions:IsActivated("RightClick") then

			if math.abs(mouseDelta.y) > math.abs(mouseDelta.x) then
				FreeFlyingMaya.Displacement = FreeFlyingMaya.Displacement + mouseDelta.y * FreeFlyingMaya.ZoomSens * speedZoomFactor;
			else
				FreeFlyingMaya.Displacement = FreeFlyingMaya.Displacement + mouseDelta.x * FreeFlyingMaya.ZoomSens * speedZoomFactor;
			end

		end
        
	end

	FreeFlyingMaya.Displacement = FreeFlyingMaya.Displacement - Static.GetScroll() * speedZoomFactor;
    
end
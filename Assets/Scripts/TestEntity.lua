TestEntity = {}

function TestEntity.OnCreate(self, userId, actionId)
    local transform = Transformation.New(playerEntity);
    local script = Script.New(playerEntity, "TestEntity");
end

function TestEntity.AddClientComponents(self)

end

function TestEntity.AddLocalClientComponents(self)
	
end
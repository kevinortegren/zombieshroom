-- Used for unit testing. Please do not remove. Used in UnitTesting/Network.cpp.

TestEntity = {}

function TestEntity.OnCreate(self, userId, actionId)
    local transform = Transformation.New(self);
    local script = Script.New(self, "TestEntity");
end

function TestEntity.AddClientComponents(self)
	
end

function TestEntity.AddLocalClientComponents(self)
	
end
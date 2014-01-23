-- Used for unit testing. Please do not remove. Used in UnitTesting/Network.cpp.

TestEntity = {}

function TestEntity.OnCreate(userId, actionId)
	local self = Entity.New();

    local transform = Transformation.New(self);
    local script = Script.New(self, "TestEntity");
	local network = Network.New(self, userId, actionId);
end
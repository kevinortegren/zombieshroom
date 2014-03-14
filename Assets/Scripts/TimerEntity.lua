TimerEntity = {}

function TimerEntity.StartTimer(userId, actionId, duration, scriptName, functionName, target)
	local timerEntity = TimerEntity.OnCreate(userId, actionId);
	local timerComponent = timerEntity:GetTimer();
	timerComponent:Set(duration, scriptName, functionName, target);
end

function TimerEntity.OnCreate(userId, actionId)
	local self = Entity.New();

    local script = Script.New(self, "TimerEntity");
	local network = Network.New(self, userId, actionId);

	local timerComponent = Timer.New(self);
	return self;
end

function TimerEntity.OnDestroy(self)
	Entity.Remove(self);
end
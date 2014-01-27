MatchState = {}

function MatchState.OnCreate(userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating match state");
	
	local matchState = Entity.New();
	local script = Script.New(matchState, "MatchState");
	local network = Network.New(matchState, userId, actionId);
  local rules = TDMRuleSet.New(matchState);
  
  -- Set default values for score limit and time left
  rules:SetScoreLimit(0);
	rules:SetTimeLeft(20*60);
  -- Initialize the team scores
	rules:SetTeamScore(0, -1); -- Spectator team score should never be used
	rules:SetTeamScore(1, 0);
	rules:SetTeamScore(2, 0);

	Entity.RegisterGroup("NonExport", matchState);
  
  Entity.RegisterTag("MatchState", matchState);
end

function MatchState.OnDestroy (self)
	Logging.Log(LogLevel.DEBUG_PRINT, "Match state destroyed");
end
-- Used for unit testing. Please do not remove. Used in UnitTesting/Script.cpp.

TestTable = {}
TestTable.TestVariable = 10;

function TestTable.TestSetVariable()
    if TestTable.TestVariable == 5 then
        Logging.Log(LogLevel.SUCCESS, "Global variable set!");
    else
        Logging.Log(LogLevel.NON_FATAL_ERROR, "Global variable not set! Current value: " .. TestTable.TestVariable);
    end
end

function TestTable.TestGetVariable()
	TestTable.TestVariable = 25;
end
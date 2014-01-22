-- Used for unit testing. Please do not remove. Used in UnitTesting/Script.cpp.

Global = {
    TestVariable = 10
}

function Test()
    if TestVariable == 5 then
        Logging.Log(LogLevel.SUCCESS, "Global variable set!");
    else
        Logging.Log(LogLevel.NON_FATAL_ERROR, "Global variable not set!");
    end
    
end
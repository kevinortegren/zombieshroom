local result, errorStr = loadfile("C:\\Users\\BTH\\Documents\\GitHub\\zombieshroom\\Assets\\Scripts\\Explosion.lua")
if result == nil then
    -- Compilation failed, print the error message.
    print(errorStr)
else
    -- Compilation was successful, run the code.
    result()
end
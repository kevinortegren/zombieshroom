function PrintSomeData (p_bool, p_number, p_string)
	print("[LUA] Message via print")
	io.write("[LUA] Message via io.write")
	print(p_bool)
	print(p_number)
	CreateThing()
end
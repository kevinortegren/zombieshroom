#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <stdexcept>
#include <exception>

#include <UnitTesting.h>

#undef main

RootEngine::GameSharedContext g_engineContext;

TEST(Main, EngineInit) 
{
	void* m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");
	EXPECT_NE(m_engineModule, nullptr);

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");
	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, "");
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "UnitTesting.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		testing::InitGoogleTest(&argc, argv);

		int result = RUN_ALL_TESTS();
		std::cin.get();
		return result;
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
		//std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		//std::cin.get();
		return 1;
	}
	
	return 0;
}
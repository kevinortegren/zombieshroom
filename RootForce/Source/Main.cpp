
#include <Main.h>

#include <stdexcept>

#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Vertex.h>


#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <ECS/Tests/TestSystem.h>

#include <exception>

#include <glm/glm.hpp>

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Rootforce.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			testing::InitGoogleTest(&argc, argv);

			int result = RUN_ALL_TESTS();
			std::cin.get();
			return result;
		}
		else
		{
			Main m(path);
			m.Start();
		}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
		std::cout << e.what() << "\n";
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		// TODO: Log unknown exception message
		std::cin.get();
		return 1;
	}
	
	return 0;
}

Main::Main(std::string p_workingDirectory) 
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");
	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_RENDER | RootEngine::SubsystemInit::INIT_PHYSICS, p_workingDirectory);

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
	{
		// TODO: Log error and throw exception (?)
	}

	// TODO: Make these parameters more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
			"Root Force",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1280,
			720,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
		SDL_DestroyWindow);
	if (m_window == nullptr) 
	{
		// TODO: Log error and throw exception (?)
	}

	m_engineContext.m_renderer->SetupSDLContext(m_window.get());
	m_engineContext.m_resourceManager->LoadEffect("test");
	m_engineContext.m_resourceManager->LoadCollada("testchar");
}

Main::~Main() 
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{
	int numVertices = 3;
	Render::Vertex1P1N realVertices[3];
	realVertices[0].m_pos = glm::vec3(0.5f, 0.5f, 1.f); realVertices[0].m_normal = glm::vec3(0.f, 0.f, 1.f);
	realVertices[1].m_pos = glm::vec3(-0.5f, 0.5f, 1.f); realVertices[1].m_normal = glm::vec3(0.f, 0.0f, 1.f);
	realVertices[2].m_pos = glm::vec3(-0.5f, -0.5f, 1.f); realVertices[2].m_normal = glm::vec3(0.f, 0.f, 1.f);
	int numIndices = 3;
	GLuint indices[] = {
		0, 1, 2
	};

	std::shared_ptr<Render::MeshInterface> mesh = m_engineContext.m_renderer->CreateMesh();
	mesh->Init(realVertices, 3, indices, numIndices);

	Render::Uniforms uniforms;
	uniforms.m_normal = glm::mat4(1);
	uniforms.m_world = glm::mat4(1);
	
	
	Render::RenderJob job;
	job.m_mesh = m_engineContext.m_resourceManager->GetModel("testchar")->m_meshes[0];
	job.m_uniforms = &uniforms;
	job.m_effect = m_engineContext.m_resourceManager->GetEffect("test");

	int facesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfFaces;
	int verticesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfVertices;
	int indicesTotal = m_engineContext.m_resourceManager->GetModel("testchar")->numberOfIndices;
	float* tempVertices = (float*)malloc(verticesTotal * 3 * sizeof(float));
	for(int i = 0; i < verticesTotal; i ++)
	{
			tempVertices[i*3] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].x;  // 0, 3, 6, 9
			tempVertices[i*3 + 1] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].y; //1, 4, 7, 10
			tempVertices[i*3 + 2] = m_engineContext.m_resourceManager->GetModel("testchar")->meshPoints[i].z;  //2,5,8,11  	
	}
	int* tempIndices = (int*)malloc(indicesTotal * sizeof(int));
	tempIndices = (int*)&m_engineContext.m_resourceManager->GetModel("testchar")->meshIndices[0];

	float pos[3] = {0,0,0};
	int handle = m_engineContext.m_physics->AddDynamicObjectToWorld(facesTotal, &tempIndices[0], 3 * sizeof(int), verticesTotal, &tempVertices[0], 3*sizeof(float), pos, pos,1.0f );
	

	float angle = 0.0f;

	uint64_t old = SDL_GetPerformanceCounter();
	while (m_running)
	{
		uint64_t now = SDL_GetPerformanceCounter();
		float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
		old = now;

		HandleEvents();

		// TODO: Poll and handle events
		// TODO: Update game state
		// TODO: Render and present game
	
		angle += 90.0f*dt;
		uniforms.m_world = glm::rotate<float>(glm::mat4(1.0f), angle, 0.0f, 1.0f, 0.0f);
		uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));
		m_engineContext.m_renderer->AddRenderJob(&job);
	//	testfuncofawesome(m_engineContext.m_physics->GetDebugVectors());
		m_engineContext.m_physics->Update();
		//m_engineContext.m_renderer->DrawLine(m_engineContext.m_physics->GetDebugVectors());
		
		m_engineContext.m_renderer->Render();
		
	}
}
void Main::testfuncofawesome(std::vector<glm::vec3> p_debugVectors)
{
	m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Entered testfunofawesome");
	int size  = p_debugVectors.size();
		if(size <= 0)
		{

			m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Breaking testfunc");
			return;
		}
		
		
		Render::Vertex1P1C* vertices = new Render::Vertex1P1C();
		unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * size);

		/*m_debugVectors.push_back(from);
		m_debugVectors.push_back(to);
		m_debugVectors.push_back(color);*/

		m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "the for ");
		for(int i = 0; i < size; i++)
		{
			m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Iteration %d of %d ", i, size);

			vertices[i].m_pos = p_debugVectors[i];
			vertices[i].m_color = glm::vec4(1.0f, 0, 0, 1.0f);
			indices[i] = i;
		}

		m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "after for ");
		//vertices[0].m_pos = p_pos1;
		//vertices[0].m_color = glm::vec4(p_colour, 1.0f);
		//vertices[1].m_pos = p_pos2;
		//vertices[1].m_color = glm::vec4(p_colour, 1.0f);

		

		Render::Uniforms uniforms;
		uniforms.m_normal = glm::mat4(1);
		uniforms.m_world = glm::mat4(1);
		m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "testfunofawesome creating mesh ");
		std::shared_ptr<Render::MeshInterface> mesh = m_engineContext.m_renderer->CreateMesh();

		m_engineContext.m_logger->LogText(LogTag::PHYSICS, LogLevel::DEBUG_PRINT, "Leaving testfunofawesome");
		//mesh->Init(vertices, size, indices, size);
}
void Main::HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYDOWN:
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
		}
	}
}



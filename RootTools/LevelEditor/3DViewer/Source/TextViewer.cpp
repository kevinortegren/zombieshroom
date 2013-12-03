#include "stdafx.h"
#include "ReadMemory.h"
#include "RootEngine\Include\RootEngine.h"
#include "Utility\DynamicLoader\Include\DynamicLoader.h"

#include <stdexcept>
#include <exception>
#include <glm/glm.hpp>

#include <Utility\ECS\Include\World.h>

#include <memory>

#include <RootEngine/Include/ModelImporter.h>
//#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/EffectImporter.h>

#include <RootTools\LevelEditor\3DViewer\Include\RawMeshPrimitives.h>

#undef main

bool m_running;
void* m_engineModule;
std::shared_ptr<SDL_Window> m_window;
RootEngine::GameSharedContext m_engineContext;

void HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type) 
		{
		case SDL_QUIT:
			m_running = false;
			break;

		//default:
			//if (m_engineContext.m_inputSys != nullptr)
			//	m_engineContext.m_inputSys->HandleInput(event);
			//if (m_engineContext.m_gui != nullptr)
			//	m_engineContext.m_gui->HandleEvent(event);
		}
	}
}

int main(int argc, char* argv[]) 
{
	std::string path(argv[0]);
	std::string rootforcename = "Level3DViewer.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	try 
	{
		if (argc > 1 && strcmp(argv[1], "-test") == 0)
		{
			//testing::InitGoogleTest(&argc, argv);

			//int result = RUN_ALL_TESTS();
			std::cin.get();
			//return result;
		}
		else
		{
			m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

			INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

			m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, path);

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
			m_running = true;

			m_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); 

			// Cube mesh.
			std::shared_ptr<Render::Mesh> cubeMesh = m_engineContext.m_renderer->CreateMesh();
			Utility::Cube cube(Render::VertexType::VERTEXTYPE_1P);
			cubeMesh->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
			cubeMesh->m_elementBuffer = m_engineContext.m_renderer->CreateBuffer();
			cubeMesh->m_vertexAttributes = m_engineContext.m_renderer->CreateVertexAttributes();
			cubeMesh->CreateIndexBuffer(cube.m_indices, cube.m_numberOfIndices);
			cubeMesh->CreateVertexBuffer1P(reinterpret_cast<Render::Vertex1P*>(cube.m_vertices), cube.m_numberOfVertices);

			m_engineContext.m_resourceManager->LoadEffect("Mesh");

			Render::Material material;
			material.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");

			Render::RenderJob job;
			job.m_mesh = cubeMesh;
			job.m_material = &material;

			// Start the main loop
			uint64_t old = SDL_GetPerformanceCounter();
			while (m_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				HandleEvents();
				// TODO: Update game state
				// TODO: Render and present game
				m_engineContext.m_renderer->Clear();

				m_engineContext.m_renderer->AddRenderJob(job);
				m_engineContext.m_renderer->Render();

				m_engineContext.m_renderer->Swap();
			}

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
	
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);

	return 0;
}



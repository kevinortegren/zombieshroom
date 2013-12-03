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

ReadMemory RM;
void LoadScene()
{

}

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
			RM.InitalizeSharedMemory();

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

			std::shared_ptr<Render::Mesh> Mesh[g_maxMeshes];

			Render::RenderJob job[g_maxMeshes];
			
					

			m_engineContext.m_resourceManager->LoadEffect("Mesh");

			Render::Material material;
			material.m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
			
			int numberMeshes;

			//Load existing objects
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

			numberMeshes = *RM.NumberOfMeshes;

			ReleaseMutex(RM.IdMutexHandle);

			for(int i = 0; i < numberMeshes; i++)
			{
				Render::Vertex* m_vertices;

				Mesh[i] = m_engineContext.m_renderer->CreateMesh();

				RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
				WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

				m_vertices = new Render::Vertex1P[RM.PmeshList[i]->nrOfVertices];

				for(int j = 0; j < RM.PmeshList[i]->nrOfVertices; j++)
				{
					m_vertices[j].m_pos = RM.PmeshList[i]->vertex[j];
				}

				Mesh[i]->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
				Mesh[i]->m_vertexAttributes = m_engineContext.m_renderer->CreateVertexAttributes();
				Mesh[i]->CreateVertexBuffer1P(reinterpret_cast<Render::Vertex1P*>(m_vertices), RM.PmeshList[i]->nrOfVertices);

				ReleaseMutex(RM.MeshMutexHandle);
			}



			// Start the main loop
			uint64_t old = SDL_GetPerformanceCounter();
			while (m_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

				if(*RM.MeshIdChange != -1)
				{
					Render::Vertex* m_vertices;
					Mesh[*RM.MeshIdChange] = m_engineContext.m_renderer->CreateMesh();

					RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
					WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

					m_vertices = new Render::Vertex1P[RM.PmeshList[*RM.MeshIdChange]->nrOfVertices];

						for(int i = 0; i < RM.PmeshList[*RM.MeshIdChange]->nrOfVertices; i++)
						{
							m_vertices[i].m_pos = RM.PmeshList[*RM.MeshIdChange]->vertex[i];
						}

					Mesh[*RM.MeshIdChange]->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
					Mesh[*RM.MeshIdChange]->m_vertexAttributes = m_engineContext.m_renderer->CreateVertexAttributes();
					Mesh[*RM.MeshIdChange]->CreateVertexBuffer1P(reinterpret_cast<Render::Vertex1P*>(m_vertices), RM.PmeshList[*RM.MeshIdChange]->nrOfVertices);

					*RM.MeshIdChange = -1;
					ReleaseMutex(RM.MeshMutexHandle);
					numberMeshes = *RM.NumberOfMeshes;
				}

				ReleaseMutex(RM.IdMutexHandle);

				HandleEvents();

				m_engineContext.m_renderer->Clear();
				
				for(int i = 0; i < numberMeshes; i ++)
				{
					if(Mesh[i]->m_vertexBuffer != nullptr)
					{
						job[i].m_mesh = Mesh[i];
						job[i].m_material = &material;

						m_engineContext.m_renderer->AddRenderJob(job[i]);
					}
				}

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

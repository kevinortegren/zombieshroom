#include "stdafx.h"
#include "ReadMemory.h"
#include "RootEngine\Include\RootEngine.h"
#include "Utility\DynamicLoader\Include\DynamicLoader.h"

#include <stdexcept>
#include <exception>
#include <glm/glm.hpp>
#include <memory>

#include <Utility\ECS\Include\World.h>
#include <Utility\ECS\Include\Shared\RenderingSystem.h>
#include <Utility\ECS\Include\Shared\LightSystem.h>

#include <RootEngine/Include/ResourceManager/ResourceManager.h>

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
ECS::Entity* CreateLightEntity(ECS::World* p_world)	
{
	ECS::Entity* lightEntity = p_world->GetEntityManager()->CreateEntity();

	RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(lightEntity);
	
	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(lightEntity);

	RootForce::PointLight* pl = p_world->GetEntityManager()->CreateComponent<RootForce::PointLight>(lightEntity);
	pl->m_color.a = 1;
	pl->m_color.r = 0.5;
	pl->m_color.b = 0.5;
	pl->m_color.g = 0.5;
	pl->m_range = 1000;
	pl->m_attenuation = glm::vec3(0,0,0.01);

	return lightEntity;
}
//CREATE CAMERA ENTITY

ECS::Entity* CreateMeshEntity(ECS::World* p_world)
{
	ECS::Entity* entity = p_world->GetEntityManager()->CreateEntity();

	RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);
	renderable->m_model = new RootEngine::Model;
	renderable->m_model->m_meshes.resize(1);
	renderable->m_model->m_meshes[0] = m_engineContext.m_renderer->CreateMesh();
	
	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);

	return entity;
}

std::string GetNameFromPath( std::string p_path )
 {
  std::string cutPath;
  std::string::size_type slashIndex, dotIndex;

  // Extract the file name
  cutPath  = p_path;
  slashIndex = cutPath.find_last_of("/")+1;
  if(slashIndex == 0)
   slashIndex = cutPath.find_last_of("\\")+1;
  cutPath  = cutPath.substr(slashIndex, cutPath.size());
  dotIndex = cutPath.find_last_of(".");
  cutPath  = cutPath.substr(0, dotIndex);
  return cutPath;
 } 
int main(int argc, char* argv[]) 
{
	// Enable components to use.
	RootForce::Renderable::SetTypeId(0);
    RootForce::Transform::SetTypeId(1);
    RootForce::PointLight::SetTypeId(2);

	// Setup world.
	ECS::World m_world(3);

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
			Render::DirectionalLight dl;
			dl.m_color = glm::vec4(0,0,1,1);

			// Initialize systems.
			RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
			renderingSystem->SetLoggingInterface(m_engineContext.m_logger);
			renderingSystem->SetRendererInterface(m_engineContext.m_renderer);
			m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

			RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, m_engineContext.m_renderer);
			m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

			std::vector<ECS::Entity*> LightEntities;
			std::vector<ECS::Entity*> Entities;

			/*Entities.push_back(CreateEntity(&m_world));*/

			//m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[0]);
			//m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[0]);

			//renderable->m_model

			std::shared_ptr<Render::Mesh> Mesh[g_maxMeshes];

			//Render::RenderJob job[g_maxMeshes];		

			m_engineContext.m_resourceManager->LoadEffect("Mesh");

			m_engineContext.m_resourceManager->LoadTexture("sphere_diffuse.dds");

			//Render::Material material;
			Render::Material* materials;
			materials = new Render::Material[10];	//Satte ett fast väre på 10 här.
			materials[0].m_diffuseMap = m_engineContext.m_resourceManager->GetTexture("sphere_diffuse.dds");
			materials[0].m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
			
			int numberMeshes;
			int numberLights;

			Render::Vertex1P1N1UV* m_vertices;
			m_vertices = new Render::Vertex1P1N1UV[g_maxVerticesPerMesh];


			//Load existing objects

			///////////////////////LOAD MESHES////////////////////////////////
			RM.MeshIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.MeshIdMutexHandle, RM.milliseconds);

			numberMeshes = *RM.NumberOfMeshes;
			*RM.MeshIdChange = -1;
			std::string OldtempTexName;
			std::string tempTexName;
			int numberOfMaterials = 1;

			ReleaseMutex(RM.MeshIdMutexHandle);
			
			for(int i = 0; i < numberMeshes; i++)
			{
				//Render::Vertex* m_vertices;
				Entities.push_back(CreateMeshEntity(&m_world));
				
				RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
				WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_position = RM.PmeshList[i]->transformation.position;
				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_scale = RM.PmeshList[i]->transformation.scale;

				tempTexName = GetNameFromPath(RM.PmeshList[i]->texturePath)+".dds";	//Verkar ta in en jävla lampa och kamera ibland, dafaq.
				if(tempTexName == "NONE.dds")
				{
				RM.PmeshList[i]->MaterialID = 0;
				}
				else
				{
				m_engineContext.m_resourceManager->LoadTexture(tempTexName);
				materials[numberOfMaterials].m_diffuseMap = m_engineContext.m_resourceManager->GetTexture(tempTexName);
				materials[numberOfMaterials].m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
				numberOfMaterials ++;
				RM.PmeshList[i]->MaterialID = numberOfMaterials;
				}

				glm::quat rotation;
				rotation.x = RM.PmeshList[i]->transformation.rotation.x;
				rotation.y = RM.PmeshList[i]->transformation.rotation.y;
				rotation.z = RM.PmeshList[i]->transformation.rotation.z;
				rotation.w = RM.PmeshList[i]->transformation.rotation.w;

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_orientation.SetOrientation(rotation);

				//m_vertices = new Render::Vertex1P[RM.PmeshList[i]->nrOfVertices];

				//m_vertices = new Render::Vertex1P1N1UV[RM.PmeshList[i]->nrOfVertices];

				for(int j = 0; j < RM.PmeshList[i]->nrOfVertices; j++)
				{
					m_vertices[j].m_pos = RM.PmeshList[i]->vertex[j];
					m_vertices[j].m_normal = RM.PmeshList[i]->normal[j];
					m_vertices[j].m_UV = RM.PmeshList[i]->UV[j];

				}

				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->m_vertexAttributes =  m_engineContext.m_renderer->CreateVertexAttributes();
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(reinterpret_cast<Render::Vertex1P1N1UV*>(m_vertices), RM.PmeshList[i]->nrOfVertices); 
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_material = materials[RM.PmeshList[i]->MaterialID];

				ReleaseMutex(RM.MeshMutexHandle);

			}

			///////////////////////Load Lights////////////////////////////////
			RM.LightIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.LightIdMutexHandle, RM.milliseconds);

			numberLights = *RM.NumberOfLights;
			*RM.LightIdChange = -1;

			ReleaseMutex(RM.LightIdMutexHandle);

			
			for(int i = 0; i < numberLights; i++)
			{
				//Render::Vertex* m_vertices;
				LightEntities.push_back(CreateLightEntity(&m_world));
				
				RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
				WaitForSingleObject(RM.LightMutexHandle, RM.milliseconds);

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[i])->m_position = RM.PlightList[i]->transformation.position;
				//m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[i])->m_scale = RM.PlightList[i]->transformation.scale;

				//glm::quat rotation;
				//rotation.x = RM.PlightList[i]->transformation.rotation.x;
				//rotation.y = RM.PlightList[i]->transformation.rotation.y;
				//rotation.z = RM.PlightList[i]->transformation.rotation.z;
				//rotation.w = RM.PlightList[i]->transformation.rotation.w;

				ReleaseMutex(RM.LightMutexHandle);
			}


			// Start the main loop
			uint64_t old = SDL_GetPerformanceCounter();
			while (m_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				RM.MeshIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.MeshIdMutexHandle, RM.milliseconds);

				int MeshIndex = *RM.MeshIdChange;
				*RM.MeshIdChange = -1;
				ReleaseMutex(RM.MeshIdMutexHandle);

				///////////////////////UPDATE MESHES////////////////////////////////
				if(MeshIndex != -1)					
				{
					bool newMaterial = false;
					//Render::Vertex* m_vertices
					int size = Entities.size()-1;
					if(MeshIndex > size)
					{
						Entities.push_back(CreateMeshEntity(&m_world));
					}
				
					RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");		
					WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_position = RM.PmeshList[MeshIndex]->transformation.position;
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_scale = RM.PmeshList[MeshIndex]->transformation.scale;

					tempTexName = GetNameFromPath(RM.PmeshList[MeshIndex]->texturePath)+".dds";	//Verkar ta in en jävla lampa och kamera ibland, dafaq.
						if(tempTexName == "NONE.dds")
						{
						RM.PmeshList[MeshIndex]->MaterialID = 0;
						}
						else
						{
						m_engineContext.m_resourceManager->LoadTexture(tempTexName);
						materials[numberOfMaterials].m_diffuseMap = m_engineContext.m_resourceManager->GetTexture(tempTexName);
						materials[numberOfMaterials].m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
						numberOfMaterials ++;
						RM.PmeshList[MeshIndex]->MaterialID = numberOfMaterials;
					}

					glm::quat rotation;
					rotation.x = RM.PmeshList[MeshIndex]->transformation.rotation.x;
					rotation.y = RM.PmeshList[MeshIndex]->transformation.rotation.y;
					rotation.z = RM.PmeshList[MeshIndex]->transformation.rotation.z;
					rotation.w = RM.PmeshList[MeshIndex]->transformation.rotation.w;
				
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_orientation.Rotate(rotation);

					//m_vertices = new Render::Vertex1P[RM.PmeshList[MeshIndex]->nrOfVertices];
					for(int j = 0; j < RM.PmeshList[MeshIndex]->nrOfVertices; j++)
					{
						m_vertices[j].m_pos = RM.PmeshList[MeshIndex]->vertex[j];
						m_vertices[j].m_normal = RM.PmeshList[MeshIndex]->normal[j];
						m_vertices[j].m_UV = RM.PmeshList[MeshIndex]->UV[j];

					}

					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->m_vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->m_vertexAttributes =  m_engineContext.m_renderer->CreateVertexAttributes();
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(reinterpret_cast<Render::Vertex1P1N1UV*>(m_vertices), RM.PmeshList[MeshIndex]->nrOfVertices); 
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_material = materials[RM.PmeshList[MeshIndex]->MaterialID];	////SÄTTER DET SÅ DET ÄR ETT MATERIAL PER MESH

					//delete [] m_vertices;
					ReleaseMutex(RM.MeshMutexHandle);

				}

				///////////////////////UPDATE LIGHTS////////////////////////////////
				RM.LightIdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.LightIdMutexHandle, RM.milliseconds);

				int LightIndex = *RM.LightIdChange;
				*RM.LightIdChange = -1;
				ReleaseMutex(RM.LightIdMutexHandle);

				if(LightIndex != -1)					
				{
					//Render::Vertex* m_vertices
					int size = LightEntities.size()-1;
					if(LightIndex > size)
					{
						LightEntities.push_back(CreateLightEntity(&m_world));
					}
				
					RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
					WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_position = RM.PlightList[LightIndex]->transformation.position;
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_scale = RM.PlightList[LightIndex]->transformation.scale;

					glm::quat rotation;
					rotation.x = RM.PlightList[LightIndex]->transformation.rotation.x;
					rotation.y = RM.PlightList[LightIndex]->transformation.rotation.y;
					rotation.z = RM.PlightList[LightIndex]->transformation.rotation.z;
					rotation.w = RM.PlightList[LightIndex]->transformation.rotation.w;

					//delete [] m_vertices;
					ReleaseMutex(RM.LightMutexHandle);

				}

				HandleEvents();

				m_engineContext.m_renderer->Clear();

				pointLightSystem->Process();
				renderingSystem->Process();

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

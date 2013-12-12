#include "stdafx.h"
#include "ReadMemory.h"
#include "RootEngine\Include\RootEngine.h"
#include "Utility\DynamicLoader\Include\DynamicLoader.h"

#include <stdexcept>
#include <exception>
#include <glm/glm.hpp>
#include <memory>

#include <RootSystems\Include\RenderingSystem.h>
#include <RootSystems\Include\LightSystem.h>
#include <RootSystems\Include\CameraSystem.h>

#include <Utility\ECS\Include\World.h>

#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootTools\LevelEditor\3DViewer\Include\RawMeshPrimitives.h>

#undef main

bool g_running;
void* g_engineModule;
std::shared_ptr<SDL_Window> g_window;
RootEngine::GameSharedContext g_engineContext;
std::vector<std::shared_ptr<Render::MeshInterface>> g_meshes;


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
			g_running = false;
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
	
	g_meshes.push_back(g_engineContext.m_renderer->CreateMesh());
	renderable->m_model->m_meshes[0] = g_meshes[g_meshes.size() - 1].get();

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
    RootForce::Camera::SetTypeId(3);

	// Setup world.
	ECS::World m_world;

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

			g_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

			INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(g_engineModule, "InitializeEngine");

			g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_ALL, path);

			if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) 
			{
				// TODO: Log error and throw exception (?)
			}

			// TODO: Make these parameters more configurable.
			g_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
					"Root Force",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					1280,
					720,
					SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN),
				SDL_DestroyWindow);
			if (g_window == nullptr) 
			{
				// TODO: Log error and throw exception (?)
			}

			g_engineContext.m_renderer->SetupSDLContext(g_window.get());
			g_running = true;

			g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); 
			Render::DirectionalLight dl;
			dl.m_color = glm::vec4(0,0,1,1);

			// Initialize systems.
			RootForce::RenderingSystem* renderingSystem = new RootForce::RenderingSystem(&m_world);
			renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
			renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
			m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

			RootForce::PointLightSystem* pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
			m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

			RootForce::CameraSystem* cameraSystem = new RootForce::CameraSystem(&m_world);
			m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem, "CameraSystem");

			std::vector<ECS::Entity*> cameras;
			std::vector<ECS::Entity*> LightEntities;
			std::vector<ECS::Entity*> Entities;

			cameras.push_back(m_world.GetEntityManager()->CreateEntity());
			RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameras[0]);
			RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameras[0]);
			//camera->m_near = 0.1f;
			//camera->m_far = 1000.0f;
			//camera->m_fov = 75.0f;
			m_world.GetTagManager()->RegisterEntity("Camera", cameras[0]);

			//cameraTransform->m_orientation.SetOrientation(glm::quat(0,1,0,0));

			cout << "Default Camera Created" << endl;
			cout << "Position: "		<< cameraTransform->m_position.x << " "
				<< cameraTransform->m_position.y << " "
				<< cameraTransform->m_position.z << endl;
			cout << "Rotation: "		<< cameraTransform->m_orientation.GetQuaterion().x << " "
				<< cameraTransform->m_orientation.GetQuaterion().y << " "
				<< cameraTransform->m_orientation.GetQuaterion().z << " "
				<< cameraTransform->m_orientation.GetQuaterion().w << endl;
			cout << "FieldOfView "		<< camera->m_fov << endl;

			/*Entities.push_back(CreateEntity(&m_world));*/

			//m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[0]);
			//m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[0]);

			//renderable->m_model

			std::shared_ptr<Render::Mesh> Mesh[g_maxMeshes];

			//Render::RenderJob job[g_maxMeshes];		

			g_engineContext.m_resourceManager->LoadEffect("Mesh");

			g_engineContext.m_resourceManager->LoadTexture("sphere_diffuse", Render::TextureType::TEXTURE_2D);

			//Render::Material material;
			Render::Material* materials;
			materials = new Render::Material[g_maxMeshes];	//Satte ett fast värde på 100 HÄR

			materials[0].m_diffuseMap = g_engineContext.m_resourceManager->GetTexture("sphere_diffuse");
			materials[0].m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");
			
			int numberMeshes;
			int numberLights;

			Render::Vertex1P1N1UV* m_vertices;
			m_vertices = new Render::Vertex1P1N1UV[g_maxVerticesPerMesh];


			//Load existing objects

			///////////////////////LOAD MESHES////////////////////////////////
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

			numberMeshes = *RM.NumberOfMeshes;
			*RM.MeshIdChange = glm::vec2(-1, -1);
			std::string OldtempTexName;
			std::string tempTexName;
			

			ReleaseMutex(RM.IdMutexHandle);

			RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
			WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

			//int renderNrOfMaterials = *RM.NumberOfMaterials;

			//for(int i = 0; i < renderNrOfMaterials; i++)
			//{
			//	materials[i].m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(RM.PmaterialList[i]->texturePath);
			//	//materials[i].m_normalMap = m_engineContext.m_resourceManager->GetTexture(RM.PmaterialList[i]->normalPath);
			//	materials[i].m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh");
			//	cout << "Material: " << RM.PmaterialList[i]->texturePath << " added to index: " << i << endl;
			//}

			for(int i = 0; i < numberMeshes; i++)
			{
				//Render::Vertex* m_vertices;
				Entities.push_back(CreateMeshEntity(&m_world));	


				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_position = RM.PmeshList[i]->transformation.position;
				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_scale = RM.PmeshList[i]->transformation.scale;


				//tempTexName = GetNameFromPath(RM.PmeshList[i]->texturePath)+".dds";	//Verkar ta in en jävla lampa och kamera ibland, dafaq.
				//if(tempTexName == "NONE.dds")
				//{
					RM.PmeshList[i]->MaterialID = 0;
				//}
				//else
				//{
				//	m_engineContext.m_resourceManager->LoadTexture(tempTexName);
				//	materials[numberOfMaterials].m_diffuseMap = m_engineContext.m_resourceManager->GetTexture(tempTexName);
				//	materials[numberOfMaterials].m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
				//	RM.PmeshList[i]->MaterialID = numberOfMaterials;
				//	numberOfMaterials ++;
				//	
				//}

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

				auto model = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model;
				auto mesh = model->m_meshes[0];
				auto buffer = g_engineContext.m_renderer->CreateBuffer();
				mesh->SetVertexBuffer(buffer);

				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer());
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(reinterpret_cast<Render::Vertex1P1N1UV*>(m_vertices), RM.PmeshList[i]->nrOfVertices); 
				m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_material = materials[RM.PmeshList[i]->MaterialID];

				ReleaseMutex(RM.MeshMutexHandle);

			}

			///////////////////////Load Lights////////////////////////////////
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

			numberLights = *RM.NumberOfLights;
			*RM.LightIdChange = glm::vec2(-1,-1);

			ReleaseMutex(RM.IdMutexHandle);

			
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
			while (g_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

				int MeshIndex = RM.MeshIdChange->x;
				int RemoveMeshIndex = RM.MeshIdChange->y;
				*RM.MeshIdChange = glm::vec2(-1, -1);
				ReleaseMutex(RM.IdMutexHandle);

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

					//tempTexName = GetNameFromPath(RM.PmeshList[MeshIndex]->texturePath)+".dds";	//Verkar ta in en jävla lampa och kamera ibland, dafaq.
					//if(tempTexName == "NONE.dds")
					//{
						RM.PmeshList[MeshIndex]->MaterialID = 0;
					//}
					//else
					//{
					//	m_engineContext.m_resourceManager->LoadTexture(tempTexName);
					//	materials[numberOfMaterials].m_diffuseMap = m_engineContext.m_resourceManager->GetTexture(tempTexName);
					//	materials[numberOfMaterials].m_effect = m_engineContext.m_resourceManager->GetEffect("Mesh");
					//	RM.PmeshList[MeshIndex]->MaterialID = numberOfMaterials;
					//	numberOfMaterials ++;
					//}

					glm::quat rotation;
					rotation.x = RM.PmeshList[MeshIndex]->transformation.rotation.x;
					rotation.y = RM.PmeshList[MeshIndex]->transformation.rotation.y;
					rotation.z = RM.PmeshList[MeshIndex]->transformation.rotation.z;
					rotation.w = RM.PmeshList[MeshIndex]->transformation.rotation.w;
				
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_orientation.SetOrientation(rotation);

					//m_vertices = new Render::Vertex1P[RM.PmeshList[MeshIndex]->nrOfVertices];
					for(int j = 0; j < RM.PmeshList[MeshIndex]->nrOfVertices; j++)
					{
						m_vertices[j].m_pos = RM.PmeshList[MeshIndex]->vertex[j];
						m_vertices[j].m_normal = RM.PmeshList[MeshIndex]->normal[j];
						m_vertices[j].m_UV = RM.PmeshList[MeshIndex]->UV[j];

					}

					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer());
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(reinterpret_cast<Render::Vertex1P1N1UV*>(m_vertices), RM.PmeshList[MeshIndex]->nrOfVertices); 
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_material = materials[RM.PmeshList[MeshIndex]->MaterialID];	////SÄTTER DET SÅ DET ÄR ETT MATERIAL PER MESH

					ReleaseMutex(RM.MeshMutexHandle);

				}

				//Remove component at index
				if(RemoveMeshIndex != -1)	
				{					
					m_world.GetEntityManager()->RemoveAllComponents(Entities[RemoveMeshIndex]);
					m_world.GetEntityManager()->RemoveEntity(Entities[RemoveMeshIndex]);
					Entities.erase(Entities.begin() + RemoveMeshIndex);
				}
				

				///////////////////////UPDATE LIGHTS////////////////////////////////
				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

				int LightIndex = RM.LightIdChange->x;
				*RM.LightIdChange = glm::vec2(-1,-1);
				ReleaseMutex(RM.IdMutexHandle);

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

				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
				int cameraIDchange = RM.CameraIdChange->x;
				RM.CameraIdChange->x = -1;
				ReleaseMutex(RM.IdMutexHandle);


				/////////////////////// UPDATE CAMERAS ////////////////////////////////
				if( cameraIDchange != -1)
				{
					RM.CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
					WaitForSingleObject(RM.CameraMutexHandle, RM.milliseconds);

					glm::quat rotation;
					rotation.x = RM.PcameraList[cameraIDchange]->transformation.rotation.x;
					rotation.y = RM.PcameraList[cameraIDchange]->transformation.rotation.y;
					rotation.z = RM.PcameraList[cameraIDchange]->transformation.rotation.z;
					rotation.w = RM.PcameraList[cameraIDchange]->transformation.rotation.w;

					//CAMERA IS LOOKING BACKWARDS!!
					cameraTransform->m_position.x = RM.PcameraList[cameraIDchange]->transformation.position.x; // ALT negative
					cameraTransform->m_position.y = RM.PcameraList[cameraIDchange]->transformation.position.y;
					cameraTransform->m_position.z = RM.PcameraList[cameraIDchange]->transformation.position.z;// ALT negative
					//cameraTransform->m_orientation.SetOrientation(glm::quat(0,1,0,0));
					cameraTransform->m_orientation.SetOrientation(rotation);
					cameraTransform->m_orientation.Yaw(180);
					
					camera->m_far = RM.PcameraList[cameraIDchange]->farClippingPlane;					
					camera->m_near = RM.PcameraList[cameraIDchange]->nearClippingPlane;
					//camera->m_fov = glm::degrees(RM.PcameraList[cameraIDchange]->horizontalFieldOfView);
					camera->m_fov = glm::degrees(RM.PcameraList[cameraIDchange]->verticalFieldOfView);

					cout << "CameraIdChange: "	<< cameraIDchange << endl;
					cout << "Position: "		<< cameraTransform->m_position.x << " "
												<< cameraTransform->m_position.y << " "
												<< cameraTransform->m_position.z << endl;
					cout << "Rotation: "		<< cameraTransform->m_orientation.GetQuaterion().x << " "
												<< cameraTransform->m_orientation.GetQuaterion().y << " "
												<< cameraTransform->m_orientation.GetQuaterion().z << " "
												<< cameraTransform->m_orientation.GetQuaterion().w << endl;
					cout << "FieldOfView "		<< camera->m_fov << endl;
					cout << "Far "				<< camera->m_far << endl;
					cout << "Near "				<< camera->m_near << endl;

					ReleaseMutex(RM.CameraMutexHandle);

				}

				

				HandleEvents();

				g_engineContext.m_renderer->Clear();

				cameraSystem->Process();
				pointLightSystem->Process();
				renderingSystem->Process();
				

				g_engineContext.m_renderer->Render();

				g_engineContext.m_renderer->Swap();
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
	DynamicLoader::FreeSharedLibrary(g_engineModule);

	return 0;
}

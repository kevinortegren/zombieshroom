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
#include <RootSystems\Include\Components.h>
#include <ComponentExporter.h>

#undef main

bool g_running;
void* g_engineModule;
std::shared_ptr<SDL_Window> g_window;
RootEngine::GameSharedContext g_engineContext;
int entityExport;
string g_savepath = "C:/Users/BTH/Documents/zombieshroom/Assets/Levels/";

ReadMemory RM;

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

		case SDL_KEYDOWN:
			{
				// If pressing "P" set to export scene
				if(event.key.keysym.scancode == SDL_SCANCODE_P)
				{
					entityExport = true;
					RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
					WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

					*RM.export = 1;

					ReleaseMutex(RM.IdMutexHandle);
				}
			}
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

void CreateMaterial(string textureName, string materialName)
{
	if(textureName == "" || textureName == "NONE")
	{
		Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(materialName);
		mat->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("grayLambert" , Render::TextureType::TEXTURE_2D);
		mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
	}
	else
	{
		Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(materialName);
		mat->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture(textureName, Render::TextureType::TEXTURE_2D);
		mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
	}

	//Could use a materialName -> Lambert, Phong etc instead of "Mesh"
}

ECS::Entity* CreateMeshEntity(ECS::World* p_world, std::string p_name)
{
	ECS::Entity* entity = p_world->GetEntityManager()->CreateEntity();

	RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);
	renderable->m_model = g_engineContext.m_resourceManager->CreateModel(p_name);

	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);
	RootForce::Collision* collision = p_world->GetEntityManager()->CreateComponent<RootForce::Collision>(entity);
	collision->m_meshHandle = p_name;

	p_world->GetGroupManager()->RegisterEntity("Static", entity);

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
	RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

	*RM.export = 0;
	entityExport = 0;

	ReleaseMutex(RM.IdMutexHandle);

	// Enable components to use.
	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
    RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
    RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
    RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
	
	// Setup world.
	ECS::World m_world;

	m_world.GetEntityExporter()->SetExporter(Exporter);

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

			g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)); 
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

			//Create camera entity
			cameras.push_back(m_world.GetEntityManager()->CreateEntity());
			RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameras[0]);
			RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameras[0]);
			m_world.GetTagManager()->RegisterEntity("Camera", cameras[0]);
			m_world.GetGroupManager()->RegisterEntity("NonExport", cameras[0]);

			std::shared_ptr<Render::Mesh> Mesh[g_maxMeshes];
			
			int numberMeshes;
			int numberLights;

			Render::Vertex1P1N1UV* m_vertices;
			m_vertices = new Render::Vertex1P1N1UV[g_maxVerticesPerMesh];


			//Load existing objects

			/////////////////////// LOAD SCENE ////////////////////////////////
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

			numberMeshes = *RM.NumberOfMeshes;
			*RM.MeshIdChange = glm::vec2(-1, -1);
			std::string OldtempTexName;
			std::string tempTexName;

			ReleaseMutex(RM.IdMutexHandle);

			RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
			WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

			/////////////////////// LOAD MATERIALS ////////////////////////////////
			int renderNrOfMaterials = *RM.NumberOfMaterials;

			for(int i = 0; i < renderNrOfMaterials; i++)
			{
				CreateMaterial(GetNameFromPath(RM.PmaterialList[i]->texturePath), GetNameFromPath(RM.PmaterialList[i]->materialName));
			}
			// Set to update all materials
			renderNrOfMaterials = -1;

			/////////////////////// LOAD MESHES ////////////////////////////////
			for(int i = 0; i < numberMeshes; i++)
			{
				string name = RM.PmeshList[i]->modelName;
				Entities.push_back(CreateMeshEntity(&m_world, name));	
				cout << "LOADED: " << name  << " TO INDEX " << i << endl;

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_position = RM.PmeshList[i]->transformation.position;
				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_scale = RM.PmeshList[i]->transformation.scale;

				glm::quat rotation;
				rotation.x = RM.PmeshList[i]->transformation.rotation.x;
				rotation.y = RM.PmeshList[i]->transformation.rotation.y;
				rotation.z = RM.PmeshList[i]->transformation.rotation.z;
				rotation.w = RM.PmeshList[i]->transformation.rotation.w;

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[i])->m_orientation.SetOrientation(rotation);

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

				// Get material connected to mesh and set it from materiallist
				RootForce::Renderable* rendy = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i]);
				rendy->m_material = g_engineContext.m_resourceManager->GetMaterial(GetNameFromPath(RM.PmeshList[i]->materialName));

				ReleaseMutex(RM.MeshMutexHandle);

			}

			/////////////////////// Load Lights ////////////////////////////////
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

			numberLights = *RM.NumberOfLights;
			*RM.LightIdChange = glm::vec2(-1,-1);

			ReleaseMutex(RM.IdMutexHandle);
			
			for(int i = 0; i < numberLights; i++)
			{
				LightEntities.push_back(CreateLightEntity(&m_world));
				
				RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
				WaitForSingleObject(RM.LightMutexHandle, RM.milliseconds);

				m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[i])->m_position = RM.PlightList[i]->transformation.position;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_color.r = RM.PlightList[i]->color.r;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_color.g = RM.PlightList[i]->color.g;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_color.b = RM.PlightList[i]->color.b;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_color.a = RM.PlightList[i]->color.a;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_attenuation.x = 0.0f;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_attenuation.y = 1-0.1 * RM.PlightList[i]->Intensity;
				m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[i])->m_attenuation.z = 0.0f;
				
				ReleaseMutex(RM.LightMutexHandle);
			}

			//////////////////////// LOAD CAMERA  /////////////////////////////////////////
			RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
			WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
			*RM.CameraIdChange = glm::vec2(-1,-1);
			ReleaseMutex(RM.IdMutexHandle);

			RM.CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
			WaitForSingleObject(RM.CameraMutexHandle, RM.milliseconds);

			glm::quat rotation;
			rotation.x = RM.PcameraList[0]->transformation.rotation.x;
			rotation.y = RM.PcameraList[0]->transformation.rotation.y;
			rotation.z = RM.PcameraList[0]->transformation.rotation.z;
			rotation.w = RM.PcameraList[0]->transformation.rotation.w;

			cameraTransform->m_position.x = RM.PcameraList[0]->transformation.position.x;
			cameraTransform->m_position.y = RM.PcameraList[0]->transformation.position.y;
			cameraTransform->m_position.z = RM.PcameraList[0]->transformation.position.z;
			cameraTransform->m_orientation.SetOrientation(rotation);
			//Rotate 180 to fix camera
			cameraTransform->m_orientation.Yaw(180);

			camera->m_far = RM.PcameraList[0]->farClippingPlane;					
			camera->m_near = RM.PcameraList[0]->nearClippingPlane;
			camera->m_fov = glm::degrees(RM.PcameraList[0]->verticalFieldOfView);

			ReleaseMutex(RM.CameraMutexHandle);


			///////////////////////////////////////////////////////////////     MAIN LOOP STARTS HERE //////////////////////////////////////////////////////////////
			uint64_t old = SDL_GetPerformanceCounter();
			while (g_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				// GET MESH CHANGE AND REMOVE INDEX
				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
				int MeshIndex = RM.MeshIdChange->x;
				int RemoveMeshIndex = RM.MeshIdChange->y;
				int RemoveLightIndex = RM.LightIdChange->y;
				entityExport = *RM.export;
				
				*RM.MeshIdChange = glm::vec2(-1, -1);
				ReleaseMutex(RM.IdMutexHandle);

				/////////////  EXPORT   ///////////////////////
				if(entityExport == 2)
				{			
					//m_world.GetEntityManager()->RemoveAllComponentsOfType<RootForce::Renderable>();

					for(int i = 0; i < Entities.size()-1; i++)
					{
						//UPDATE modelName for all Entities from shared memory

						RootForce::Renderable *mesh = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(Entities[i]);

						//string shortPath = GetNameFromPath(RM.PmaterialList[RM.PmeshList[i]->MaterialID]->texturePath);
						string materialName = GetNameFromPath(RM.PmeshList[i]->materialName);
						//CreateMaterial(shortPath, materialName);
						mesh->m_material = g_engineContext.m_resourceManager->GetMaterial(materialName);

						//if(shortPath == "NONE")
						//{
						//	mesh->m_material = *defaultMaterial;
						//}
						//else
						//{
						//	mesh->m_material.m_diffuseMap = g_engineContext.m_resourceManager->GetTexture(shortPath);
						//	mesh->m_material.m_effect = g_engineContext.m_resourceManager->GetEffect("Mesh"); 
						//}

						RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
						WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
						string name = RM.PmeshList[i]->modelName;
						ReleaseMutex(RM.MeshMutexHandle);

						RootForce::Collision* collision = m_world.GetEntityManager()->GetComponent<RootForce::Collision>(Entities[i]);
						collision->m_meshHandle = name + "0";

						mesh->m_model = g_engineContext.m_resourceManager->CreateModel(name);

					}

					m_world.GetEntityExporter()->Export(g_savepath + "level.world");
					entityExport = false;
					RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
					WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
					*RM.export = 0;
					ReleaseMutex(RM.IdMutexHandle);
				}

				/////////////////////// UPDATE MESHES ////////////////////////////////
				if(MeshIndex != -1)					
				{
					bool newMaterial = false;
					int size = Entities.size()-1;
					if(MeshIndex > size)
					{
						string name = RM.PmeshList[MeshIndex]->transformation.name;
						if(name != "")
						{
							Entities.push_back(CreateMeshEntity(&m_world, name));
							cout << "Adding " << name << " to index: " << MeshIndex << endl;
						}
					}
					else
					{
						cout << "Updating " << RM.PmeshList[MeshIndex]->transformation.name << " at index: " << MeshIndex << endl;
					}
				
					RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");		
					WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

					// TRANSFORM AND SCALE
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_position = RM.PmeshList[MeshIndex]->transformation.position;
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_scale = RM.PmeshList[MeshIndex]->transformation.scale;

					////Update material list
					CreateMaterial(GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->texturePath), GetNameFromPath(RM.PmeshList[MeshIndex]->materialName));

					/// ROTATION
					glm::quat rotation;
					rotation.x = RM.PmeshList[MeshIndex]->transformation.rotation.x;
					rotation.y = RM.PmeshList[MeshIndex]->transformation.rotation.y;
					rotation.z = RM.PmeshList[MeshIndex]->transformation.rotation.z;
					rotation.w = RM.PmeshList[MeshIndex]->transformation.rotation.w;				
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex])->m_orientation.SetOrientation(rotation);

					// COPY VERTICES
					for(int j = 0; j < RM.PmeshList[MeshIndex]->nrOfVertices; j++)
					{
						m_vertices[j].m_pos = RM.PmeshList[MeshIndex]->vertex[j];
						m_vertices[j].m_normal = RM.PmeshList[MeshIndex]->normal[j];
						m_vertices[j].m_UV = RM.PmeshList[MeshIndex]->UV[j];

					}

					// SET INFORMATION TO GAME
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer());
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
					m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV(reinterpret_cast<Render::Vertex1P1N1UV*>(m_vertices), RM.PmeshList[MeshIndex]->nrOfVertices); 

					RootForce::Renderable* rendy = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex]);
					rendy->m_material = g_engineContext.m_resourceManager->GetMaterial(GetNameFromPath(RM.PmeshList[MeshIndex]->materialName));

					ReleaseMutex(RM.MeshMutexHandle);
				}

				/////////////////////// UPDATE LIGHTS ////////////////////////////////

				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);

				int LightIndex = RM.LightIdChange->x;
				*RM.LightIdChange = glm::vec2(-1,-1);
				ReleaseMutex(RM.IdMutexHandle);

				if(LightIndex != -1)					
				{
					int size = LightEntities.size()-1;
					if(LightIndex > size)
					{
						LightEntities.push_back(CreateLightEntity(&m_world));
					}
				
					RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
					WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_position = RM.PlightList[LightIndex]->transformation.position;
					m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_scale = RM.PlightList[LightIndex]->transformation.scale;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.r = RM.PlightList[LightIndex]->color.r;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.g = RM.PlightList[LightIndex]->color.g;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.b = RM.PlightList[LightIndex]->color.b;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.a = RM.PlightList[LightIndex]->color.a;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.x = 0.0f;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.y = 1-0.1 * RM.PlightList[LightIndex]->Intensity;
					m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.z = 0.0f;

					ReleaseMutex(RM.LightMutexHandle);
				}

				/////////////////////// UPDATE CAMERAS ////////////////////////////////
				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
				int cameraIDchange = RM.CameraIdChange->x;
				RM.CameraIdChange->x = -1;
				ReleaseMutex(RM.IdMutexHandle);
				
				if( cameraIDchange != -1)
				{
					RM.CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
					WaitForSingleObject(RM.CameraMutexHandle, RM.milliseconds);

					glm::quat rotation;
					rotation.x = RM.PcameraList[cameraIDchange]->transformation.rotation.x;
					rotation.y = RM.PcameraList[cameraIDchange]->transformation.rotation.y;
					rotation.z = RM.PcameraList[cameraIDchange]->transformation.rotation.z;
					rotation.w = RM.PcameraList[cameraIDchange]->transformation.rotation.w;

					cameraTransform->m_position.x = RM.PcameraList[cameraIDchange]->transformation.position.x;
					cameraTransform->m_position.y = RM.PcameraList[cameraIDchange]->transformation.position.y;
					cameraTransform->m_position.z = RM.PcameraList[cameraIDchange]->transformation.position.z;
					cameraTransform->m_orientation.SetOrientation(rotation);
					//Rotate 180 to fix camera
					cameraTransform->m_orientation.Yaw(180);
					
					camera->m_far = RM.PcameraList[cameraIDchange]->farClippingPlane;					
					camera->m_near = RM.PcameraList[cameraIDchange]->nearClippingPlane;
					camera->m_fov = glm::degrees(RM.PcameraList[cameraIDchange]->verticalFieldOfView);

					ReleaseMutex(RM.CameraMutexHandle);

				}			

				//Remove component at index
				if(RemoveMeshIndex != -1 && RemoveMeshIndex < Entities.size())	
				{					
					cout << "Removing " << RemoveMeshIndex << " EntitySize " << Entities.size() << endl;
					m_world.GetEntityManager()->RemoveAllComponents(Entities[RemoveMeshIndex]);
					m_world.GetEntityManager()->RemoveEntity(Entities[RemoveMeshIndex]);
					Entities.erase(Entities.begin() + RemoveMeshIndex);
				}

				if(RemoveLightIndex != -1)	
				{					
					m_world.GetEntityManager()->RemoveAllComponents(LightEntities[RemoveLightIndex]);
					m_world.GetEntityManager()->RemoveEntity(LightEntities[RemoveLightIndex]);
					LightEntities.erase(LightEntities.begin() + RemoveLightIndex);
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

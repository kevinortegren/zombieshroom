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

ReadMemory RM;
int numberMeshes;
int numberLights;
// Setup world.
ECS::World m_world;
RootForce::RenderingSystem* renderingSystem;
RootForce::CameraSystem* cameraSystem;
RootForce::PointLightSystem* pointLightSystem;
RootForce::ParticleSystem* particleSystem;

std::vector<ECS::Entity*> cameras;
std::vector<ECS::Entity*> LightEntities;
std::vector<ECS::Entity*> Entities;
std::vector<ECS::Entity*> particleEntities;
std::vector<ECS::Entity*> spawnPointEntities;

void HandleEvents();
std::string GetNameFromPath( std::string p_path );
void Initialize(RootEngine::GameSharedContext g_engineContext);

void CreateMaterial(string textureName, string materialName, string normalMap, string specularMap);
ECS::Entity* CreateLightEntity(ECS::World* p_world);
ECS::Entity* CreateMeshEntity(ECS::World* p_world, std::string p_name, int index);
ECS::Entity* CreateTransformEntity(ECS::World* p_world, int index);
ECS::Entity* CreateParticleEntity(ECS::World* p_world, std::string p_name, int index);
void CreateCameraEntity();

void UpdateCamera();
void LoadLocators();
void UpdateMesh(int index, bool updateTransformation, bool updateShape, bool remove);
void UpdateLight(int index, bool remove);

void ExportToLevel();

string PL = "PointLight";

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

			Initialize(g_engineContext);

			//LOAD
			CreateCameraEntity();			
			LoadLocators();


			RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
			WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
			int renderNrOfMaterials = *RM.NumberOfMaterials;

			for(int i = 0; i < renderNrOfMaterials; i++)
			{
				CreateMaterial(GetNameFromPath(RM.PmaterialList[i]->texturePath), GetNameFromPath(RM.PmaterialList[i]->materialName), GetNameFromPath(RM.PmaterialList[i]->normalPath),GetNameFromPath(RM.PmaterialList[i]->specularPath));
			}

			ReleaseMutex(RM.MeshMutexHandle);

			// Set to update all materials
			//renderNrOfMaterials = -1;

			/////////////////////// LOAD MESHES ////////////////////////////////

			RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
			WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
			numberMeshes = *RM.NumberOfMeshes;
			ReleaseMutex(RM.MeshMutexHandle);

			for(int i = 0; i < numberMeshes; i++)
			{				
				string name = RM.PmeshList[i]->modelName;
				Entities.push_back(CreateMeshEntity(&m_world, name, i));
				auto model = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model;
				auto mesh = model->m_meshes[0];
				auto buffer = g_engineContext.m_renderer->CreateBuffer();
				mesh->SetVertexBuffer(buffer);

				UpdateMesh(i, true, true, false);
			}			

			///////////////////////// Load Lights ////////////////////////////////

			RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
			WaitForSingleObject(RM.LightMutexHandle, RM.milliseconds);
			numberLights = *RM.NumberOfLights;
			ReleaseMutex(RM.LightMutexHandle);

			for(int i = 0; i < numberLights; i++)
			{
				UpdateLight(i, false);
			}

			///////////////////////////////////////////////////////////////     MAIN LOOP STARTS HERE  //////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			uint64_t old = SDL_GetPerformanceCounter();
			while (g_running)
			{
				uint64_t now = SDL_GetPerformanceCounter();
				float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
				old = now;

				m_world.SetDelta(dt);

				string type;
				int updateID, removeID;
				bool updateTransform, updateShape;

				//GET A MESSAGE
				RM.ReadMessage(type, updateID, removeID, updateTransform, updateShape);
				if(type != "")
				{
					cout << type << " ID " << updateID << endl;
					cout << "NumberOfMessages " << *RM.NumberOfMessages << endl;
				}

				int locatorIdChange = -1;
				if(type == "Locator")
					locatorIdChange = updateID;

				// GET EXPORT STATE
				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
				entityExport = *RM.export;				
				ReleaseMutex(RM.IdMutexHandle);

				/////////////  EXPORT   ///////////////////////
				if(entityExport == 2)
				{			
					ExportToLevel();
				}
				/////////////////////// UPDATE LOCATORS //////////////////////////////
				if(locatorIdChange != -1)
				{
					cout << RM.PlocatorList[locatorIdChange]->transformation.name << " updated!" << endl;
				}

				//UPDATE

				if(type == "Mesh")
				{
					if(removeID == -1)
						UpdateMesh(updateID, updateTransform, updateShape, false);
					else
						UpdateMesh(removeID, updateTransform, updateShape, true);
				}

				if(type == "Light")
				{
					if(removeID == -1)
						UpdateLight(updateID, false);
					else
						UpdateLight(removeID, true);
				}

				if(type == "Camera")
					UpdateCamera();

				HandleEvents();
				g_engineContext.m_renderer->Clear();
				cameraSystem->Process();
				pointLightSystem->Process();
				particleSystem->Process();
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

/////////////////////////////////////////////////////////// FUNCTION DEFINITIONS ////////////////////////////////////////////

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

void Initialize(RootEngine::GameSharedContext g_engineContext)
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
	RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);
	m_world.GetEntityExporter()->SetExporter(Exporter);
	RM.InitalizeSharedMemory();

	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

	Render::DirectionalLight dl;
	dl.m_direction = glm::vec3(0,-1,0);
	dl.m_color = glm::vec4(0,0,1,1);

	//g_engineContext.m_renderer->AddDirectionalLight(dl, 0);

	// Initialize systems.
	renderingSystem = new RootForce::RenderingSystem(&m_world);
	renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem, "RenderingSystem");

	pointLightSystem = new RootForce::PointLightSystem(&m_world, g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem, "PointLightSystem");

	cameraSystem = new RootForce::CameraSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem, "CameraSystem");

	particleSystem = new RootForce::ParticleSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ParticleSystem>(particleSystem, "ParticleSystem");
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

void CreateMaterial(string textureName, string materialName, string normalMap, string specularMap)
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
		if(normalMap != "" && normalMap != "NONE")
		{
		mat->m_normalMap = g_engineContext.m_resourceManager->LoadTexture(normalMap, Render::TextureType::TEXTURE_2D);
		}
		if(specularMap != "" && specularMap != "NONE")
		{
		mat->m_specularMap = g_engineContext.m_resourceManager->LoadTexture(specularMap, Render::TextureType::TEXTURE_2D);
		}
		mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh"); //Måste ha tangenter (använd Mesh_NormalMap)
	}

	//Could use a materialName -> Lambert, Phong etc instead of "Mesh"
}

ECS::Entity* CreateMeshEntity(ECS::World* p_world, std::string p_name, int index)
{
	ECS::Entity* entity = p_world->GetEntityManager()->CreateEntity();

	RootForce::Renderable* renderable = p_world->GetEntityManager()->CreateComponent<RootForce::Renderable>(entity);
	renderable->m_model = g_engineContext.m_resourceManager->CreateModel(p_name);
	renderable->m_model->m_transform = glm::mat4x4(1);

	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);
	RootForce::Collision* collision = p_world->GetEntityManager()->CreateComponent<RootForce::Collision>(entity);
	collision->m_meshHandle = p_name;

	for(int i = 0; i < RM.PmeshList[index]->transformation.nrOfFlags; i++)
	{
		p_world->GetGroupManager()->RegisterEntity(RM.PmeshList[index]->transformation.flags[i], entity);
	}
	//p_world->GetGroupManager()->RegisterEntity("Static", entity);
	//p_world->GetGroupManager()->UnregisterEntity("Static", entity);

	return entity;
}

ECS::Entity* CreateTransformEntity(ECS::World* p_world, int index)
{
	ECS::Entity* entity = p_world->GetEntityManager()->CreateEntity();

	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);
	transform->m_position.x = RM.PlocatorList[index]->transformation.position.x;
	transform->m_position.y = RM.PlocatorList[index]->transformation.position.y;
	transform->m_position.z = RM.PlocatorList[index]->transformation.position.z;

	glm::quat rotation;
	rotation.x = RM.PlocatorList[index]->transformation.rotation.x;
	rotation.y = RM.PlocatorList[index]->transformation.rotation.y;
	rotation.z = RM.PlocatorList[index]->transformation.rotation.z;
	rotation.w = RM.PlocatorList[index]->transformation.rotation.w;

	transform->m_orientation.SetOrientation(rotation);

	for(int i = 0; i < RM.PlocatorList[index]->transformation.nrOfFlags; i++)
	{
		p_world->GetGroupManager()->RegisterEntity(RM.PlocatorList[index]->transformation.flags[i], entity);
	}

	return entity;
}

ECS::Entity* CreateParticleEntity(ECS::World* p_world, std::string p_name, int index)
{
	ECS::Entity* entity = p_world->GetEntityManager()->CreateEntity();
	RootForce::Transform* transform = p_world->GetEntityManager()->CreateComponent<RootForce::Transform>(entity);
	RootForce::ParticleEmitter* particle = p_world->GetEntityManager()->CreateComponent<RootForce::ParticleEmitter>(entity);

	Render::ParticleSystemDescription desc;
	desc.m_initalPos = RM.PlocatorList[index]->transformation.position;
	desc.m_initalVel = glm::vec3(0,0,0);
	desc.m_size = glm::vec2(0.5f, 0.5f);

	particle->m_system = g_engineContext.m_renderer->CreateParticleSystem(desc);
	particle->m_material = g_engineContext.m_resourceManager->GetMaterial("test");
	particle->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Particle/Particle");
	particle->m_material->m_diffuseMap = g_engineContext.m_resourceManager->LoadTexture("smoke", Render::TextureType::TEXTURE_2D);

	for(int i = 0; i < RM.PlocatorList[index]->transformation.nrOfFlags; i++)
	{
		p_world->GetGroupManager()->RegisterEntity(RM.PlocatorList[index]->transformation.flags[i], entity);
	}

	return entity;
}

void CreateCameraEntity()
{
	//Create camera entity
	cameras.push_back(m_world.GetEntityManager()->CreateEntity());
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameras[0]);
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameras[0]);
	m_world.GetTagManager()->RegisterEntity("Camera", cameras[0]);
	m_world.GetGroupManager()->RegisterEntity("NonExport", cameras[0]);

	UpdateCamera();
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



void UpdateCamera()
{
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameras[0]);
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameras[0]);

	//if(cameraIDchange != -1)
	//{
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
	//}
}

void LoadLocators()
{
	RM.LocatorMutexHandle = CreateMutex(nullptr, false, L"LocatorMutex");
	WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
	int nrLoc = *RM.NumberOfLocators;

	for(int i = 0; i < nrLoc; i++)
	{
		cout << "Found locator " << i << " " << RM.PlocatorList[i]->transformation.name << endl;
		cout << "POS X Y Z " << RM.PlocatorList[i]->transformation.position.x << RM.PlocatorList[i]->transformation.position.y << RM.PlocatorList[i]->transformation.position.z << endl;

		for(int j = 0; j < RM.PlocatorList[i]->transformation.nrOfFlags; j++)
		{
			string flagName = "Particle";
			if(flagName.compare(RM.PlocatorList[i]->transformation.flags[j]) == 0)
			{
				particleEntities.push_back(CreateParticleEntity(&m_world, RM.PlocatorList[i]->transformation.name, i));
			}
		}

		for(int j = 0; j < RM.PlocatorList[i]->transformation.nrOfFlags; j++)
		{
			string flagName = "SpawnPoint";
			string flagName2 = RM.PlocatorList[i]->transformation.flags[j];

			if(flagName == flagName2)
			{
				spawnPointEntities.push_back(CreateTransformEntity(&m_world, i));
			}
		}

	}

	ReleaseMutex(RM.LocatorMutexHandle);
}

void UpdateMesh(int index, bool updateTransformation, bool updateShape, bool remove)
{
	int MeshIndex = -1;
	int RemoveMeshIndex = -1;

	if(remove)
	{
		MeshIndex = -1;
		RemoveMeshIndex = index;
	}
	else
	{
		MeshIndex = index;
		RemoveMeshIndex = -1;
	}


	RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
	numberMeshes = *RM.NumberOfMeshes;
	ReleaseMutex(RM.MeshMutexHandle);
	

	if(MeshIndex != -1)					
	{
		RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");		
		WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

		bool newMaterial = false;
		int size = Entities.size()-1;
		if(MeshIndex > size)
		{
			string name = RM.PmeshList[MeshIndex]->transformation.name;
			if(name != "")
			{
				Entities.push_back(CreateMeshEntity(&m_world, name, MeshIndex));
				cout << "Adding " << name << " to index: " << MeshIndex << endl;
			}
		}
		else
		{
			cout << "Updating " << RM.PmeshList[MeshIndex]->transformation.name << " at index: " << MeshIndex << endl;
		}

		if(updateTransformation)
		{
			// TRANSFORM AND SCALE
			RootForce::Transform* transform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex]);

			transform->m_position = RM.PmeshList[MeshIndex]->transformation.position;
			transform->m_scale = RM.PmeshList[MeshIndex]->transformation.scale;

			////Update material list
			CreateMaterial(GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->texturePath), GetNameFromPath(RM.PmeshList[MeshIndex]->materialName),GetNameFromPath(RM.PmaterialList[MeshIndex]->normalPath), GetNameFromPath(RM.PmaterialList[MeshIndex]->specularPath));

			/// ROTATION
			glm::quat rotation;
			rotation.x = RM.PmeshList[MeshIndex]->transformation.rotation.x;
			rotation.y = RM.PmeshList[MeshIndex]->transformation.rotation.y;
			rotation.z = RM.PmeshList[MeshIndex]->transformation.rotation.z;
			rotation.w = RM.PmeshList[MeshIndex]->transformation.rotation.w;	

			transform->m_orientation.SetOrientation(rotation);

			//PIVOT
			glm::mat4x4 pivotRotMat = glm::translate(glm::mat4(1), glm::vec3(-RM.PmeshList[MeshIndex]->transformation.rotPivot.x, -RM.PmeshList[MeshIndex]->transformation.rotPivot.y, -RM.PmeshList[MeshIndex]->transformation.rotPivot.z));
			glm::mat4x4 scaleRotMat = glm::translate(glm::mat4(1), glm::vec3(-RM.PmeshList[MeshIndex]->transformation.scalePivot.x, -RM.PmeshList[MeshIndex]->transformation.scalePivot.y, -RM.PmeshList[MeshIndex]->transformation.scalePivot.z));

			glm::mat4x4 scaleMatrix = glm::scale(transform->m_scale);
			glm::mat4x4 modifiedScale = glm::inverse(scaleRotMat) * scaleMatrix * scaleRotMat;

			glm::mat4x4 rotationMatrix = glm::rotate(glm::mat4(1.0f), transform->m_orientation.GetAngle(), transform->m_orientation.GetAxis());
			glm::mat4x4 modifiedRotation = glm::inverse(pivotRotMat) * rotationMatrix * pivotRotMat;

			glm::mat4x4 modifiedSR = modifiedRotation * modifiedScale;

			glm::vec3 modifiedPos = glm::vec3(modifiedSR[3][0], modifiedSR[3][1], modifiedSR[3][2]);
			transform->m_position += modifiedPos;

			// Get material connected to mesh and set it from materiallist
			RootForce::Renderable* rendy = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex]);
			rendy->m_material = g_engineContext.m_resourceManager->GetMaterial(GetNameFromPath(RM.PmeshList[MeshIndex]->materialName));	
		}
		

		if(updateShape)
		{
		// COPY VERTICES
		Render::Vertex1P1N1UV* m_vertices;
		m_vertices = new Render::Vertex1P1N1UV[g_maxVerticesPerMesh];

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

		}

		ReleaseMutex(RM.MeshMutexHandle);
	}

	//Remove mesh at index
	if(RemoveMeshIndex != -1 && RemoveMeshIndex < Entities.size())	
	{					
		cout << "Removing " << RemoveMeshIndex << " EntitySize " << Entities.size() << endl;
		m_world.GetEntityManager()->RemoveAllComponents(Entities[RemoveMeshIndex]);
		m_world.GetEntityManager()->RemoveEntity(Entities[RemoveMeshIndex]);
		Entities.erase(Entities.begin() + RemoveMeshIndex);
	}
}

void UpdateLight(int index, bool remove)
{
	int LightIndex = -1;
	int RemoveLightIndex = -1;

	if(remove)
	{
		LightIndex = -1;
		RemoveLightIndex = index;
	}
	else
	{
		LightIndex = index;
		RemoveLightIndex = -1;
	}

	RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(RM.LightMutexHandle, RM.milliseconds);
	numberLights = *RM.NumberOfLights;
	ReleaseMutex(RM.LightMutexHandle);
	

	int size = LightEntities.size()-1;

	if(LightIndex != -1)					
	{		
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

	if(RemoveLightIndex != -1 && RemoveLightIndex < size)	
	{					
		m_world.GetEntityManager()->RemoveAllComponents(LightEntities[RemoveLightIndex]);
		m_world.GetEntityManager()->RemoveEntity(LightEntities[RemoveLightIndex]);
		LightEntities.erase(LightEntities.begin() + RemoveLightIndex);
	}
}

void ExportToLevel()
{
	for(int i = 0; i < Entities.size()-1; i++)
	{
		//UPDATE modelName for all Entities from shared memory

		RootForce::Renderable *mesh = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(Entities[i]);
		string materialName = GetNameFromPath(RM.PmeshList[i]->materialName);
		mesh->m_material = g_engineContext.m_resourceManager->GetMaterial(materialName);

		RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
		WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
		string name = RM.PmeshList[i]->modelName;
		ReleaseMutex(RM.MeshMutexHandle);

		RootForce::Collision* collision = m_world.GetEntityManager()->GetComponent<RootForce::Collision>(Entities[i]);
		collision->m_meshHandle = name + "0";

		mesh->m_model = g_engineContext.m_resourceManager->CreateModel(name);
	}

	m_world.GetEntityExporter()->Export(g_savepath + "Levels/" + g_levelName + ".world");

	entityExport = false;
	RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
	*RM.export = 0;
	ReleaseMutex(RM.IdMutexHandle);
}
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
#include <RootSystems\Include\WorldSystem.h>
#include <RootSystems\Include\ShadowSystem.h>

#include <Utility\ECS\Include\World.h>

#include <RootEngine/Include/ResourceManager/ResourceManager.h>

#include <RootTools\LevelEditor\3DViewer\Include\RawMeshPrimitives.h>
#include <RootSystems\Include\ComponentTypes.h>
#include <ComponentExporter.h>

#include <CPUTimer.h>

#undef main

bool g_running;
void* g_engineModule;
std::shared_ptr<SDL_Window> g_window;
RootEngine::GameSharedContext g_engineContext;
int entityExport;

ReadMemory RM;
int numberMeshes;
int numberLights;
int numberCameras;

// Setup world.
ECS::World m_world;
RootForce::RenderingSystem* renderingSystem;
RootForce::CameraSystem* cameraSystem;
RootForce::PointLightSystem* pointLightSystem;
RootForce::DirectionalLightSystem* directionalLightSystem;
RootForce::ParticleSystem* particleSystem;
RootForce::WorldSystem* worldSystem;
RootForce::ShadowSystem* shadowSystem;

std::vector<ECS::Entity*> cameras;
std::vector<ECS::Entity*> LightEntities;
std::vector<ECS::Entity*> Entities;
std::vector<ECS::Entity*> locatorEntities;

Utility::CPUTimer timer;

void HandleEvents();
std::string GetNameFromPath( std::string p_path );
void Initialize(RootEngine::GameSharedContext g_engineContext);

void CreateMaterial(string textureName, string materialName, string normalMap, string specularMap, string glowMap, int meshID);
ECS::Entity* CreateLightEntity(ECS::World* p_world);
ECS::Entity* CreateMeshEntity(ECS::World* p_world, std::string p_name, int index);
ECS::Entity* CreateTransformEntity(ECS::World* p_world, int index);
ECS::Entity* CreateParticleEntity(ECS::World* p_world, std::string p_name, int index);
void CreateCameraEntity(int index);

void UpdateCamera(int index);
void LoadLocators();
void UpdateMesh(int index, bool updateTransformation, bool updateShape, bool remove);
void UpdateLight(int index, bool remove, bool firstTimeLoad);


void ExportToLevel();
void LoadSceneFromMaya();
void UpdateParticle(int index);

Render::TextureInterface* painter;

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

			painter = g_engineContext.m_resourceManager->CreateTexture("painter");
			painter->CreateEmptyTexture(256, 256, Render::TextureFormat::TEXTURE_RGBA);

			Initialize(g_engineContext);

			LoadSceneFromMaya();

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
				int updateID = -1;
				int	removeID = -1;
				bool updateTransform, updateShape;
				std::deque<UpdateMessage> localMessages;
				int numberMessages = 0;

				if(type != "")
				{
					cout << type << " ID " << updateID << endl;
					//cout << "NumberOfMessages " << *RM.NumberOfMessages << endl;
				}

				// GET EXPORT STATE
				RM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
				WaitForSingleObject(RM.IdMutexHandle, RM.milliseconds);
				entityExport = *RM.export;				
				numberMessages = *RM.NumberOfMessages;
				ReleaseMutex(RM.IdMutexHandle);

				for(int i = 0; i < numberMessages; i ++)
				{
					localMessages.push_back(RM.PeekMessageAt(i));
				}

				RM.ClearAllMessages();
				
				/////////////  EXPORT   ///////////////////////
				if(entityExport == 2)
				{			
					ExportToLevel();
				}
				/////////////////////// UPDATE LOCATORS //////////////////////////////

				if(type == "Locator")
				{
					//cout << RM.PlocatorList[updateID]->transformation.name << " updated!" << endl;

					for(int i = 0; i < RM.PlocatorList[updateID]->transformation.nrOfFlags; i++)
					{
						string temp = RM.PlocatorList[updateID]->transformation.flags[i];
						if(temp == "Particle")
							UpdateParticle(updateID);
					}
				}


				//UPDATE
				while(localMessages.size() > 0)
				{
					updateID = localMessages[0].updateID;
					removeID = localMessages[0].removeID;
					type = localMessages[0].name;
					updateShape = localMessages[0].updateShape;
					updateTransform = localMessages[0].updateTransform;

					localMessages.pop_front();
					if(type == "Mesh")
					{
						if(removeID == -1)
							UpdateMesh(updateID, updateTransform, updateShape, false);
						else
						{
							UpdateMesh(removeID, updateTransform, updateShape, true);
							cout << "RemoveID " << removeID << endl;
						}
					}

					if(type == "Light")
					{
						if(removeID == -1)
							UpdateLight(updateID, false, false);
						else
							UpdateLight(removeID, true, false);
					}

					if(type == "Camera")
					{
						UpdateCamera(updateID);
					}

					if(type == "Texture")
					{
						if(updateID != -1)
						{
							RM.TextureMutexHandle = CreateMutex(nullptr, false, L"TextureMutex");
							WaitForSingleObject(RM.TextureMutexHandle, RM.milliseconds);
							cout << RM.PpaintList[updateID]->heigth << endl;
							cout << RM.PpaintList[updateID]->width << endl;

							//painter->BufferData(RM.PpaintList[updateID]->Pixels);
							ReleaseMutex(RM.TextureMutexHandle);
						}
					}
				}
				

				

				HandleEvents();
				g_engineContext.m_renderer->Clear();

				cameraSystem->Process();
				directionalLightSystem->Process();
				pointLightSystem->Process();			
				particleSystem->Process();				
				shadowSystem->Process();			
				renderingSystem->Process();	

				g_engineContext.m_renderer->Render();

				timer.Start();
				g_engineContext.m_renderer->Swap();
				timer.Stop();

				std::cout << "Frame time ms " << timer.GetTime() << std::endl;

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
	RootForce::DirectionalLight::SetTypeId(RootForce::ComponentType::DIRECTIONALLIGHT);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::Collision::SetTypeId(RootForce::ComponentType::COLLISION);
	RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);
	RootForce::Shadowcaster::SetTypeId(RootForce::ComponentType::SHADOWCASTER);
	m_world.GetEntityExporter()->SetExporter(Exporter);
	RM.InitalizeSharedMemory();

	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(10000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(10000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(10000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::DirectionalLight>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(100);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Collision>(10000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(10000);
	m_world.GetEntityManager()->GetAllocator()->CreateList<RootForce::Shadowcaster>(100);



	// Initialize systems.
	renderingSystem = new RootForce::RenderingSystem(&m_world);
	renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	renderingSystem->SetRendererInterface(g_engineContext.m_renderer);
	m_world.GetSystemManager()->AddSystem<RootForce::RenderingSystem>(renderingSystem);

	pointLightSystem = new RootForce::PointLightSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::PointLightSystem>(pointLightSystem);

	directionalLightSystem = new RootForce::DirectionalLightSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::DirectionalLightSystem>(directionalLightSystem);

	cameraSystem = new RootForce::CameraSystem(&m_world, &g_engineContext);
	m_world.GetSystemManager()->AddSystem<RootForce::CameraSystem>(cameraSystem);

	particleSystem = new RootForce::ParticleSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ParticleSystem>(particleSystem);

	shadowSystem = new RootForce::ShadowSystem(&m_world);
	m_world.GetSystemManager()->AddSystem<RootForce::ShadowSystem>(shadowSystem);

	worldSystem = new RootForce::WorldSystem(&m_world, &g_engineContext);
	worldSystem->CreateSun();
}

void LoadSceneFromMaya()
{
	RM.ClearAllMessages();
	//LOAD						
	LoadLocators();

	//LOAD CAMERAS
	RM.CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
	WaitForSingleObject(RM.CameraMutexHandle, RM.milliseconds);
	numberCameras = *RM.NumberOfCameras;
	ReleaseMutex(RM.CameraMutexHandle);
	CreateCameraEntity(0);

	//LOAD MATERIALS
	RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
	int renderNrOfMaterials = *RM.NumberOfMaterials;

	for(int i = 0; i < renderNrOfMaterials; i++)
	{
		CreateMaterial(GetNameFromPath(RM.PmaterialList[i]->texturePath), GetNameFromPath(RM.PmaterialList[i]->materialName), GetNameFromPath(RM.PmaterialList[i]->normalPath),GetNameFromPath(RM.PmaterialList[i]->specularPath),GetNameFromPath(RM.PmaterialList[i]->glowPath), -1);
	}

	ReleaseMutex(RM.MeshMutexHandle);

	/////////////////////// LOAD MESHES ////////////////////////////////

	RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
	numberMeshes = *RM.NumberOfMeshes;
	

	for(int i = 0; i < numberMeshes; i++)
	{				
		string name = RM.PmeshList[i]->modelName;
		Entities.push_back(CreateMeshEntity(&m_world, name, i));
		auto model = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[i])->m_model;
		auto mesh = model->m_meshes[0];
		auto buffer = g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER);
		mesh->SetVertexBuffer(buffer);

		UpdateMesh(i, true, true, false);
	}			

	ReleaseMutex(RM.MeshMutexHandle);

	///////////////////////// Load Lights ////////////////////////////////

	RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
	WaitForSingleObject(RM.LightMutexHandle, RM.milliseconds);
	numberLights = *RM.NumberOfLights;
	ReleaseMutex(RM.LightMutexHandle);

	for(int i = 0; i < numberLights; i++)
	{
		UpdateLight(i, false, true);
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

bool fexists(const std::string& filename)
{
	bool Exists = false;

	ifstream ifile(filename.c_str());
	if (ifile)
	{
		Exists = true;
	}

  return Exists;
}

void CreateMaterial(string textureName, string materialName, string normalMap, string specularMap, string glowMap, int meshID)
{
	bool painted = false;
	bool painting = false;
	bool transparent = false;
	int paintID;
	string temp;
	string NormalName;
	ifstream ifile;
	RM.MeshMutexHandle = CreateMutex(nullptr, false, L"MeshMutex");
	WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);
	paintID = RM.PmeshList[meshID]->paintIndex;

	for(int i = 0; i < RM.PmeshList[meshID]->transformation.nrOfFlags; i++)
	{
		string flag = RM.PmeshList[meshID]->transformation.flags[i];
		if(flag == "Painted")
		{
			painted = true;
		}
		if(flag == "Painting")
		{
			painting = true;
		}
		if(flag == "Transparent")
		{
			transparent = true;
		}
	}

	ReleaseMutex(RM.MeshMutexHandle);

	if(textureName == "" || textureName == "NONE")
	{
		Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(materialName);
		mat->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("grayLambert" , Render::TextureType::TEXTURE_2D);
		mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
	}
	else if(textureName == "PaintTexture" || painted || painting)
	{
		RM.TextureMutexHandle = CreateMutex(nullptr, false, L"TextureMutex");
		WaitForSingleObject(RM.TextureMutexHandle, RM.milliseconds);

		Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(materialName);

		if(textureName == "PaintTexture" || painting)
		{			
			mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Blend_Flipped");
			painter->BufferData(RM.PpaintList[paintID]->Pixels);
			mat->m_textures[Render::TextureSemantic::TEXTUREMAP] = painter;
		}
		else if(painted)
		{
			mat->m_textures[Render::TextureSemantic::TEXTUREMAP] = g_engineContext.m_resourceManager->LoadTexture(textureName, Render::TextureType::TEXTURE_2D);
			mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Blend");
		}
		
		mat->m_textures[Render::TextureSemantic::TEXTURE_R] = g_engineContext.m_resourceManager->LoadTexture(RM.PpaintList[paintID]->textureRed, Render::TextureType::TEXTURE_2D);
		mat->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		mat->m_textures[Render::TextureSemantic::TEXTURE_R]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

		mat->m_textures[Render::TextureSemantic::TEXTURE_G] = g_engineContext.m_resourceManager->LoadTexture(RM.PpaintList[paintID]->textureGreen, Render::TextureType::TEXTURE_2D);
		mat->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		mat->m_textures[Render::TextureSemantic::TEXTURE_G]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

		mat->m_textures[Render::TextureSemantic::TEXTURE_B] = g_engineContext.m_resourceManager->LoadTexture(RM.PpaintList[paintID]->textureBlue, Render::TextureType::TEXTURE_2D);
		mat->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		mat->m_textures[Render::TextureSemantic::TEXTURE_B]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

		//temp = RM.PpaintList[paintID]->textureRed;

		//if (fexists(g_textureLookPath + temp + "Normal.dds"))
		//{
		//	NormalName = temp + "Normal";
		//	mat->m_textures[Render::TextureSemantic::TEXTURE_RN] = g_engineContext.m_resourceManager->LoadTexture(NormalName, Render::TextureType::TEXTURE_2D);
		//	mat->m_textures[Render::TextureSemantic::TEXTURE_RN]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	mat->m_textures[Render::TextureSemantic::TEXTURE_RN]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		//}

		//temp = RM.PpaintList[paintID]->textureGreen;

		//if (fexists(g_textureLookPath + temp + "Normal.dds"))
		//{
		//NormalName = temp + "Normal";
		//mat->m_textures[Render::TextureSemantic::TEXTURE_GN] = g_engineContext.m_resourceManager->LoadTexture(NormalName, Render::TextureType::TEXTURE_2D);
		//mat->m_textures[Render::TextureSemantic::TEXTURE_GN]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		//mat->m_textures[Render::TextureSemantic::TEXTURE_GN]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		//}

		//temp = RM.PpaintList[paintID]->textureBlue;

		//if (fexists(g_textureLookPath + temp + "Normal.dds"))
		//{
		//NormalName = temp + "Normal";
		//mat->m_textures[Render::TextureSemantic::TEXTURE_BN] = g_engineContext.m_resourceManager->LoadTexture(NormalName, Render::TextureType::TEXTURE_2D);
		//mat->m_textures[Render::TextureSemantic::TEXTURE_BN]->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
		//mat->m_textures[Render::TextureSemantic::TEXTURE_BN]->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
		//}



		//UPDATE TILEFACTOR
		if(meshID != -1)
		{
			RootForce::Renderable* rendy = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[meshID]);
			if(mat->m_effect == g_engineContext.m_resourceManager->GetEffect("Mesh_Blend") || mat->m_effect == g_engineContext.m_resourceManager->GetEffect("Mesh_Blend_Flipped"))
			{
				rendy->m_params[Render::Semantic::SIZEMIN] = &mat->m_tileFactor;
			}

			mat->m_tileFactor = RM.PpaintList[paintID]->tileFactor;	
		}
	

		ReleaseMutex(RM.TextureMutexHandle);
	}
	else
	{
		Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(materialName);
		mat->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture(textureName, Render::TextureType::TEXTURE_2D);
		if(specularMap != "" && specularMap != "NONE")
		{
			mat->m_textures[Render::TextureSemantic::SPECULAR] = g_engineContext.m_resourceManager->LoadTexture(specularMap, Render::TextureType::TEXTURE_2D);
		}
		if(glowMap != "" && glowMap != "NONE")
		{
			mat->m_textures[Render::TextureSemantic::GLOW] = g_engineContext.m_resourceManager->LoadTexture(glowMap, Render::TextureType::TEXTURE_2D);
		}
		if(normalMap != "" && normalMap != "NONE" && !transparent)
		{
			mat->m_textures[Render::TextureSemantic::NORMAL] = g_engineContext.m_resourceManager->LoadTexture(normalMap, Render::TextureType::TEXTURE_2D);
			mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_NormalMap");
		}
		else if(normalMap != "" && normalMap != "NONE" && transparent)
		{
			mat->m_textures[Render::TextureSemantic::NORMAL] = g_engineContext.m_resourceManager->LoadTexture(normalMap, Render::TextureType::TEXTURE_2D);
			mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh_Normal_Trans");
		}
		else
		{
			mat->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
		}
			
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
	locatorEntities.push_back(entity);

	transform->m_position = RM.PlocatorList[index]->transformation.position;

	particle->m_particleSystems = g_engineContext.m_resourceManager->LoadParticleEmitter(p_name, false);
	for(unsigned i = 0; i < particle->m_particleSystems.size(); i++)
		particle->m_systems.push_back(g_engineContext.m_renderer->CreateParticleSystem());

	for(int i = 0; i < RM.PlocatorList[index]->transformation.nrOfFlags; i++)
	{
		p_world->GetGroupManager()->RegisterEntity(RM.PlocatorList[index]->transformation.flags[i], entity);
	}

	return entity;
}

void UpdateParticle(int index)
{
	RootForce::Transform* transform =  m_world.GetEntityManager()->GetComponent<RootForce::Transform>(locatorEntities[index]);
	
	transform->m_position = RM.PlocatorList[index]->transformation.position;
}

void CreateCameraEntity(int index)
{
	//Create camera entity
	cameras.push_back(m_world.GetEntityManager()->CreateEntity());
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(cameras[index]);
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(cameras[index]);
	m_world.GetTagManager()->RegisterEntity("Camera", cameras[index]);
	m_world.GetGroupManager()->RegisterEntity("NonExport", cameras[index]);

	UpdateCamera(index);
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



void UpdateCamera(int index)
{
	if(index != -1)
	{

		RootForce::Transform* cameraTransform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(cameras[0]);
		RootForce::Camera* camera = m_world.GetEntityManager()->GetComponent<RootForce::Camera>(cameras[0]);

		RM.CameraMutexHandle = CreateMutex(nullptr, false, L"CameraMutex");
		WaitForSingleObject(RM.CameraMutexHandle, RM.milliseconds);

		glm::quat rotation;
		rotation.x = RM.PcameraList[index]->transformation.rotation.x;
		rotation.y = RM.PcameraList[index]->transformation.rotation.y;
		rotation.z = RM.PcameraList[index]->transformation.rotation.z;
		rotation.w = RM.PcameraList[index]->transformation.rotation.w;

		cameraTransform->m_position.x = RM.PcameraList[index]->transformation.position.x;
		cameraTransform->m_position.y = RM.PcameraList[index]->transformation.position.y;
		cameraTransform->m_position.z = RM.PcameraList[index]->transformation.position.z;
		cameraTransform->m_orientation.SetOrientation(rotation);
		//Rotate 180 to fix camera
		cameraTransform->m_orientation.Yaw(180);

		camera->m_frustum.m_far = RM.PcameraList[index]->farClippingPlane;					
		camera->m_frustum.m_near = RM.PcameraList[index]->nearClippingPlane;
		camera->m_frustum.m_fov = glm::degrees(RM.PcameraList[index]->verticalFieldOfView);

		ReleaseMutex(RM.CameraMutexHandle);
	}
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
				locatorEntities.push_back(CreateParticleEntity(&m_world, RM.PlocatorList[i]->transformation.name, i));
			}
		}

		for(int j = 0; j < RM.PlocatorList[i]->transformation.nrOfFlags; j++)
		{
			string flagName = "SpawnPoint";
			string flagName2 = RM.PlocatorList[i]->transformation.flags[j];

			if(flagName == flagName2)
			{
				locatorEntities.push_back(CreateTransformEntity(&m_world, i));
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

	if(MeshIndex != -1)					
	{
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
			//cout << "Updating " << RM.PmeshList[MeshIndex]->transformation.name << " at index: " << MeshIndex << endl;
		}

		if(updateTransformation)
		{
			// TRANSFORM AND SCALE
			RootForce::Transform* transform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(Entities[MeshIndex]);

			transform->m_position = RM.PmeshList[MeshIndex]->transformation.position;
			transform->m_scale = RM.PmeshList[MeshIndex]->transformation.scale;

			////Update material list
			CreateMaterial(GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->texturePath), GetNameFromPath(RM.PmeshList[MeshIndex]->materialName),GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->normalPath), GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->specularPath),GetNameFromPath(RM.PmaterialList[RM.PmeshList[MeshIndex]->MaterialID]->glowPath), MeshIndex);
			
			RootForce::Renderable* rendy = m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex]);

			Render::Material* mat = g_engineContext.m_resourceManager->GetMaterial(GetNameFromPath(RM.PmeshList[MeshIndex]->materialName));
			//if(mat->m_effect == g_engineContext.m_resourceManager->GetEffect("Mesh_Blend") || mat->m_effect == g_engineContext.m_resourceManager->GetEffect("Mesh_Blend_Flipped"))
			//{
			//	rendy->m_params[Render::Semantic::SIZEMIN] = &mat->m_tileFactor;
			//}

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

			//cout << "position x " << transform->m_position.x << endl;
			//cout << "position y " << transform->m_position.y << endl;
			//cout << "position z " << transform->m_position.z << endl;

			glm::mat3 rotationTest = transform->m_orientation.GetMatrix();

			float x,y,z;
			x = atan2(rotationTest[2][1], rotationTest[2][2]);
			y = atan2(-rotationTest[2][0], sqrt(pow(rotationTest[2][1],2) + pow(rotationTest[2][2],2)));
			z = atan2(rotationTest[1][0], rotationTest[0][0]);

				
			//cout << "rot x " << -glm::degrees(x) << endl;
			//cout << "rot y " << -glm::degrees(y) << endl;
			//cout << "rot z " << -glm::degrees(z) << endl;

			// Get material connected to mesh and set it from materiallist
			
			rendy->m_material = g_engineContext.m_resourceManager->GetMaterial(GetNameFromPath(RM.PmeshList[MeshIndex]->materialName));	

			
		}
		

		if(updateShape)
		{
		// COPY VERTICES
		Render::Vertex1P1N1UV1T1BT* m_vertices;
		m_vertices = new Render::Vertex1P1N1UV1T1BT[g_maxVerticesPerMesh];

		for(int j = 0; j < RM.PmeshList[MeshIndex]->nrOfVertices; j++)
		{
			m_vertices[j].m_pos = RM.PmeshList[MeshIndex]->vertex[j];
			m_vertices[j].m_normal = RM.PmeshList[MeshIndex]->normal[j];
			m_vertices[j].m_UV = RM.PmeshList[MeshIndex]->UV[j];
			m_vertices[j].m_tangent = RM.PmeshList[MeshIndex]->tangent[j];
			m_vertices[j].m_bitangent = RM.PmeshList[MeshIndex]->binormal[j];

		}

		// SET INFORMATION TO GAME
		m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexBuffer(g_engineContext.m_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->SetVertexAttribute(g_engineContext.m_renderer->CreateVertexAttributes());
		m_world.GetEntityManager()->GetComponent<RootForce::Renderable>(Entities[MeshIndex])->m_model->m_meshes[0]->CreateVertexBuffer1P1N1UV1T1BT(reinterpret_cast<Render::Vertex1P1N1UV1T1BT*>(m_vertices), RM.PmeshList[MeshIndex]->nrOfVertices); 

		}

		
	}

	//Remove mesh at index
	if(RemoveMeshIndex >= 0)	
	{
		m_world.GetEntityManager()->RemoveAllComponents(Entities[Entities.size()-1]);
		m_world.GetEntityManager()->RemoveEntity(Entities[Entities.size()-1]);
		Entities.pop_back();
	}

	ReleaseMutex(RM.MeshMutexHandle);
}

void UpdateLight(int index, bool remove, bool firstTimeLoad)
{
	int LightIndex = -1;
	int RemoveLightIndex = -1;
	bool ambientInfoExists = false;

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
	string type = RM.PlightList[LightIndex]->LightType;

	if(LightIndex != -1)					
	{		
		RM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
		WaitForSingleObject(RM.MeshMutexHandle, RM.milliseconds);

		if(type == "PointLight")
		{
			if(LightIndex > size)
			{
				LightEntities.push_back(CreateLightEntity(&m_world));
			}

			m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_position = RM.PlightList[LightIndex]->transformation.position;
			m_world.GetEntityManager()->GetComponent<RootForce::Transform>(LightEntities[LightIndex])->m_scale = RM.PlightList[LightIndex]->transformation.scale;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.r = RM.PlightList[LightIndex]->color.r;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.g = RM.PlightList[LightIndex]->color.g;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.b = RM.PlightList[LightIndex]->color.b;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_color.a = RM.PlightList[LightIndex]->color.a;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.x = 0.0f;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.y = 1-0.1 * RM.PlightList[LightIndex]->Intensity;
			m_world.GetEntityManager()->GetComponent<RootForce::PointLight>(LightEntities[LightIndex])->m_attenuation.z = 0.0f;
		}

		if(type == "DirectionalLight" && firstTimeLoad)
		{
			Render::DirectionalLight dl;
			dl.m_direction = RM.PlightList[LightIndex]->direction;
			dl.m_color = RM.PlightList[LightIndex]->color;

			g_engineContext.m_renderer->AddDirectionalLight(dl, 0);
		}

		if(type == "AmbientLight")
		{
			worldSystem->SetAmbientLight(RM.PlightList[LightIndex]->color);
			ambientInfoExists = true;
		}

		if(!ambientInfoExists)
			g_engineContext.m_renderer->SetAmbientLight(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		ReleaseMutex(RM.LightMutexHandle);
	}

	if(RemoveLightIndex >= 0)	
	{					
		m_world.GetEntityManager()->RemoveAllComponents(LightEntities[LightEntities.size()-1]);
		m_world.GetEntityManager()->RemoveEntity(LightEntities[LightEntities.size()-1]);
		LightEntities.pop_back();
		//LightEntities.erase(LightEntities.begin() + RemoveLightIndex);
	}
}

void ExportToLevel()
{
	for(int i = 0; i < Entities.size(); i++)
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
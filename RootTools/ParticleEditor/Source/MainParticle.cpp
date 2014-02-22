#include <RootTools/ParticleEditor/Include/MainParticle.h>
#include <QtWidgets/QApplication>
#include <stdexcept>
#include <exception>
#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <glm/glm.hpp>
#include <RootEngine/Include/RootEngine.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_syswm.h> 

#undef main
int main(int argc, char *argv[])
{
	std::string path(argv[0]);
	std::string rootforcename = "ParticleEditor.exe";
	path = path.substr(0, path.size() - rootforcename.size());
	
	try 
	{
		QApplication a(argc, argv);
		ParticleEditor w;
		
		MainParticle m(path, &w);
		w.show();
		uint64_t old = SDL_GetPerformanceCounter();
		bool notExit = true;
		while(notExit)
		{
			notExit = w.CheckExit();
			
			

			uint64_t now = SDL_GetPerformanceCounter();
			float dt = (now - old) / (float)SDL_GetPerformanceFrequency();
			old = now;
			w.Update(dt);
			a.processEvents(QEventLoop::AllEvents);
			m.Update(dt);
		}
		
	} 
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		return 1;
	} 
	catch (...) 
	{
		std::cout << "Unknown exception" << std::endl;
		std::cin.get();
		return 1;
	}
	return 0;
}

MainParticle::MainParticle( std::string p_workingDirectory, ParticleEditor* p_particleEditorQt )
{
	m_particleEditorQt = p_particleEditorQt;
	m_workingDirectory = p_workingDirectory;
	g_world = &m_world;

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	if (m_engineModule == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");

	if (libInitializeEngine == nullptr)
	{
		throw std::runtime_error("Failed to load RootEngine - please check your installation");
	}

	g_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_INPUT | RootEngine::SubsystemInit::INIT_RENDER, p_workingDirectory);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		throw std::runtime_error("Failed to initialize SDL");
	}


	// TODO: Make these parameters (even?) more configurable.
	m_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(
		"Root Particle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		p_particleEditorQt->ui.frame->width(),
		p_particleEditorQt->ui.frame->height(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS),
		SDL_DestroyWindow);
	
	//////////////////////////////////////////////////////////////////////////
	//Get HWND from SDL window and change its position
	//////////////////////////////////////////////////////////////////////////
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);

	if(SDL_GetWindowWMInfo(m_window.get(), &windowInfo))
	{
		HWND handle = windowInfo.info.win.window;
		int x = p_particleEditorQt->ui.frame->pos().x();
		int y = p_particleEditorQt->ui.frame->pos().y() + p_particleEditorQt->ui.menuBar->height();
		BOOL result = SetWindowPos(handle, NULL, x, y, 0, 0, 
			SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

		SetParent(handle, (HWND)p_particleEditorQt->winId());
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_window == nullptr) 
	{
		throw std::runtime_error("Failed to create window");
	}
	// Setup the SDL context
	g_engineContext.m_renderer->SetupSDLContext(m_window.get());

	RootForce::Renderable::SetTypeId(RootForce::ComponentType::RENDERABLE);
	RootForce::Transform::SetTypeId(RootForce::ComponentType::TRANSFORM);
	RootForce::PointLight::SetTypeId(RootForce::ComponentType::POINTLIGHT);
	RootForce::Camera::SetTypeId(RootForce::ComponentType::CAMERA);
	RootForce::LookAtBehavior::SetTypeId(RootForce::ComponentType::LOOKATBEHAVIOR);
	RootForce::ThirdPersonBehavior::SetTypeId(RootForce::ComponentType::THIRDPERSONBEHAVIOR);
	RootForce::ParticleEmitter::SetTypeId(RootForce::ComponentType::PARTICLE);

	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Renderable>(1000);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Transform>(1000);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::PointLight>(1000);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::Camera>(10);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::LookAtBehavior>(100);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ThirdPersonBehavior>(10);
	g_world->GetEntityManager()->GetAllocator()->CreateList<RootForce::ParticleEmitter>(100);
	

	// Initialize render and point light system.
	m_renderingSystem = new RootForce::RenderingSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::RenderingSystem>(m_renderingSystem);

	m_renderingSystem->SetLoggingInterface(g_engineContext.m_logger);
	m_renderingSystem->SetRendererInterface(g_engineContext.m_renderer);

	m_pointLightSystem = new RootForce::PointLightSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::PointLightSystem>(m_pointLightSystem);

	m_particleSystem = new RootForce::ParticleSystem(g_world);
	g_world->GetSystemManager()->AddSystem<RootForce::ParticleSystem>(m_particleSystem);

	// Initialize camera systems.
	m_cameraSystem = new RootForce::CameraSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::CameraSystem>(m_cameraSystem);
	m_lookAtSystem = new RootForce::LookAtSystem(g_world, &g_engineContext);
	g_world->GetSystemManager()->AddSystem<RootForce::LookAtSystem>(m_lookAtSystem);
	
	g_engineContext.m_inputSys->LockMouseToCenter(false);
	
	//Create player aiming device
	m_aimingDevice = m_world.GetEntityManager()->CreateEntity();
	m_world.GetTagManager()->RegisterEntity("AimingDevice", m_aimingDevice);
	m_world.GetGroupManager()->RegisterEntity("NonExport", m_aimingDevice);
	RootForce::Transform* aimingDeviceTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(m_aimingDevice);
	aimingDeviceTransform->m_scale = glm::vec3(0.01f);
	RootForce::Renderable* aimingRenderable = m_world.GetEntityManager()->CreateComponent<RootForce::Renderable>(m_aimingDevice);
	aimingRenderable->m_material = g_engineContext.m_renderer->CreateMaterial("AimingDeviceRenderable");
	aimingRenderable->m_model = g_engineContext.m_resourceManager->LoadCollada("Primitives/sphere");
	aimingRenderable->m_material->m_textures[Render::TextureSemantic::DIFFUSE] = g_engineContext.m_resourceManager->LoadTexture("blockMana", Render::TextureType::TextureType::TEXTURE_2D);
	aimingRenderable->m_material->m_effect = g_engineContext.m_resourceManager->LoadEffect("Mesh");
	g_engineContext.m_renderer->SetAmbientLight(glm::vec4(1,1,1,1));

	// Add camera entity.	
	m_cameraEntity = m_world.GetEntityManager()->CreateEntity();
	RootForce::Camera* camera = m_world.GetEntityManager()->CreateComponent<RootForce::Camera>(m_cameraEntity);
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->CreateComponent<RootForce::Transform>(m_cameraEntity);
	RootForce::LookAtBehavior* cameraLookAt = m_world.GetEntityManager()->CreateComponent<RootForce::LookAtBehavior>(m_cameraEntity);
	RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world.GetEntityManager()->CreateComponent<RootForce::ThirdPersonBehavior>(m_cameraEntity);
	cameraTransform->m_position = glm::vec3(0);
	camera->m_frustum.m_near = 0.1f;
	camera->m_frustum.m_far = 1000.0f;
	camera->m_frustum.m_fov = 45.0f;


	cameraLookAt->m_targetTag = "AimingDevice";
	cameraLookAt->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	cameraThirdPerson->m_targetTag = "AimingDevice";
	cameraThirdPerson->m_displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	m_world.GetTagManager()->RegisterEntity("Camera", m_cameraEntity);
	m_world.GetGroupManager()->RegisterEntity("NonExport", m_cameraEntity);	

	p_particleEditorQt->SetContext(&g_engineContext);
	p_particleEditorQt->SetWorld(&m_world);
	p_particleEditorQt->SetWorkingDirectory(m_workingDirectory);
	p_particleEditorQt->SetAimingDevice(m_aimingDevice);
	p_particleEditorQt->Init();
	p_particleEditorQt->ConnectSignalsAndSlots();

	m_focusInterpolation = false;
	m_focusIntTime = 0.0f;
}

MainParticle::~MainParticle()
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);

}

void MainParticle::HandleEvents()
{
	if (g_engineContext.m_inputSys != nullptr)
		g_engineContext.m_inputSys->Reset();
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		if (g_engineContext.m_inputSys != nullptr)
			g_engineContext.m_inputSys->HandleInput(event);
	}
}

void MainParticle::Update( float p_delta )
{
	g_world->SetDelta(p_delta);
	g_engineContext.m_renderer->Clear();
	HandleEvents();
	UpdateInput();
	m_lookAtSystem->Process();
	m_cameraSystem->Process();
	UpdateThirdPerson();
	m_particleSystem->Process();
	m_pointLightSystem->Process();
	m_renderingSystem->Process();
	g_engineContext.m_renderer->Render();
	g_engineContext.m_renderer->Swap();
	
}

void MainParticle::UpdateInput()
{
	ECS::Entity* cameraEntity = m_world.GetTagManager()->GetEntityByTag("Camera");
	RootForce::ThirdPersonBehavior* cameraThirdPerson = m_world.GetEntityManager()->GetComponent<RootForce::ThirdPersonBehavior>(cameraEntity);
	RootForce::Transform* cameraTrans = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(cameraEntity);
	RootForce::Transform* trans = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_aimingDevice);
	//Set camera distance to aiming device by mouse scroll with a speed factor depending on distance from aiming device position
	float speedZoomFac = cameraThirdPerson->m_distance/50.0f;
	cameraThirdPerson->m_distance -= (g_engineContext.m_inputSys->GetScroll() * 4 * speedZoomFac);

	//If ALT is being pressed
	if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_LALT) == RootEngine::InputManager::KeyState::DOWN)
	{
		//Get mouse movement since last frame
		glm::ivec2 m_deltaMouseMovement = g_engineContext.m_inputSys->GetDeltaMousePos();
		
		//If left mouse button is being pressed
		if(g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::MouseButton::LEFT) == RootEngine::InputManager::KeyState::DOWN)
		{
			//Adjust yaw and pitch with delta mouse movement
			trans->m_orientation.YawGlobal(m_deltaMouseMovement.x * 0.3f);
			trans->m_orientation.Pitch(-m_deltaMouseMovement.y * 0.3f);
		}
		else if(g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::MouseButton::MIDDLE) == RootEngine::InputManager::KeyState::DOWN)
		{
			//Stop interpolation movement
			m_focusInterpolation = false;

			//Pan camera position
			trans->m_position += cameraTrans->m_orientation.GetUp() * -(float)m_deltaMouseMovement.y * 0.005f;
			trans->m_position += cameraTrans->m_orientation.GetRight() * (float)m_deltaMouseMovement.x * 0.005f;
			m_particleEditorQt->SetLookAtSpinBox(trans->m_position);
		}
		else if (g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::MouseButton::RIGHT) == RootEngine::InputManager::KeyState::DOWN)
		{
			//Zoom in or out depending by moving mouse when pressing ALT+RMB
			if(glm::abs(m_deltaMouseMovement.y) > glm::abs(m_deltaMouseMovement.x))
				cameraThirdPerson->m_distance += m_deltaMouseMovement.y * 0.3f * speedZoomFac;
			else
				cameraThirdPerson->m_distance += m_deltaMouseMovement.x * 0.3f * speedZoomFac;
		}
	}
	else if(g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::MouseButton::LEFT) == RootEngine::InputManager::KeyState::DOWN_EDGE)
	{
		RootForce::Camera* camera = m_world.GetEntityManager()->GetComponent<RootForce::Camera>(cameraEntity);
		//Send mouse coords to ray-sphere intersection test
		m_particleEditorQt->CheckRayVsObject(g_engineContext.m_inputSys->GetGlobalMousePos(), cameraTrans->m_position, camera->m_viewMatrix);
	}
	else if(g_engineContext.m_inputSys->GetKeyState(RootEngine::InputManager::MouseButton::MouseButton::LEFT) == RootEngine::InputManager::KeyState::DOWN)
	{
		//Get screen coordinates of mouse when pressing down LMB first time
		glm::ivec2 mousePos = g_engineContext.m_inputSys->GetGlobalMousePos();
	}

	//Check if focus button is pressed
	if(g_engineContext.m_inputSys->GetKeyState(SDL_SCANCODE_F) == RootEngine::InputManager::KeyState::DOWN_EDGE )
	{
		m_focusInterpolation = true;
		m_focusIntTime = 0.0f;
		m_toPosition = m_particleEditorQt->FocusButtonClicked();
		m_fromPosition = trans->m_position;
	}

	if(m_focusInterpolation)
	{
		//0.2 sec interpolation
		m_focusIntTime += g_world->GetDelta()*10.0f;
		if(m_focusIntTime >= 1.0f)
		{
			m_focusInterpolation = false;
			m_focusIntTime = 1.0f;
		}
		trans->m_position = glm::mix(m_fromPosition, m_toPosition , m_focusIntTime);
		m_particleEditorQt->SetLookAtSpinBox(trans->m_position);
	}

}

void MainParticle::UpdateThirdPerson()
{
	RootForce::ThirdPersonBehavior* thirdPersonBehavior = m_world.GetEntityManager()->GetComponent<RootForce::ThirdPersonBehavior>(m_cameraEntity);
	RootForce::Transform* cameraTransform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_cameraEntity);
	RootForce::Transform* targetTransform = m_world.GetEntityManager()->GetComponent<RootForce::Transform>(m_aimingDevice);
	
	//Move the entity
	glm::vec3 targetPosition = targetTransform->m_position;
	RootForce::Orientation tOrientation = targetTransform->m_orientation;
	glm::vec3 localDisplacement(0.0f);
	localDisplacement.z = -thirdPersonBehavior->m_distance;
	glm::vec3 worldDisplacement;
	worldDisplacement = tOrientation.GetRight() * -localDisplacement.x + tOrientation.GetUp() * localDisplacement.y + tOrientation.GetFront() * localDisplacement.z;
	cameraTransform->m_position = targetPosition + worldDisplacement;
}

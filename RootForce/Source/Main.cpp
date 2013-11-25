#include <Main.h>

#include <stdexcept>

#include <RootEngine/Include/Logging/Logging.h>

#include <RootEngine/Render/Include/Renderer.h>


#include <gtest/gtest.h>

#include <Utility/DynamicLoader/Include/DynamicLoader.h>
#include <RootEngine/Include/RootEngine.h>

#include <ECS/Tests/TestSystem.h>

#include <exception>
#include <gtest/gtest.h>

#include <glm/glm.hpp>

#undef main

TEST(Test, Foo) 
{
	int a = 0;
	EXPECT_TRUE(a == 0);
}

int main(int argc, char* argv[]) 
{
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
			Main m;
			m.Start();
		}
	} 
	catch (std::exception& e) 
	{
		// TODO: Log exception message
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

Main::Main() 
	: m_running(true)
{

	m_engineModule = DynamicLoader::LoadSharedLibrary("RootEngine.dll");

	INITIALIZEENGINE libInitializeEngine = (INITIALIZEENGINE)DynamicLoader::LoadProcess(m_engineModule, "InitializeEngine");
	m_engineContext = libInitializeEngine(RootEngine::SubsystemInit::INIT_NETWORK | RootEngine::SubsystemInit::INIT_RENDER);

	m_engineContext.m_logger->LogText("Hello world from logger");

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
}

Main::~Main() 
{
	SDL_Quit();
	DynamicLoader::FreeSharedLibrary(m_engineModule);
}

void Main::Start() 
{

	//Open the log file stream for this instance(Do this once at the beginning of the program)
	//Logging::GetInstance()->OpenLogStream();

	//Include Logging.h in the file you want to use the logging function
	//Write a string to the log file stream(Do this when you want to log something...)
	//Logging::GetInstance()->LogTextToFile("Log %f entry test ", 1.435);
	
	//Write a log string to console
	//Logging::GetInstance()->LogTextToConsole("Console entry test %d", 12);

	int numVertices = 3;
	float vertices[] = {
		0.5f, 0.5f, 1.f, 0.f, 0.f, 1.f,
		-0.5f, 0.5f, 1.f, 0.f, 0.0f, 1.f,
		-0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f,
	};

	int numIndices = 3;
	GLuint indices[] = {
		0, 1, 2
	};

	std::shared_ptr<Render::BufferInterface> vertexBuffer = m_engineContext.m_renderer->CreateBuffer();
	vertexBuffer->Init(GL_ARRAY_BUFFER);
	vertexBuffer->BufferData(numVertices, 6 * sizeof(float), vertices); 

	std::shared_ptr<Render::BufferInterface> indexBuffer = m_engineContext.m_renderer->CreateBuffer();
	indexBuffer->Init(GL_ELEMENT_ARRAY_BUFFER);
	indexBuffer->BufferData(numIndices, sizeof(GLuint), indices);

	std::shared_ptr<Render::VertexAttributesInterface> attrs = m_engineContext.m_renderer->CreateVertexAttributes();
	attrs->Init(2);
	attrs->SetVertexAttribPointer(vertexBuffer->GetBufferId(), 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	attrs->SetVertexAttribPointer(vertexBuffer->GetBufferId(), 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)0 + 3 * sizeof(float));

	Render::Uniforms uniforms;
	uniforms.m_normal = glm::mat4(1);
	uniforms.m_world = glm::mat4(1);

	Render::RenderJob job;

	job.m_vertexBuffer = vertexBuffer;
	job.m_indexBuffer = indexBuffer;
	job.m_attributes = attrs;
	job.m_uniforms = &uniforms;

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

		angle += 0.0000001f;
		uniforms.m_world = glm::rotate<float>(uniforms.m_world, angle, 0.0f, 1.0f, 0.0f);
		uniforms.m_normal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(uniforms.m_world))));

		m_engineContext.m_renderer->AddRenderJob(&job);

		m_engineContext.m_renderer->Render();
	}
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

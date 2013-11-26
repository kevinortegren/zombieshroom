#include "guiInstance.h"
#include "Logging\Logging.h"
#include <gtest/gtest.h>

namespace RootEngine
{
	namespace GUISystem
	{
		guiInstance* guiInstance::s_gui = nullptr;
		RootEngine::SubsystemSharedContext g_context;
		void guiInstance::Startup(void)
		{
			m_sysInterface = new guiSysInterface();
			m_renderInterface = new guiRenderInterface();
			m_fileInterface = new guiFileInterface();

			m_rootContext = NULL;

			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Succesfull startup of GUI");
		}


		void guiInstance::Shutdown(void)
		{
			m_rootContext->RemoveReference();
			Rocket::Core::Shutdown();

			delete m_sysInterface;
			delete m_renderInterface;
			delete m_fileInterface;
			delete s_gui;
		}

		void guiInstance::Initalize(float p_width, float p_height)
		{

			Rocket::Core::SetSystemInterface(m_sysInterface);
			m_sysInterface->SetElapsedTime(0.1f);

			Rocket::Core::SetRenderInterface(m_renderInterface);
			m_renderInterface->SetViewport(p_width,p_height);

			Rocket::Core::SetFileInterface(m_fileInterface);

			Rocket::Core::Initialise();

			Rocket::Core::FontDatabase::LoadFontFace("C://Windows//Fonts//arial.ttf");

			m_rootContext = Rocket::Core::CreateContext("root", Rocket::Core::Vector2i(p_width, p_height));
			if(m_rootContext == nullptr)
				g_context.m_logger->LogText(LogTag::GUI, 1, "Error: failed to create gui context");
		}

		bool guiInstance::AttachDocument(std::string p_path)
		{

			Rocket::Core::ElementDocument* document = m_rootContext->LoadDocument((m_workingDir + p_path).c_str());
			document->AppendChild(document->CreateTextNode("man cakes"));
			if(document != NULL)
			{
				document->Show();
				document->RemoveReference();
				return true;
			}

			g_context.m_logger->LogText(LogTag::GUI, 2, "Warning: failed to load document %s", p_path.c_str());
			return false;
		}

		void guiInstance::Update( float p_time )
		{
			m_sysInterface->SetElapsedTime(p_time);
			m_rootContext->Update();
			m_rootContext->Render();
		}

		guiInstance* guiInstance::GetInstance()
		{
			if(!s_gui)
				s_gui = new guiInstance();

			return s_gui;
		}

		
	}
}

RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::GUISystem::g_context = p_context;

	return RootEngine::GUISystem::guiInstance::GetInstance();
}

TEST(GUI, GUITest1)
{
	EXPECT_FALSE(RootEngine::GUISystem::guiInstance::GetInstance()->AttachDocument("/errorPath/noFind.rml"));

	EXPECT_TRUE(RootEngine::GUISystem::guiInstance::GetInstance()->AttachDocument("Assets/GUI/demo.rml"));
}
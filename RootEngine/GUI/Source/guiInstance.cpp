#include "guiInstance.h"
#include "Logging\Logging.h"
#include <gtest/gtest.h>
#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
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

			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::DEBUG_PRINT, "Successful startup of GUI");
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

			Rocket::Core::FontDatabase::LoadFontFace("C://Windows//Fonts//arial.ttf"); //TODO: Remove once testing is completed

			m_rootContext = Rocket::Core::CreateContext("root", Rocket::Core::Vector2i(p_width, p_height));
			if(m_rootContext == nullptr)
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::FATAL_ERROR, "Error: failed to create gui context");
		}

		std::shared_ptr<Rocket::Core::ElementDocument> guiInstance::AttachDocument(std::string p_path)
		{
			std::shared_ptr<Rocket::Core::ElementDocument> document(m_rootContext->LoadDocument((m_workingDir + p_path).c_str()));

			if(document != NULL)
			{
				document->Show();
				return document;
			}

			g_context.m_logger->LogText(LogTag::GUI, LogLevel::WARNING, "Warning: failed to load document %s", p_path.c_str());
			return nullptr;
		}

		void guiInstance::LoadFont(std::string p_path)
		{
			if(!Rocket::Core::FontDatabase::LoadFontFace((m_workingDir + p_path).c_str()))
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::WARNING, "Warning: failed to load Font %s", p_path.c_str());
		}

		void guiInstance::Update( float p_time )
		{
			m_sysInterface->SetElapsedTime(p_time);
			m_rootContext->Update();
			m_rootContext->Render();
			
		}

		void guiInstance::HandleEvent(SDL_Event p_event)
		{
			glm::vec2 mousePos;
			switch(p_event.type)
			{
			case SDL_KEYDOWN:
					KeyButtonState(MapToRocket(p_event.key.keysym.scancode), true);
				break;
			case SDL_KEYUP:
				KeyButtonState(MapToRocket(p_event.key.keysym.scancode), false);
				break;
			case SDL_MOUSEBUTTONDOWN:
				MouseButtonState(MapToRocket(p_event.button.button-SDL_BUTTON_LEFT+InputManager::MouseButton::LEFT), true);
				break;
			case SDL_MOUSEBUTTONUP:
				MouseButtonState(MapToRocket(p_event.button.button-SDL_BUTTON_LEFT+InputManager::MouseButton::LEFT), false);
				break;
			case SDL_MOUSEMOTION:
				mousePos.x = static_cast<float>(p_event.motion.x);
				mousePos.y = static_cast<float>(p_event.motion.y);
				MouseMovement(mousePos);
				break;
			default:
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::MASS_DATA_PRINT, "Event %d did not match any case", p_event.type);
			}
		}

		Rocket::Core::Input::KeyIdentifier guiInstance::MapToRocket(SDL_Keycode p_key)
			{
			if(p_key >= SDL_Scancode::SDL_SCANCODE_A && p_key <= SDL_Scancode::SDL_SCANCODE_Z)
				return (Rocket::Core::Input::KeyIdentifier)(p_key + Rocket::Core::Input::KeyIdentifier::KI_A - SDL_Scancode::SDL_SCANCODE_A);
			else if(p_key >= SDL_Scancode::SDL_SCANCODE_1 && p_key <= SDL_Scancode::SDL_SCANCODE_9)
				return (Rocket::Core::Input::KeyIdentifier)(p_key + Rocket::Core::Input::KeyIdentifier::KI_1 - SDL_Scancode::SDL_SCANCODE_1);
			else if(p_key == SDL_Scancode::SDL_SCANCODE_0)
				return Rocket::Core::Input::KeyIdentifier::KI_0;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_SPACE)
				return Rocket::Core::Input::KeyIdentifier::KI_SPACE;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_RETURN)
				return Rocket::Core::Input::KeyIdentifier::KI_RETURN;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_BACKSPACE)
				return Rocket::Core::Input::KeyIdentifier::KI_BACK;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_TAB)
				return Rocket::Core::Input::KeyIdentifier::KI_TAB;
			else if(p_key == InputManager::MouseButton::LEFT)
				return (Rocket::Core::Input::KeyIdentifier)0;
			else if(p_key == InputManager::MouseButton::RIGHT)
				return (Rocket::Core::Input::KeyIdentifier)1;
			else
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::MASS_DATA_PRINT, "Key %d not mapped for libRocket", p_key);
		}

		void guiInstance::MouseMovement(glm::vec2 p_pos)
		{
			//m_rootContext->ProcessMouseMove((int)p_pos.x, (int)p_pos.y, 0); //TODO: not sure if the last argument should be 0
		}

		void guiInstance::MouseButtonState(Rocket::Core::Input::KeyIdentifier p_buttonIndex, bool p_isDown)
		{
			if(p_isDown) //Button down
			{
				m_rootContext->ProcessMouseButtonDown(p_buttonIndex, 0); //TODO: not sure if the last argument should be 0
			}
			else //Button up
			{
				m_rootContext->ProcessMouseButtonUp(p_buttonIndex, 0); //TODO: not sure if the last argument should be 0
			}
		}

		void guiInstance::KeyButtonState(Rocket::Core::Input::KeyIdentifier p_buttonIndex, bool p_isDown)
		{
			if(p_isDown) //Button down
			{
				m_rootContext->ProcessKeyDown(p_buttonIndex, 0); //TODO: not sure if the last argument should be 0
			}
			else //Button up
			{
				m_rootContext->ProcessKeyUp(p_buttonIndex, 0); //TODO: not sure if the last argument should be 0
			}
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
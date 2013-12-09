#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <Awesomium/STLHelpers.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include "guiInstance.h"
#include "Logging/Logging.h"

namespace RootEngine
{
	namespace GUISystem
	{
		guiInstance* guiInstance::s_gui = nullptr;
		RootEngine::SubsystemSharedContext g_context;

		void guiInstance::Startup(void)
		{
			m_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
			g_context.m_logger->LogText(LogTag::GUI, LogLevel::INIT_PRINT, "GUI subsystem initialized!");
			m_glTexSurfaceFactory = new GLTextureSurfaceFactory();
			m_core->set_surface_factory(m_glTexSurfaceFactory);

		}


		void guiInstance::Shutdown(void)
		{
			m_view->Stop();
			m_view->Destroy();
			//Awesomium::WebCore::Shutdown(); // This causes the program to freeze, but does not seem necessary. Code remains for future reference.

			glDeleteTextures(1, &m_texture);
			glDeleteVertexArrays(1, &m_vertexArrayBuffer);
			delete m_glTexSurfaceFactory;
		}

		void guiInstance::Initialize( int p_width, int p_height )
		{
			m_width = p_width;
			m_height = p_height;
			m_view = m_core->CreateWebView(m_width, m_height);
			
			m_view->SetTransparent(true);
  
			g_context.m_resourceManager->LoadEffect("2D_GUI");
			m_program = g_context.m_resourceManager->GetEffect("2D_GUI")->GetTechniques()[0]->GetPrograms()[0];

			// Prepare a texture for output
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(1, &m_texture);
			glBindTexture(GL_TEXTURE_2D, m_texture);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			// Prepare a quad for texture output
			glGenVertexArrays(1, &m_vertexArrayBuffer);
			glBindVertexArray(m_vertexArrayBuffer);
			GLuint vertexBufferObject;
			glGenBuffers(1, &vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			float quadVertices[] = {
				-1.f, 1.f, 0.f, 0.f,
				-1.f, -1.f, 0.f, 1.f,
				1.f, 1.f, 1.f, 0.f,
				1.f, -1.f, 1.f, 1.f
			};
			glBufferData(GL_ARRAY_BUFFER, 4*4*sizeof(float), quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (char*)NULL);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (char*)NULL+2*sizeof(float));
			// Done, unbind VAO
			glBindVertexArray(0);
		}

		void guiInstance::Update()
		{
			m_core->Update();
		}

		void guiInstance::Render()
		{
			m_program->Apply();

			glBindVertexArray(m_vertexArrayBuffer);

			glActiveTexture(GL_TEXTURE0);
			SurfaceToTexture((GLTextureSurface*)m_view->surface());

			/*glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glDisable(GL_DEPTH_TEST);*/

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			/*glEnable(GL_DEPTH_TEST);
			glDisable( GL_BLEND );*/

			glBindVertexArray(0);
		}

		guiInstance* guiInstance::GetInstance()
		{
			if(!s_gui)
				s_gui = new guiInstance();

			return s_gui;
		}

		void guiInstance::LoadURL( std::string p_path )
		{
			Awesomium::WebURL url(Awesomium::WSLit(("file://" + m_workingDir + "Assets/GUI/" + p_path).c_str()));

			m_view->LoadURL(url);
		}

		void guiInstance::SurfaceToTexture(GLTextureSurface* p_surface)
		{
			if(p_surface)
				glBindTexture(GL_TEXTURE_2D, p_surface->GetTexture());

			/*if(m_view->IsLoading() || p_surface == 0 || !p_surface->is_dirty())
				return;*/

			//glInvalidateTexImage(m_texture, 0);
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, p_surface->buffer());
		}

		void guiInstance::HandleEvents( SDL_Event p_event )
		{
			Awesomium::WebKeyboardEvent tempEvent;
			char* temp = new char[20];
			int keyCheck = 0;
			switch(p_event.type)
			{
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				keyCheck = MapToAwesomium(p_event.key.keysym.scancode);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				keyCheck = MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT);
				break;
			}
			if(keyCheck == -1)
				return;
			switch(p_event.type)
			{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					tempEvent.virtual_key_code = MapToAwesomium(p_event.key.keysym.scancode);
					Awesomium::GetKeyIdentifierFromVirtualKeyCode(tempEvent.virtual_key_code, &(temp));

					memcpy(tempEvent.key_identifier, temp, sizeof(char)*20 );

					if(p_event.type == SDL_KEYDOWN)
						tempEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;
					if(p_event.type == SDL_KEYUP)
						tempEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;
					m_view->InjectKeyboardEvent(tempEvent);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_view->InjectMouseDown((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEBUTTONUP:
					m_view->InjectMouseUp((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEMOTION:
					m_view->InjectMouseMove(p_event.motion.x, p_event.motion.y);
					break;
				default:
					g_context.m_logger->LogText(LogTag::INPUT, LogLevel::MASS_DATA_PRINT, "Event %d did not match any case", p_event.type); 
			}
		}

		int guiInstance::MapToAwesomium( SDL_Keycode p_key )
		{
			if(p_key >= SDL_Scancode::SDL_SCANCODE_A && p_key <= SDL_Scancode::SDL_SCANCODE_Z)
				return (int)(p_key + Awesomium::KeyCodes::AK_A - SDL_Scancode::SDL_SCANCODE_A);
			else if(p_key >= SDL_Scancode::SDL_SCANCODE_1 && p_key <= SDL_Scancode::SDL_SCANCODE_9)
				return (int)(p_key + Awesomium::KeyCodes::AK_1 - SDL_Scancode::SDL_SCANCODE_1);
			else if(p_key == SDL_Scancode::SDL_SCANCODE_0)
				return Awesomium::KeyCodes::AK_0;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_SPACE)
				return Awesomium::KeyCodes::AK_SPACE;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_RETURN)
				return Awesomium::KeyCodes::AK_RETURN;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_BACKSPACE)
				return Awesomium::KeyCodes::AK_BACK;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_TAB)
				return Awesomium::KeyCodes::AK_TAB;
			else if(p_key == InputManager::MouseButton::LEFT)
				return Awesomium::kMouseButton_Left;
			else if(p_key == InputManager::MouseButton::RIGHT)
				return Awesomium::kMouseButton_Right;
			else
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::MASS_DATA_PRINT, "Key %d not mapped for Awesomium", p_key);

			return -1;
		}


		void guiTest::OnDocumentReady(Awesomium::WebView* called, const Awesomium::WebURL& url)
		{
			m_testBool = true;
		}

		void guiJSTest::OnMethodCall (Awesomium::WebView *caller, unsigned int remote_object_id, const Awesomium::WebString &method_name, const Awesomium::JSArray &args)
		{
			m_testBool = true;
		}
	}
}

RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::GUISystem::g_context = p_context;

	return RootEngine::GUISystem::guiInstance::GetInstance();
}



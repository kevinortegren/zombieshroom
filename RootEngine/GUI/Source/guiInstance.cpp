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
		}


		void guiInstance::Shutdown(void)
		{
			m_shouldTerminate = true;
			m_thread.join();

			for(unsigned i = 0; i < m_viewBuffer.size(); i++)
			{
				delete m_viewBuffer.at(i);
			}
			//Awesomium::WebCore::Shutdown(); // This causes the program to freeze, but does not seem necessary. Code remains for future reference.

			glDeleteTextures(1, &m_texture);
			glDeleteVertexArrays(1, &m_vertexArrayBuffer);
			delete m_glTexSurfaceFactory;
		}

		void guiInstance::Initialize( int p_width, int p_height, SDL_Window* p_window, SDL_GLContext p_glContext )
		{
			m_width = p_width;
			m_height = p_height;
			m_window = p_window;
			m_glContext = p_glContext;
  
			g_context.m_resourceManager->LoadEffect("2D_GUI");
			m_program = g_context.m_resourceManager->GetEffect("2D_GUI")->GetTechniques()[0]->GetPrograms()[0];

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
			
			m_shouldTerminate = false;
			m_thread = std::thread(&guiInstance::RunThread, this);
		}

		void guiInstance::Update()
		{
			
		}

		void guiInstance::Render(WebView* p_view)
		{
			m_program->Apply();

			glBindVertexArray(m_vertexArrayBuffer);
			m_drawMutex.lock();
			glActiveTexture(GL_TEXTURE0);
			SurfaceToTexture((GLTextureSurface*)((WebViewImpl*)p_view)->m_webView->surface());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindTexture(GL_TEXTURE_2D, 0);
			m_drawMutex.unlock();
			
			glBindVertexArray(0);
		}

		guiInstance* guiInstance::GetInstance()
		{
			if(!s_gui)
				s_gui = new guiInstance();

			return s_gui;
		}

		WebView* guiInstance::LoadURL( std::string p_callbackObjectName, std::string p_path )
		{
			Awesomium::WebURL url(Awesomium::WSLit(("file://" + m_workingDir + "Assets/GUI/" + p_path).c_str()));
			//Awesomium::WebURL url(Awesomium::WSLit("http://www.google.com"));

			WebViewImpl* temp = new WebViewImpl(p_callbackObjectName, m_dispatcher);

			m_loadListMutex.lock();
			m_loadList.push_back(std::pair<WebViewImpl*,Awesomium::WebURL>(temp,url));
			m_loadListMutex.unlock();

			while(!temp->m_webView)
				Sleep(100);

			return temp;

		}

		void guiInstance::DestroyView( WebView* p_view )
		{
			m_viewBufferMutex.lock();
			for(unsigned i = 0; i < m_viewBuffer.size(); i++)
				if(m_viewBuffer.at(i) == p_view)
					m_viewBuffer.erase(m_viewBuffer.begin() + i--);
			m_viewBuffer.shrink_to_fit();
			m_viewBufferMutex.unlock();
		}

		void guiInstance::SurfaceToTexture(GLTextureSurface* p_surface)
		{
			SDL_GLContext tmp = SDL_GL_GetCurrentContext();
			GLuint texture = p_surface->GetTexture();
			if(p_surface)
				glBindTexture(GL_TEXTURE_2D, texture);
		}

		void guiInstance::HandleEvents( SDL_Event p_event )
		{
			Awesomium::WebKeyboardEvent tempEvent;
			
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
                case SDL_TEXTINPUT:
				{
					char temp[20];
					char* t = temp;

					tempEvent.virtual_key_code = MapToAwesomium(p_event.key.keysym.scancode);
					Awesomium::GetKeyIdentifierFromVirtualKeyCode(tempEvent.virtual_key_code, &t);

					tempEvent.modifiers = 0;
					if (p_event.key.keysym.mod & KMOD_LALT || p_event.key.keysym.mod & KMOD_RALT)
						tempEvent.modifiers |= Awesomium::WebKeyboardEvent::kModAltKey;
					if (p_event.key.keysym.mod & KMOD_LCTRL || p_event.key.keysym.mod & KMOD_RCTRL)
						tempEvent.modifiers |= Awesomium::WebKeyboardEvent::kModControlKey;
					if (p_event.key.keysym.mod & KMOD_LSHIFT || p_event.key.keysym.mod & KMOD_RSHIFT)
						tempEvent.modifiers |= Awesomium::WebKeyboardEvent::kModShiftKey;
					if (p_event.key.keysym.mod & KMOD_NUM)
						tempEvent.modifiers |= Awesomium::WebKeyboardEvent::kModIsKeypad;

					tempEvent.native_key_code = p_event.key.keysym.scancode;

					if(p_event.type == SDL_KEYDOWN)
						tempEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;
					if(p_event.type == SDL_TEXTINPUT)
					{
						tempEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;
						for (int i = 0; i < 4; i++) {
							tempEvent.text[i] = (wchar16)p_event.text.text[i];
							tempEvent.unmodified_text[i] = (wchar16)p_event.text.text[i];
						}
						// Use the first character of the text as the 'key code'
						tempEvent.virtual_key_code = (wchar16)p_event.text.text[0];
						tempEvent.native_key_code = (wchar16)p_event.text.text[0];
						Awesomium::GetKeyIdentifierFromVirtualKeyCode(tempEvent.virtual_key_code, &t);
					}
					if(p_event.type == SDL_KEYUP)
						tempEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;
                        
                    for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						m_viewBuffer.at(i)->InjectKeyboardEvent(tempEvent);

				} break;

				case SDL_MOUSEBUTTONDOWN:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						m_viewBuffer.at(i)->InjectMouseDown((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEBUTTONUP:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						m_viewBuffer.at(i)->InjectMouseUp((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEMOTION:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						m_viewBuffer.at(i)->InjectMouseMove(p_event.motion.x, p_event.motion.y);
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
			else if(p_key == SDL_Scancode::SDL_SCANCODE_RETURN || p_key == SDL_Scancode::SDL_SCANCODE_KP_ENTER)
				return Awesomium::KeyCodes::AK_RETURN;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_BACKSPACE)
				return Awesomium::KeyCodes::AK_BACK;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_TAB)
				return Awesomium::KeyCodes::AK_TAB;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_ESCAPE)
				return Awesomium::KeyCodes::AK_ESCAPE;
			else if(p_key == InputManager::MouseButton::LEFT)
				return Awesomium::kMouseButton_Left;
			else if(p_key == InputManager::MouseButton::RIGHT)
				return Awesomium::kMouseButton_Right;
			else
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::MASS_DATA_PRINT, "Key %d not mapped for Awesomium", p_key);

			return -1;
		}

		void guiInstance::RunThread()
		{
			SDL_GL_MakeCurrent(m_window, m_glContext);
			// Initialize Awesomium
			m_glTexSurfaceFactory = new GLTextureSurfaceFactory();
			m_dispatcher = new Dispatcher();
			m_core = Awesomium::WebCore::Initialize(Awesomium::WebConfig());
			g_context.m_logger->LogText(LogTag::GUI, LogLevel::INIT_PRINT, "GUI subsystem initialized!");
			m_core->set_surface_factory(m_glTexSurfaceFactory);
			
			uint64_t oldTime = SDL_GetPerformanceCounter();

			while(!m_shouldTerminate)
			{
				m_loadListMutex.lock();
				for(auto pair : m_loadList)
				{
					pair.first->m_webView = m_core->CreateWebView(m_width, m_height);

					pair.first->m_webView->SetTransparent(true);

					pair.first->m_webView->LoadURL(pair.second);
					
					m_viewBuffer.push_back(pair.first);
				}
				m_loadList.clear();
				m_loadListMutex.unlock();

				m_viewBufferMutex.lock();
				for(unsigned i = 0; i < m_viewBuffer.size(); i++)
					if(m_viewBuffer[i])
						m_viewBuffer[i]->Update();
				m_viewBufferMutex.unlock();

				m_drawMutex.lock();
				m_core->Update();
				for(unsigned i = 0; i < m_viewBuffer.size(); i++)
					if(m_viewBuffer[i] && m_viewBuffer[i]->GetView())
					{
						GLRAMTextureSurface* surface = (GLRAMTextureSurface*)m_viewBuffer[i]->GetView()->surface();
						if(surface)
							surface->UpdateTexture(); // Force a texture update
					}
				// Wait until texture updates are complete before releasing the lock
				GLsync fenceId = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 ); 
				GLenum result; 
				while(true) 
				{ 
					result = glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(5000000000)); //5 Second timeout 
					if(result != GL_TIMEOUT_EXPIRED) break; //we ignore timeouts and wait until all OpenGL commands are processed! 
				} 
				m_drawMutex.unlock();
				
				uint64_t newTime = SDL_GetPerformanceCounter();
				float dt = (newTime - oldTime) / (float)SDL_GetPerformanceFrequency();
				oldTime = newTime;
				if(dt < 0.16f)
					Sleep((DWORD)((0.016f-dt)*1000));
			}
			SDL_GL_DeleteContext(m_glContext);
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



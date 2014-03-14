#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <Awesomium/STLHelpers.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <exception>
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

			m_program->Apply();
			glUniform1i(glGetUniformLocation(m_program->GetHandle(), "outputWidth"), p_width);
			glUniform1i(glGetUniformLocation(m_program->GetHandle(), "outputHeight"), p_height);
			glUniform1i(glGetUniformLocation(m_program->GetHandle(), "tileSize"), TILE_SIZE);

			// Prepare a quad for texture output
			glGenVertexArrays(1, &m_vertexArrayBuffer);
			glBindVertexArray(m_vertexArrayBuffer);
			
			glGenBuffers(1, &m_vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, GL_MAX_TEXTURE_UNITS*6*sizeof(TileData), 0, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TileData), (char*)NULL);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TileData), (char*)NULL+2*sizeof(float));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(TileData), (char*)NULL+4*sizeof(float));
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
			WebViewImpl* view = (WebViewImpl*)p_view;
			if(!view->m_isActive)
				return;

			GLTextureSurface* surface = (GLTextureSurface*)view->GetView()->surface();
			if(!surface)
				return;

			// Buffer quads in a buffer until max amount of texture units is filled
			//   then issue a draw call and reset
			if(surface->GetShouldResize())
				m_resizeMutex.lock();

			unsigned numTexturesUsed = 0;
			float tileTexWidth = TILE_SIZE/(float)m_width*2;
			float tileTexHeight = TILE_SIZE/(float)m_height*2;

			m_program->Apply();
			glBindVertexArray(m_vertexArrayBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
			auto tiles = surface->GetTiles();
			for(unsigned x = 0; x < tiles->size(); ++x)
				for(unsigned y = 0; y < tiles->at(x).size(); ++y)
				{
					SurfaceTile& tile = tiles->at(x).at(y);

					glUniform1i(glGetUniformLocation(m_program->GetHandle(), ("tile["+std::to_string(numTexturesUsed)+"]").c_str()), numTexturesUsed);
					glActiveTexture(GL_TEXTURE0 + numTexturesUsed);
					glBindTexture(GL_TEXTURE_2D, tile.Texture);

					TileData quadVertices[] = {
						TileData(-1.f + tileTexWidth * x,     1.f - tileTexHeight * (y+1), 0.f, 1.f, numTexturesUsed),
						TileData(-1.f + tileTexWidth * (x+1), 1.f - tileTexHeight * (y+1), 1.f, 1.f, numTexturesUsed),
						TileData(-1.f + tileTexWidth * x,     1.f - tileTexHeight * y,     0.f, 0.f, numTexturesUsed),
						TileData(-1.f + tileTexWidth * x,     1.f - tileTexHeight * y,     0.f, 0.f, numTexturesUsed),
						TileData(-1.f + tileTexWidth * (x+1), 1.f - tileTexHeight * (y+1), 1.f, 1.f, numTexturesUsed),
						TileData(-1.f + tileTexWidth * (x+1), 1.f - tileTexHeight * y,     1.f, 0.f, numTexturesUsed),
					};

					glBufferSubData(GL_ARRAY_BUFFER, numTexturesUsed*6*sizeof(TileData), 6*sizeof(TileData), quadVertices);
					
					numTexturesUsed++;

					// If max textures used or last tile, do the rendering and restart the texture usage
					if(numTexturesUsed >= (unsigned)std::min(GL_MAX_TEXTURE_UNITS, 32) || (x + 1 == tiles->size() && y + 1 == tiles->at(x).size()))
					{
						glDrawArrays(GL_TRIANGLES, 0, numTexturesUsed*6);

						numTexturesUsed = 0;
					}
				}
			glBindVertexArray(0);
			if(surface->GetShouldResize())
				m_resizeMutex.unlock();
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

			while(!temp->m_webView || temp->m_webView->IsLoading())
				Sleep(100);

			return temp;

		}

		void guiInstance::DestroyView( WebView* p_view )
		{
			m_destroyListMutex.lock();
				m_destroyList.push_back((WebViewImpl*)p_view);
			m_destroyListMutex.unlock();
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
						if(m_viewBuffer.at(i)->m_isActive)
							m_viewBuffer.at(i)->InjectKeyboardEvent(tempEvent);

				} break;

				case SDL_MOUSEBUTTONDOWN:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						if(m_viewBuffer.at(i)->m_isActive)
							m_viewBuffer.at(i)->InjectMouseDown((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEBUTTONUP:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						if(m_viewBuffer.at(i)->m_isActive)
							m_viewBuffer.at(i)->InjectMouseUp((Awesomium::MouseButton)MapToAwesomium(p_event.button.button - SDL_BUTTON_LEFT + InputManager::MouseButton::LEFT));
					break;
				case SDL_MOUSEMOTION:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						if(m_viewBuffer.at(i)->m_isActive)
							m_viewBuffer.at(i)->InjectMouseMove(p_event.motion.x, p_event.motion.y);
					break;
					
				case SDL_MOUSEWHEEL:
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						if(m_viewBuffer.at(i)->m_isActive)
							m_viewBuffer.at(i)->InjectMouseWheel(p_event.wheel.x, p_event.wheel.y);
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
			else if(p_key == SDL_Scancode::SDL_SCANCODE_DELETE)
				return Awesomium::KeyCodes::AK_DELETE;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_TAB)
				return Awesomium::KeyCodes::AK_TAB;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_ESCAPE)
				return Awesomium::KeyCodes::AK_ESCAPE;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_HOME)
				return Awesomium::KeyCodes::AK_HOME;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_END)
				return Awesomium::KeyCodes::AK_END;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_PAGEUP)
				return Awesomium::KeyCodes::AK_PRIOR;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_PAGEDOWN)
				return Awesomium::KeyCodes::AK_NEXT;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_UP)
				return Awesomium::KeyCodes::AK_UP;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_DOWN)
				return Awesomium::KeyCodes::AK_DOWN;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_LEFT)
				return Awesomium::KeyCodes::AK_LEFT;
			else if(p_key == SDL_Scancode::SDL_SCANCODE_RIGHT)
				return Awesomium::KeyCodes::AK_RIGHT;
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
			try
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

					m_destroyListMutex.lock();
						m_viewBufferMutex.lock();
							for(auto view : m_destroyList)
								for(auto itr = m_viewBuffer.begin(); itr != m_viewBuffer.end(); ++itr)
									if(*itr == view)
									{
										delete *itr;
										m_viewBuffer.erase(itr);
										break;
									}
							m_destroyList.clear();
							m_viewBuffer.shrink_to_fit();
						m_viewBufferMutex.unlock();
					m_destroyListMutex.unlock();

					m_viewBufferMutex.lock();
						for(unsigned i = 0; i < m_viewBuffer.size(); i++)
							if(m_viewBuffer[i] && m_viewBuffer[i]->m_isActive)
								m_viewBuffer[i]->Update();
					m_viewBufferMutex.unlock();

					m_resizeMutex.lock();
						m_core->Update();
					m_resizeMutex.unlock();
					for(unsigned i = 0; i < m_viewBuffer.size(); i++)
						if(m_viewBuffer[i] && m_viewBuffer[i]->GetView() && m_viewBuffer[i]->m_isActive)
						{
							GLTextureSurface* surface = (GLTextureSurface*)m_viewBuffer[i]->GetView()->surface();
							if(surface)
								surface->UpdateTexture(); // Update textures if necessary
						}
				
					uint64_t newTime = SDL_GetPerformanceCounter();
					float dt = (newTime - oldTime) / (float)SDL_GetPerformanceFrequency();
					oldTime = newTime;

					if(dt < 0.032f)
					{
						long time = (long)floorf((0.032f-dt)*1000);
						Sleep(time);
					}
				}
				SDL_GL_DeleteContext(m_glContext);
				Awesomium::WebCore::Shutdown(); 
			}
			catch(std::exception e)
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::FATAL_ERROR, "Awesomium update thread has crashed due to interference in the Force! %s", e.what());
			}
			catch(...)
			{
				g_context.m_logger->LogText(LogTag::GUI, LogLevel::FATAL_ERROR, "Awesomium update thread has crashed due to interference in the Force!");
			}
		}
		
		void guiInstance::ResizeAllViews(int p_width, int p_height)
		{
			m_width = p_width;
			m_height = p_height;

			m_drawMutex.lock();
				m_viewBufferMutex.lock();
					for(auto view : m_viewBuffer )
					{
						if (view->m_webView->surface())
						{
							((GLTextureSurface*)view->m_webView->surface())->SetShouldResize(true);
						}

						view->m_webView->Resize(p_width, p_height);
						view->m_webView->ReduceMemoryUsage();
					}
				m_viewBufferMutex.unlock();
			m_drawMutex.unlock();
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



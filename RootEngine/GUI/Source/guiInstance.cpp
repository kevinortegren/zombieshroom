#include <gtest/gtest.h>
#include <Awesomium/STLHelpers.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <RootEngine/InputManager/Include/KeyStateMouseEnum.h>
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
			g_context.m_logger->LogText(LogTag::GUI, LogLevel::DEBUG_PRINT, "Succesfull startup of GUI");
		}


		void guiInstance::Shutdown(void)
		{
			m_view->Destroy();
			m_core->Shutdown();
			delete s_gui;
		}

		void guiInstance::SetWindow( int p_width, int p_height )
		{
			m_width = p_width;
			m_height = p_height;
			m_view = m_core->CreateWebView(p_width, p_height);
			
			//m_view->SetTransparent(true);

			// Prepare a texture for output
			glGenTextures(1, &m_texture);
			//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, p_width, p_height);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, p_width, p_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			// Prepare a quad for texture output
			glGenVertexArrays(1, &m_vertexArrayBuffer);
			glBindVertexArray(m_vertexArrayBuffer);
			GLuint vertexBufferObject;
			glGenBuffers(1, &vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			float quadVertices[] = {
				-1.f, 1.f, 0.f, 1.f,
				-1.f, -1.f, 0.f, 0.f,
				1.f, 1.f, 1.f, 1.f,
				1.f, -1.f, 1.f, 0.f
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
			m_effect->Apply();

			glBindVertexArray(m_vertexArrayBuffer);

			m_effect->SetUniformInt("texSampler", 0);
			glActiveTexture(GL_TEXTURE0);
			SurfaceToTexture((Awesomium::BitmapSurface*)m_view->surface());

			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glDisable(GL_DEPTH_TEST);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glEnable(GL_DEPTH_TEST);
			glDisable( GL_BLEND );

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

			/*while (m_view->IsLoading())
				m_core->Update();

			Sleep(300);
			m_core->Update();

			bool hej = ((Awesomium::BitmapSurface*)m_view->surface())->SaveToPNG(Awesomium::WSLit((m_workingDir + "name.PNG").c_str()));
*/
		}

		void guiInstance::SetRenderEffect( Render::EffectInterface* p_effect )
		{
			m_effect = p_effect;
		}

		void guiInstance::SurfaceToTexture(Awesomium::BitmapSurface* p_surface)
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);

			if(m_view->IsLoading() || p_surface == 0)
				return;

			const unsigned char* blargh = p_surface->buffer();
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, blargh);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, blargh);
			bool hej = p_surface->SaveToPNG(Awesomium::WSLit((m_workingDir + "name.png").c_str()));
		}

	}
}

RootEngine::GUISystem::GUISystemInterface* CreateGUI(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::GUISystem::g_context = p_context;

	return RootEngine::GUISystem::guiInstance::GetInstance();
}

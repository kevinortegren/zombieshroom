#pragma once

#include <RootEngine/Render/Include/Shadowcaster.h>
#include <RootEngine/Render/Include/Texture.h>

#define RENDER_MAX_SHADOWCASTERS 1

namespace Render
{
	class GLRenderer; 

	class ShadowDevice
	{
		//This class contains a depth texture atlas used for shadow mapping
	public:
		ShadowDevice();
		~ShadowDevice();

		void Init(GLRenderer* p_renderer, int p_width, int p_height);
	private:
		Shadowcaster m_shadowcasters[RENDER_MAX_SHADOWCASTERS];
		Texture* m_depthTexture;
		GLuint m_framebuffer;
	};
}
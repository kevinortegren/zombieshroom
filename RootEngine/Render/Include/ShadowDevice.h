#pragma once

#include <RootEngine/Render/Include/Shadowcaster.h>
#include <RootEngine/Render/Include/Texture.h>
#include <RootEngine/Render/Include/Effect.h>

#include <memory>

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
		void AddShadowcaster(const Render::Shadowcaster& p_shadowcaster, int p_index);
		void Process();

		GLuint GetFramebuffer(){ return m_framebuffer; }

		std::shared_ptr<TechniqueInterface> m_technique;
		Shadowcaster m_shadowcasters[RENDER_MAX_SHADOWCASTERS];
	private:
		size_t m_numberOfShadowcasters;
		Texture* m_depthTexture;
		GLuint m_framebuffer;
	};
}
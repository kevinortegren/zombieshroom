#pragma once

#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	class RenderToTexture
	{
	public:
		RenderToTexture();
		~RenderToTexture();

		void BindTexture();
		void BindTextureAsDepth();

		GLuint m_framebuffer;
		Render::TextureInterface* m_texture;
	};

}
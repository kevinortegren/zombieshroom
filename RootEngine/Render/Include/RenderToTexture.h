#pragma once

#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	struct RenderToTextureInterface
	{
		virtual void BindTexture() = 0;
		virtual void BindTextureAsDepth() = 0;
		virtual void Store(const char* p_filename) = 0;
		virtual void SetTexture(TextureInterface* p_texture) = 0;
		virtual TextureInterface* GetTexture() = 0;
		virtual GLuint GetFramebuffer() = 0;
	};

	class RenderToTexture : public RenderToTextureInterface
	{
	public:
		RenderToTexture();
		~RenderToTexture();

		void BindTexture();
		void BindTextureAsDepth();

		void Store(const char* p_filename);
		void SetTexture(TextureInterface* p_texture);
		TextureInterface* GetTexture();
		GLuint GetFramebuffer();

		GLuint m_framebuffer;
		Render::TextureInterface* m_texture;
	};

}
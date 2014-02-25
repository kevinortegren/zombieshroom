#pragma once

#include <GL/glew.h>
#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	class GLRenderer;

	struct GeometryBuffer
	{
		~GeometryBuffer();

		void Init(GLRenderer* p_renderer, int p_width, int p_height);
		void CreateTextures(int p_width, int p_height);
		
		void Clear(GLbitfield p_mask);

		void Enable();
		
		void BindTextures();
		void UnbindTextures();
		void Resize(int p_width, int p_height);

		GLuint m_fbo;

		Render::TextureInterface* m_depthTexture; // 32bit Depth.
		Render::TextureInterface* m_diffuseTexture; // 24bit Diffuse 8bit Specular;
		Render::TextureInterface* m_normalsTexture; // 16bit normals-xy.
		Render::TextureInterface* m_glowTexture; // 24bit Glow RGB 8bit Translucency.
		Render::TextureInterface* m_backgroundTexture; // 24bit background RGB 8bit Ambient.

	};
}
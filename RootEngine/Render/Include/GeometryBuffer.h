#pragma once

#include <GL/glew.h>
#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	class GLRenderer;

	class GeometryBuffer
	{
	friend class GLRenderer;
	public:
		~GeometryBuffer();

		void Init(GLRenderer* p_renderer, int p_width, int p_height);
		void CreateBuffers(int p_width, int p_height);
		void Bind();
		void Unbind();
		void Read();
		void Resize(int p_width, int p_height);
	private:

		GLuint m_fbo;

		Render::TextureInterface* m_depthTexture; // 32bit Depth.
		Render::TextureInterface* m_diffuseTexture; // 24bit Diffuse 8bit Specular;
		Render::TextureInterface* m_normalsTexture; // 24bit ViewSpaceNormals 8bit Empty.
		Render::TextureInterface* m_glowTexture; // 24bit Glow RGB 8bit Empty.
	};
}
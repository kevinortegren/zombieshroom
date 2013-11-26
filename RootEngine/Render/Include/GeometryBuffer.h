#pragma once

#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	class GeometryBuffer
	{
	friend class GLRenderer;
	public:
		~GeometryBuffer();
		void Init(int p_width, int p_height);
		void CreateBuffers(int p_width, int p_height);
		void Bind();
		void Unbind();
		void Read();
		void Resize(int p_width, int p_height);
		
	private:
		GLuint m_fbo;
		GLuint m_rbo;

		Render::Texture m_diffuse; // 24bit Color 8bit Empty
		Render::Texture m_normals; // 24bit ViewSpaceNormals 8bit Empty.
	};
}
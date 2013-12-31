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

		GLuint m_depthHandle;	// 32bit Depth.
		GLuint m_diffuseHandle; // 24bit Diffuse 8bit Specular;
		GLuint m_normalsHandle;// 24bit ViewSpaceNormals 8bit Empty.
		GLuint m_backgroundHandle;
	};
}
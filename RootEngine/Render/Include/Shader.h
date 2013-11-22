#pragma once
//#include <Windows.h>
#include <gl\GL.h>

namespace Render
{
	class Shader
	{
	public:
		Shader( );
		~Shader( );

		GLint LoadFromFile( GLenum p_shaderType, const char* p_filename );

		GLuint GetHandle() { return m_glHandle; }

	private:
		GLuint m_glHandle;
	};
}

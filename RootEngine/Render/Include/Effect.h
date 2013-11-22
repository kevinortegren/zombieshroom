#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <glm/glm.hpp>

namespace Render
{
	class Effect
	{
	public:
		Effect( );
		~Effect( );

		void CreateEffect();

		GLint AttachShader( GLenum p_shaderType, const char* p_filename );

		GLint Compile( );
		void Apply( );
	
		void SetUniformInt( const char* p_varname, int p_val );
		void SetUniformFloat( const char* p_varname, float p_val );
		void SetUniformVector( const char* p_varname, glm::vec3& p_val );
		void SetUniformVector( const char* p_varname, glm::vec4& p_val );
		void SetUniformMatrix( const char* p_varname, glm::mat3& p_val );
		void SetUniformMatrix( const char* p_varname, glm::mat4& p_val );

	private:
		GLuint m_glHandle;
	};
}

#include <GL/glew.h>
#include "Effect.h"
#include "Shader.h"
#include <RootEngine/Render/Include/RenderExtern.h>
#include <cstdio>

namespace Render
{
	Effect::Effect(void)
	{
	}

	Effect::~Effect(void)
	{
		if( m_glHandle != 0 )
			glDeleteProgram( m_glHandle );
	}

	void Effect::CreateEffect()
	{
		m_glHandle = glCreateProgram();
	}

	GLint Effect::AttachShader( GLenum p_shaderType, const char* p_filename )
	{
		Shader shader;
		GLint status = shader.LoadFromFile( p_shaderType, p_filename );
		if( status != GL_TRUE )
			return status;

		glAttachShader( m_glHandle, shader.GetHandle() );
		return GL_TRUE;
	}

	GLint Effect::Compile( )
	{
		glUseProgram( 0 );
		// link program
		glLinkProgram( m_glHandle );

		// verify link status
		GLint status;
		glGetProgramiv( m_glHandle, GL_LINK_STATUS, &status );

		if( status != GL_TRUE )
		{
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Failed to link shader programme[%s, line %d]", __FUNCTION__, __LINE__ );
			int length = 200;
			//glGetShaderiv( handle, GL_INFO_LOG_LENGTH, &length );
			if( length > 0 )
			{
				// create a log of error messages
				char* errorLog = new char[ length ];
				int written = 0;
				glGetProgramInfoLog( m_glHandle, length, &written, errorLog );
				Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Shader error log:\n\t\t%s", errorLog , __FUNCTION__, __LINE__ );
				delete[ ] errorLog;
			}
			return status;
		}
		return GL_TRUE;
	}

	void Effect::Apply( )
	{
		glUseProgram( m_glHandle );
	}

	GLint GetLocation( GLuint p_handle, const char* p_name );

	void Effect::SetUniformInt( const char* _name, int _val )
	{
		glUniform1i( GetLocation( m_glHandle, _name ), _val );
	}
	void Effect::SetUniformFloat( const char* _name, float _val )
	{
		glUniform1f( GetLocation( m_glHandle, _name ), _val );
	}
	void Effect::SetUniformVector( const char* _name, glm::vec3& _val )
	{
		glUniform3fv( GetLocation( m_glHandle, _name ), 1, &_val[0] );
	}
	void Effect::SetUniformVector( const char* _name, glm::vec4& _val )
	{
		glUniform4fv( GetLocation( m_glHandle, _name ), 1, &_val[0] );
	}
	void Effect::SetUniformMatrix( const char* _name, glm::mat3& _val )
	{
		glUniformMatrix3fv( GetLocation( m_glHandle, _name ), 1, GL_FALSE, &_val[0][0] );
	}
	void Effect::SetUniformMatrix( const char* _name, const glm::mat4& _val )
	{
		glUniformMatrix4fv( GetLocation( m_glHandle, _name ), 1, GL_FALSE, &_val[0][0] );
	}

	void Effect::SetUniformBuffer(GLuint p_bufferId, const std::string& bufferName, unsigned int slot) {

		GLint uniformBlockIndex = glGetUniformBlockIndex(m_glHandle, bufferName.c_str());

		if (uniformBlockIndex != -1) {
			glUniformBlockBinding(m_glHandle, uniformBlockIndex, slot);
			glBindBufferBase(GL_UNIFORM_BUFFER, slot, p_bufferId);
		}
	}
	void Effect::SetTexture(GLuint p_textureHandle, const std::string& textureName, unsigned int slot)
	{
		GLint uniformLocation = glGetUniformLocation(m_glHandle, textureName.c_str());
		glUniform1i(uniformLocation, slot);
	}

	GLint GetLocation( GLuint p_handle, const char* p_name )
	{
		GLint loc = glGetUniformLocation( p_handle, p_name );
		if( loc == 0 )
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::NON_FATAL_ERROR, "Failed to locate GL variable %s.\n", p_name , __FUNCTION__, __LINE__ );
		return loc;
	}

	void Load()
	{


	}
}

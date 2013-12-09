
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/RenderExtern.h>
#include <cstdio>
#include <fstream>
#include <iostream>

namespace Render
{
	Shader::Shader( )
	{
		m_glHandle = 0;
	}

	Shader::~Shader( )
	{
		if( m_glHandle != 0 )
			glDeleteShader( m_glHandle );
	}

	GLint Shader::LoadFromFile( GLenum p_shaderType, const char* p_filename )
	{
		m_glHandle = glCreateShader( p_shaderType );

		// validate creation
		if( m_glHandle == 0 )
		{
			//Render::g_context.m_logger.LogText(LogTag::RENDER, 3, "Creating shader type %d", p_shaderType);
			return GL_FALSE;
		}

		std::ifstream shaderFile( p_filename );
	
		if( !shaderFile.is_open( ) )
		{
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "ERROR creating opening shader file %s ! [%s, line %d]", p_filename, __FUNCTION__, __LINE__);
			return GL_FALSE;
		}

		// Get the size of the file
		shaderFile.seekg(0,std::ios::end);
		std::streampos length = shaderFile.tellg();
		shaderFile.seekg(0,std::ios::beg);

		// Use a vector as the buffer.
		//std::vector< char > buffer( (unsigned int)length );
		std::string buffer; buffer.resize((unsigned int)length);
		shaderFile.read( &buffer[ 0 ], length );

		// load source from a char array
		const char* ptr = buffer.c_str(); // get character pointer
		glShaderSource( m_glHandle, 1, &ptr, NULL );

		// compile shader
		glCompileShader( m_glHandle );

		// Check for errors
		int result = 0;
		glGetShaderiv( m_glHandle, GL_COMPILE_STATUS, &result );
		if( result != GL_TRUE )
		{
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Compiling shader %s ! [%s, line %d]", p_filename, __FUNCTION__, __LINE__ );
			int length = 0;
			glGetShaderiv( m_glHandle, GL_INFO_LOG_LENGTH, &length );
			if( length > 0 )
			{
				// create a log of error messages
				char* errorLog = new char[ length ];
				int written = 0;
				glGetShaderInfoLog( m_glHandle, length, &written, errorLog );
				Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::DEBUG_PRINT, "Shader error log;\n%s", errorLog  );
				delete[ ] errorLog;
			}
			return result;
		}

		return GL_TRUE;
	}

	Program::Program(void)
	{

	}

	Program::~Program(void)
	{
		if( m_glHandle != 0 )
			glDeleteProgram( m_glHandle );
	}

	void Program::CreateProgram()
	{
		m_glHandle = glCreateProgram();
	}

	GLint Program::AttachShader( GLenum p_shaderType, const char* p_filename )
	{
		Shader shader;
		GLint status = shader.LoadFromFile( p_shaderType, p_filename );
		if( status != GL_TRUE )
			return status;

		glAttachShader( m_glHandle, shader.GetHandle() );
		return GL_TRUE;
	}

	GLint Program::Compile( )
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

	void Program::Apply( )
	{
		glUseProgram( m_glHandle );
	}

	void Program::BindUniformBuffer(const std::string& bufferName, unsigned int slot) {

		GLint uniformBlockIndex = glGetUniformBlockIndex(m_glHandle, bufferName.c_str());

		if (uniformBlockIndex != -1) {
			glUniformBlockBinding(m_glHandle, uniformBlockIndex, slot);
		}
	}
	void Program::BindTexture(const std::string& textureName, unsigned int slot)
	{
		GLint uniformLocation = glGetUniformLocation(m_glHandle, textureName.c_str());
		glUniform1i(uniformLocation, slot);
	}

	std::shared_ptr<Program> Technique::CreateProgram()
	{
		auto program = std::shared_ptr<Program>(new Program);
		m_program.push_back(program);
		return program;
	}

	std::vector<std::shared_ptr<Program>>& Technique::GetPrograms()
	{
		return m_program;
	}

	void Technique::BindUniforms()
	{
		for(auto itr = m_uniforms.begin(); itr != m_uniforms.end(); ++itr)
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, (*itr).first, (*itr).second->GetBufferId());
		}
	}

	void Technique::BindTextures()
	{
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			glActiveTexture(GL_TEXTURE0 + (*itr).first);
			glBindTexture(GL_TEXTURE_2D, (*itr).second);
		}
	}

	std::shared_ptr<Technique> Effect::CreateTechnique()
	{
		auto technique = std::shared_ptr<Technique>(new Technique);
		m_techniques.push_back(technique);
		return technique;
	}

	std::vector<std::shared_ptr<Technique>>& Effect::GetTechniques()
	{
		return m_techniques;
	}

	void EffectParams::AllocateParams(EffectInterface* p_effect)
	{
		for(auto itr = p_effect->GetTechniques().begin(); itr != p_effect->GetTechniques().end(); ++itr)
		{
			for(auto itrD = (*itr)->m_data.begin(); itrD != (*itr)->m_data.end(); ++itrD)
			{
				//std::cout << "SEM: " << (*itrD).first << " Size: " << (*itrD).second << std::endl;
			}
		}
	}

	/*GLint GetLocation( GLuint p_handle, const char* p_name );

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

	GLint GetLocation( GLuint p_handle, const char* p_name )
	{
		GLint loc = glGetUniformLocation( p_handle, p_name );
		if( loc == -1 )
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::NON_FATAL_ERROR, "Failed to locate GL variable %s.\n", p_name , __FUNCTION__, __LINE__ );
		return loc;
	} */

}

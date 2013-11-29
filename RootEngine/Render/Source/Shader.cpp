#include <GL/glew.h>

#include <RootEngine/Render/Include/RenderExtern.h>
#include <fstream>
#include <vector>

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
			Render::g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Creating shader type %d", p_shaderType);
			return GL_FALSE;
		}

		std::ifstream shaderFile( p_filename );
	
		if( !shaderFile.is_open( ) )
		{
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "ERROR creating opening shader file %s ", p_filename);
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
			Render::g_context.m_logger->LogText(LogTag::RENDER,  LogLevel::FATAL_ERROR, "Compiling shader type %d ", p_shaderType);
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

}

#include <GL/glew.h>

#include "Shader.h"

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
			printf( "ERROR creating shader type %d\n", p_shaderType );
			return GL_FALSE;
		}

		std::ifstream shaderFile( p_filename );
	
		if( !shaderFile.is_open( ) )
		{
			printf( "ERROR creating opening shader file %s\n", p_filename );
			return GL_FALSE;
		}

		// Get the size of the file
		shaderFile.seekg(0, std::ios::end);
		std::streampos length = shaderFile.tellg();
		shaderFile.seekg(0, std::ios::beg);

		// Use a vector as the buffer.
		std::vector< char > buffer( (unsigned int)length );
		shaderFile.read( &buffer[ 0 ], length );

		// load source from a char array
		const char* ptr = buffer.data(); // get character pointer
		glShaderSource( m_glHandle, 1, &ptr, NULL );

		// compile shader
		glCompileShader( m_glHandle );

		// Check for errors
		int result = 0;
		glGetShaderiv( m_glHandle, GL_COMPILE_STATUS, &result );
		if( result != GL_TRUE )
		{
			printf( "ERROR compiling shader type %d\n", p_shaderType );
			int length = 0;
			glGetShaderiv( m_glHandle, GL_INFO_LOG_LENGTH, &length );
			if( length > 0 )
			{
				// create a log of error messages
				char* errorLog = new char[ length ];
				int written = 0;
				glGetShaderInfoLog( m_glHandle, length, &written, errorLog );
				printf( "Shader error log;\n%s\n", errorLog );
				delete[ ] errorLog;
			}
			return result;
		}

		return GL_TRUE;
	}
}
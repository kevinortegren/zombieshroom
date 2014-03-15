
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Render/Include/Renderer.h>
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
		m_blendState = BLEND_NONE;
		m_depthState.depthTest = true;
		m_depthState.depthWrite = true;
		m_viewport = glm::ivec4(0, 0, 1280, 720);

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

	GLuint Program::GetHandle()
	{
		return m_glHandle;
	}

	void Program::Apply( )
	{
		// Apply blending.
		switch(m_blendState)
		{
			case BLEND_NONE:
				glDisable(GL_BLEND);
				break;
			case BLEND_ALPHA:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_ADDITIVE:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BLEND_INV_ALPHA:
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
				break;
			default:
				glDisable(GL_BLEND);
				break;
		}

		// Set Depth modes.
		if(m_depthState.depthWrite)
		{
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}

		if(m_depthState.depthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		// Use program.
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

	Technique::Technique()
		: m_flags(0)
	{

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

	void Technique::Apply()
	{
		// Per technique uniform binding.
		glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_perTechniqueBuffer->GetBufferId());
	}

	void Technique::AddUniformParam(Semantic::Semantic p_sem, unsigned int p_offset)
	{
		m_uniformsParams[p_sem] = p_offset;
	}

	BufferInterface* Technique::GetBufferInterface()
	{
		return m_perTechniqueBuffer;
	}

	Technique::~Technique()
	{

	}

	std::shared_ptr<Technique> Effect::CreateTechnique(RendererInterface* p_renderer)
	{
		auto technique = std::shared_ptr<Technique>(new Technique);

		technique->m_perTechniqueBuffer = p_renderer->CreateBuffer(GL_UNIFORM_BUFFER);

		m_techniques.push_back(technique);

		return technique;
	}

	std::vector<std::shared_ptr<Technique>>& Effect::GetTechniques()
	{
		return m_techniques;
	}

	Effect::Effect()
	{

	}

	Effect::~Effect()
	{

	}

}

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

namespace Render
{
	class Shader
	{
	public:
		Shader();
		~Shader();
		GLint LoadFromFile( GLenum p_shaderType, const char* p_filename );
		GLuint GetHandle() { return m_glHandle; }
	private:
		GLuint m_glHandle;
	};

	class ProgramInterface
	{
	public:
		virtual void CreateProgram() = 0;
		virtual GLint AttachShader( GLenum p_shaderType, const char* p_filename ) = 0;
		virtual GLint Compile( ) = 0;
		virtual void Apply( ) = 0;
	
		virtual void BindUniformBuffer(const std::string& bufferName, unsigned int slot) = 0;
		virtual void BindTexture(const std::string& textureName, unsigned int slot) = 0;
	};

	class Program : public ProgramInterface
	{
	public:
		Program();
		~Program();
		void CreateProgram();

		GLint AttachShader( GLenum p_shaderType, const char* p_filename );
		GLint Compile();

		void Apply();

		void BindUniformBuffer(const std::string& bufferName, unsigned int slot);
		void BindTexture(const std::string& textureName, unsigned int slot);

	private:
		GLuint m_glHandle;
	};

	class TechniqueInterface
	{
	public:
		virtual std::shared_ptr<Program> CreateProgram() = 0;
		virtual std::vector<std::shared_ptr<Program>>& GetPrograms() = 0;	
	};

	class Technique : public TechniqueInterface
	{
	public:
		//TODO: Move uniforms/samplers here.

		std::shared_ptr<Program> CreateProgram();
		std::vector<std::shared_ptr<Program>>& GetPrograms();

	private:
		std::vector<std::shared_ptr<Program>> m_program;
	};

	class EffectInterface
	{
	public:
		virtual std::shared_ptr<Technique> CreateTechnique() = 0;
		virtual std::vector<std::shared_ptr<Technique>>& GetTechniques() = 0;
	};

	class Effect : public EffectInterface
	{
	public:
		std::shared_ptr<Technique> CreateTechnique();
		std::vector<std::shared_ptr<Technique>>& GetTechniques();

	private:
		std::vector<std::shared_ptr<Technique>> m_techniques;
	};

	/*void SetUniformInt( const char* p_varname, int p_val );
	void SetUniformFloat( const char* p_varname, float p_val );
	void SetUniformVector( const char* p_varname, glm::vec3& p_val );
	void SetUniformVector( const char* p_varname, glm::vec4& p_val );
	void SetUniformMatrix( const char* p_varname, glm::mat3& p_val );
	void SetUniformMatrix( const char* p_varname, const glm::mat4& p_val );*/
}

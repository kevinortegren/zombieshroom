#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <RootEngine\Render\Include\Semantics.h>
#include <RootEngine\Render\Include\Buffer.h>

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
		virtual GLint Compile() = 0;
		virtual void Apply() = 0;
	
		virtual void BindUniformBuffer(const std::string& bufferName, unsigned int slot) = 0;
		virtual void BindTexture(const std::string& textureName, unsigned int slot) = 0;
	};

	class Program : public ProgramInterface
	{
	public:

		enum BlendState { BLEND_NONE, BLEND_ALPHA, BLEND_ADDITIVE };

		enum FillMode { FILL_SOLID, FILL_WIREFRAME };

		struct DepthState {		
			bool depthTest;
			bool depthWrite;
		};

		Program();
		~Program();
		void CreateProgram();

		GLint AttachShader( GLenum p_shaderType, const char* p_filename );
		GLint Compile();

		void Apply();

		void BindUniformBuffer(const std::string& bufferName, unsigned int slot);
		void BindTexture(const std::string& textureName, unsigned int slot);

		BlendState m_blendState;
		DepthState m_depthState;
		FillMode m_fillMode;

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
		std::shared_ptr<Program> CreateProgram();
		std::vector<std::shared_ptr<Program>>& GetPrograms();

		std::vector<std::pair<int, std::shared_ptr<Render::BufferInterface>>> m_uniforms;
		std::vector<std::pair<int, GLuint>> m_textures;
		std::map<Semantic::Semantic, unsigned int> m_data; // offsets.
		
		void BindUniforms();
		void BindTextures();

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

	struct EffectParamsInterface
	{
		virtual void AllocateParams(EffectInterface* p_effect) = 0;
	};

	struct EffectParams : public EffectParamsInterface
	{
		void AllocateParams(EffectInterface* p_effect);
	};
}

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
	class RendererInterface;

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
		virtual GLuint GetHandle() = 0;
		virtual void BindUniformBuffer(const std::string& bufferName, unsigned int slot) = 0;
		virtual void BindTexture(const std::string& textureName, unsigned int slot) = 0;
	};

	//TODO: Store in resource manager.
	class Program : public ProgramInterface
	{
	public:

		enum BlendState { BLEND_NONE, BLEND_ALPHA, BLEND_ADDITIVE, BLEND_INV_ALPHA };
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
		GLuint GetHandle();

		void BindUniformBuffer(const std::string& bufferName, unsigned int slot);
		void BindTexture(const std::string& textureName, unsigned int slot);

		BlendState m_blendState;
		DepthState m_depthState;
		FillMode m_fillMode;
		glm::ivec4 m_viewport;

	private:
		GLuint m_glHandle;
	};

	namespace TechniqueFlags
	{
		enum TechniqueFlags
		{
			RENDER_IGNORE = 0x01,
			RENDER_DEFERRED1 = 0x02,
			RENDER_DEFERRED0 = 0x04,
			RENDER_DEBUG = 0x08,
			RENDER_SHADOW = 0x10,
			RENDER_RTT = 0x20
		};
	}

	class TechniqueInterface
	{
	public:
		virtual std::shared_ptr<Program> CreateProgram() = 0;
		virtual std::vector<std::shared_ptr<Program>>& GetPrograms() = 0;
		virtual void AddUniformParam(Semantic::Semantic p_sem, unsigned int p_offset) = 0;
		virtual void Apply() = 0;
	};

	class Technique : public TechniqueInterface
	{
	public:
		Technique();
		std::shared_ptr<Program> CreateProgram();
		std::vector<std::shared_ptr<Program>>& GetPrograms();

		void Apply();
		void AddUniformParam(Semantic::Semantic p_sem, unsigned int p_offset);

		std::map<Semantic::Semantic, unsigned int> m_uniformsParams;
		BufferInterface* m_perTechniqueBuffer;

		unsigned m_flags;

	private:
		std::vector<std::shared_ptr<Program>> m_program;		
	};

	class EffectInterface
	{
	public:
		virtual std::shared_ptr<Technique> CreateTechnique(RendererInterface* p_renderer) = 0;
		virtual std::vector<std::shared_ptr<Technique>>& GetTechniques() = 0;
	};

	class Effect : public EffectInterface
	{
	public:
		std::shared_ptr<Technique> CreateTechnique(RendererInterface* p_renderer);
		std::vector<std::shared_ptr<Technique>>& GetTechniques();

	private:
		std::vector<std::shared_ptr<Technique>> m_techniques;
	};
}

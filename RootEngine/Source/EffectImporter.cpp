#include <RootEngine/Include/EffectImporter.h>
#include <RootEngine/Render/Include/Renderer.h>

#include <RootEngine/Include/Logging/Logging.h>
extern Logging	g_logger;

namespace RootEngine
{
	EffectImporter::EffectImporter( Render::RendererInterface* p_renderer ) : m_renderer(p_renderer)
	{
		//g_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Effect importer initialized!");
	}

	void EffectImporter::Process(const YAML::Node& p_node)
	{
		Render::EffectInterface* effect = m_renderer->CreateEffect();

		std::string techName;

		// Parsing technique.
		p_node["name"] >> techName;

		const YAML::Node& techniques = p_node["techniques"];
		for(size_t i = 0; i < techniques.size(); ++i)
		{
			std::shared_ptr<Render::Technique> technique = effect->CreateTechnique();

			std::string techniqueName;
			techniques[i]["name"] >> techniqueName;

			const YAML::Node& programs = techniques[i]["programs"];
			for(size_t j = 0; j < programs.size(); ++j)
			{
				std::shared_ptr<Render::Program> program = technique->CreateProgram();

				program->CreateProgram();

				const YAML::Node& shaders = programs[j]["shaders"];
				for(size_t k = 0; k < shaders.size(); ++k)
				{	
					std::string shaderName;
					shaders[k]["name"] >> shaderName;
					
					std::string type;
					shaders[k]["type"] >> type;
			
					GLenum glType;
					std::string extension;
					if(type == "vertex") 
					{
						glType = GL_VERTEX_SHADER;
						extension = ".vert";
					}
					else if(type == "fragment")
					{
						glType = GL_FRAGMENT_SHADER;
						extension = ".frag";
					}

					std::string shader = std::string(m_workingDirectory + "Assets//Shaders//" + shaderName  + extension);
					program->AttachShader(glType, shader.c_str());

				}
				program->Compile();
				program->Apply();

				const YAML::Node& uniforms = techniques[i]["uniforms"];
				for(size_t j = 0; j < uniforms.size(); ++j)
				{
					std::string name;
					uniforms[j]["name"] >> name;
					
					int slot;
					uniforms[j]["slot"] >> slot;

					program->BindUniformBuffer(name, slot);

				}

				const YAML::Node& texture = techniques[i]["textures"];
				for(size_t j = 0; j < texture.size(); ++j)
				{
					std::string name;
					texture[j]["name"] >> name;
					
					int slot;
					texture[j]["slot"] >> slot;

					program->BindTexture(name, slot);

				}
			}
		}

		m_effect = effect;
	}

	void EffectImporter::SetWorkingDirectory(std::string p_workingDirectory)
	{
		m_workingDirectory = p_workingDirectory;
	}

	

}
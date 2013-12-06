#include <RootEngine/Include/EffectImporter.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Semantics.h>

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
		std::shared_ptr<Render::EffectInterface> effect = m_renderer->CreateEffect();

		std::string techName;

		// Parsing technique.
		p_node["name"] >> techName;

		const YAML::Node& techniques = p_node["techniques"];
		for(size_t i = 0; i < techniques.size(); ++i)
		{
			std::shared_ptr<Render::Technique> technique = effect->CreateTechnique();

			std::string techniqueName;
			techniques[i]["name"] >> techniqueName;

			// Create programs and link shaders.
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



				if(programs[i].FindValue("blend"))
				{
					int blendType;
					programs[i]["blend"] >> blendType;
					program->m_blendState = (Render::Program::BlendState)blendType;
				}

				
				if(programs[i].FindValue("depth"))
				{
					int depthWrite;
					programs[i]["depth"]["write"] >> depthWrite;			
					program->m_depthState.depthWrite = (bool)depthWrite;
					
					int depthTest;
					programs[i]["depth"]["test"] >> depthTest;		
					program->m_depthState.depthTest = (bool)depthTest;
				}

				program->Compile();
				program->Apply();

				// Create uniform buffer for this technique.
				const YAML::Node& uniforms = techniques[i]["uniforms"];
				for(size_t j = 0; j < uniforms.size(); ++j)
				{
					std::string name;
					uniforms[j]["name"] >> name;
					
					int slot;
					uniforms[j]["slot"] >> slot;

					// Bind shader name to slot.
					program->BindUniformBuffer(name, slot);

					// Create the buffer.
					std::pair<int, std::shared_ptr<Render::BufferInterface>> bufferPair;

					std::shared_ptr<Render::BufferInterface> buffer = m_renderer->CreateBuffer();
					buffer->Init(GL_UNIFORM_BUFFER);

					bufferPair.first = slot;
					bufferPair.second = buffer;

					technique->m_uniforms.push_back(bufferPair);
				}
		
				if(techniques[i].FindValue("PerObject") != nullptr)
				{
					const YAML::Node& perObjects = techniques[i]["PerObject"];
					for(size_t k = 0; k < perObjects.size(); ++k)
					{
						std::string sem;
						perObjects[k]["sem"] >> sem;
					
						std::string type;
						perObjects[k]["type"] >> type;

						// Test.

						// Allocate based on type.
						unsigned int size = sizeof(glm::mat4);

						// Store the space based on semantic.
						technique->m_data[Render::Semantic::WORLD] = size;
					}
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
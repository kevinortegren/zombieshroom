#include <RootEngine/Include/EffectImporter.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/Semantics.h>
#include <functional>

#include <RootEngine/Include/Logging/Logging.h>
extern Logging g_logger;

namespace RootEngine
{
	EffectImporter::EffectImporter( Render::RendererInterface* p_renderer ) : m_renderer(p_renderer)
	{
		//g_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Effect importer initialized!");
	}

	void EffectImporter::Process(const YAML::Node& p_node)
	{
		Render::EffectInterface* effect = m_renderer->CreateEffect();

		if(!p_node.FindValue("techniques"))
		{
			//TODO: Log error.
			return;
		}

		const YAML::Node& techniques = p_node["techniques"];
		for(size_t i = 0; i < techniques.size(); ++i)
		{
			std::shared_ptr<Render::Technique> technique = effect->CreateTechnique(m_renderer);

			if(!techniques[i].FindValue("programs"))
			{
				//TODO: Log error.
				return;
			}

			// Create programs and link shaders.
			const YAML::Node& programs = techniques[i]["programs"];
			for(size_t j = 0; j < programs.size(); ++j)
			{
				//TODO: Use the resource manager to share programs between effects.
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
					else if(type == "geometry")
					{
						glType = GL_GEOMETRY_SHADER;
						extension = ".geometry";
					}
					else if(type == "compute")
					{
#ifdef RENDER_USE_COMPUTE
						glType = GL_COMPUTE_SHADER;
						extension = ".compute";
#else
						return;
#endif
					}
					else if(type == "tess_evaluation")
					{
						glType = GL_TESS_EVALUATION_SHADER;
						extension = ".tesseval";
					}
					else if(type == "tess_control")
					{
						glType = GL_TESS_CONTROL_SHADER;
						extension = ".tesscontrol";
					}

					std::string shader = std::string(m_workingDirectory + "Assets//Shaders//" + shaderName  + extension);
					program->AttachShader(glType, shader.c_str());
				}

				if(programs[j].FindValue("feedback"))
				{
					std::vector<std::string> varyingsVector;
					const YAML::Node& varyings = programs[j]["feedback"];
					for(unsigned k = 0; k < varyings.size(); k++)
					{
						std::string varying;
						varyings[k]["name"] >> varying;
						varyingsVector.push_back(varying);
					}
					std::vector<const char*> varingsChar;
					varingsChar.reserve(varyingsVector.size());
					std::transform(std::begin(varyingsVector), std::end(varyingsVector), 
						 std::back_inserter(varingsChar), std::mem_fn(&std::string::c_str));

					glTransformFeedbackVaryings(program->GetHandle(), varingsChar.size(), varingsChar.data(), GL_INTERLEAVED_ATTRIBS);
				}

				if(programs[j].FindValue("blend"))
				{
					int blendType;
					programs[j]["blend"] >> blendType;
					program->m_blendState = (Render::Program::BlendState)blendType;
				}
	
				if(programs[j].FindValue("depth"))
				{
					int depthWrite;
					programs[j]["depth"]["write"] >> depthWrite;			
					program->m_depthState.depthWrite = depthWrite != 0;
					
					int depthTest;
					programs[j]["depth"]["test"] >> depthTest;		
					program->m_depthState.depthTest = depthTest != 0;
				}

				if(programs[j].FindValue("viewport"))
				{
					int x;
					programs[j]["viewport"]["x"] >> x;			
					program->m_viewport.x = x;
	
					int y;
					programs[j]["viewport"]["y"] >> y;		
					program->m_viewport.y = y;

					int w;
					programs[j]["viewport"]["width"] >> w;			
					program->m_viewport.z = w;

					int h;
					programs[j]["viewport"]["height"] >> h;		
					program->m_viewport.w = h;

				}

				program->Compile();
				program->Apply();

				if(techniques[i].FindValue("bindings"))
				{
					const YAML::Node& bindings = techniques[i]["bindings"];
					for(size_t j = 0; j < bindings.size(); ++j)
					{
						std::string name;
						bindings[j]["name"] >> name;
					
						int slot;
						bindings[j]["slot"] >> slot;

						// Bind shader name to slot.
						program->BindUniformBuffer(name, slot);
					}
				}
		
				if(techniques[i].FindValue("images"))
				{
					const YAML::Node& images = techniques[i]["images"];
					for(size_t j = 0; j < images.size(); ++j)
					{
						std::string name;
						images[j]["name"] >> name;

						int slot;
						images[j]["slot"] >> slot;

						// Bind shader name to slot.
						//program->BindUniformBuffer(name, slot);

						glUniform1i(glGetUniformLocation(program->GetHandle(), name.c_str()), slot);
					}
				}
				
				if(techniques[i].FindValue("uniforms"))
				{
					const YAML::Node& uniforms = techniques[i]["uniforms"];
					for(size_t j = 0; j < uniforms.size(); ++j)
					{
						std::string sem;
						uniforms[j]["sem"] >> sem;
					
						int offset;
						uniforms[j]["offset"] >> offset;

						if(sem == "MODEL")
						{
							technique->AddUniformParam(Render::Semantic::MODEL, offset);
						}
						else if(sem == "NORMAL")
						{
							technique->AddUniformParam(Render::Semantic::NORMAL, offset);
						}
						else if(sem == "BONES")
						{
							technique->AddUniformParam(Render::Semantic::BONES, offset);
						}
						else if(sem == "POSITION")
						{
							technique->AddUniformParam(Render::Semantic::POSITION, offset);
						}
						else if(sem == "LIFETIMEMIN")
						{
							technique->AddUniformParam(Render::Semantic::LIFETIMEMIN, offset);
						}
						else if(sem == "LIFETIMEMAX")
						{
							technique->AddUniformParam(Render::Semantic::LIFETIMEMAX, offset);
						}
						else if(sem == "SPEEDMIN")
						{
							technique->AddUniformParam(Render::Semantic::SPEEDMIN, offset);
						}
						else if(sem == "SPEEDMAX")
						{
							technique->AddUniformParam(Render::Semantic::SPEEDMAX, offset);
						}
						else if(sem == "SIZEMIN")
						{
							technique->AddUniformParam(Render::Semantic::SIZEMIN, offset);
						}
						else if(sem == "SIZEMAX")
						{
							technique->AddUniformParam(Render::Semantic::SIZEMAX, offset);
						}
						else if(sem == "SIZEEND")
						{
							technique->AddUniformParam(Render::Semantic::SIZEEND, offset);
						}
						else if(sem == "COLOR")
						{
							technique->AddUniformParam(Render::Semantic::COLOR, offset);
						}
						else if(sem == "COLOREND")
						{
							technique->AddUniformParam(Render::Semantic::COLOREND, offset);
						}
						else if(sem == "GRAVITY")
						{
							technique->AddUniformParam(Render::Semantic::GRAVITY, offset);
						}
						else if(sem == "DIRECTION")
						{
							technique->AddUniformParam(Render::Semantic::DIRECTION, offset);
						}
						else if(sem == "SPREAD")
						{
							technique->AddUniformParam(Render::Semantic::SPREAD, offset);
						}
						else if(sem == "SPAWNTIME")
						{
							technique->AddUniformParam(Render::Semantic::SPAWNTIME, offset);
						}
						else if(sem == "TRANSPOSITION")
						{
							technique->AddUniformParam(Render::Semantic::TRANSPOSITION, offset);
						}
						else if(sem == "ORBITSPEED")
						{
							technique->AddUniformParam(Render::Semantic::ORBITSPEED, offset);
						}
						else if(sem == "ORBITRADIUS")
						{
							technique->AddUniformParam(Render::Semantic::ORBITRADIUS, offset);
						}
						else if(sem == "MK1")
						{
							technique->AddUniformParam(Render::Semantic::MK1, offset);
						}
						else if(sem == "MK2")
						{
							technique->AddUniformParam(Render::Semantic::MK2, offset);
						}
						else if(sem == "MK3")
						{
							technique->AddUniformParam(Render::Semantic::MK3, offset);
						}
						else if(sem == "XMAX")
						{
							technique->AddUniformParam(Render::Semantic::XMAX, offset);
						}
						else if(sem == "YMAX")
						{
							technique->AddUniformParam(Render::Semantic::YMAX, offset);
						}
						else if(sem == "EYEWORLDPOS")
						{
							technique->AddUniformParam(Render::Semantic::EYEWORLDPOS, offset);
						}
						else if(sem == "DX")
						{
							technique->AddUniformParam(Render::Semantic::DX, offset);
						}
						else if(sem == "ROTATIONSPEEDMIN")
						{
							technique->AddUniformParam(Render::Semantic::ROTATIONSPEEDMIN, offset);
						}
						else if(sem == "ROTATIONSPEEDMAX")
						{
							technique->AddUniformParam(Render::Semantic::ROTATIONSPEEDMAX, offset);
						}
						else if(sem == "MAXPERFRAME")
						{
							technique->AddUniformParam(Render::Semantic::MAXPERFRAME, offset);
						}
						else if(sem == "ALIVE")
						{
							technique->AddUniformParam(Render::Semantic::ALIVE, offset);
						}
					}
				}

				if(techniques[i].FindValue("textures"))
				{
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

			if(techniques[i].FindValue("flags") != nullptr)
			{
				const YAML::Node& flags = techniques[i]["flags"];
				for(size_t j = 0; j < flags.size(); ++j)
				{
					std::string name;
					flags[j]["name"] >> name;

					if(name == "RenderIgnore")
					{
						technique->m_flags |= Render::TechniqueFlags::RENDER_IGNORE;
					}
					else if(name == "Forward")
					{
						technique->m_flags |= Render::TechniqueFlags::RENDER_DEFERRED1;
					}
					else if(name == "Deferred")
					{
						technique->m_flags |= Render::TechniqueFlags::RENDER_DEFERRED0;
					}
					else if(name == "Debug")
					{
						technique->m_flags |= Render::TechniqueFlags::RENDER_DEBUG;
					}
					else if(name == "Shadow")
					{
						technique->m_flags |= Render::TechniqueFlags::RENDER_SHADOW;
					}
					if(name == "Refractive")
					{
						technique->m_flags |= Render::TechniqueFlags::REFRACTIVE;
					}
				}
			}
			else
			{
				// Assume deffered technique if no flags found.
				technique->m_flags = Render::TechniqueFlags::RENDER_DEFERRED0;
			}
		}

		m_effect = effect;
	}

	void EffectImporter::SetWorkingDirectory(std::string p_workingDirectory)
	{
		m_workingDirectory = p_workingDirectory;
	}

	

}
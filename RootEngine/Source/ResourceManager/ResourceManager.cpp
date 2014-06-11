#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Script/Include/RootScript.h>
#include <RootEngine/Sound/Include/SoundManager.h>
namespace RootEngine
{

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		// Clear models.
		auto modelitr = m_models.begin();
		for(; modelitr != m_models.end(); modelitr++)
		{
			delete (*modelitr).second;
			(*modelitr).second = nullptr;
		}
		m_models.clear();

		// Clear particles.
		for(auto partitr = m_particles.begin(); partitr != m_particles.end(); partitr++)
		{
			for(auto partvec = (*partitr).second.begin(); partvec != (*partitr).second.end(); partvec++)
			{
				delete (*partvec);
				(*partvec) = nullptr;
			}
			(*partitr).second.clear();
		}

		// Clear sounds.
		for(auto soundItr = m_soundAudios.begin(); soundItr != m_soundAudios.end(); soundItr++)
		{
			delete (*soundItr).second;
			(*soundItr).second = nullptr;
		}
		m_soundAudios.clear();
	}

	void ResourceManager::Init(std::string p_workingDirectory, GameSharedContext* p_context)
	{
		m_context = p_context;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(m_context->m_renderer));
#ifndef COMPILE_LEVEL_EDITOR
		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(p_context));
		m_modelExporter = std::shared_ptr<ModelExporter>(new ModelExporter(p_context));
#endif
		m_textureImporter = std::shared_ptr<TextureImporter>(new TextureImporter(m_context->m_logger, m_context->m_renderer));
		
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);

		m_particleImporter = std::shared_ptr<ParticleImporter>(new ParticleImporter(m_context));

		SetupDefaultResources();
	}

	//////////////////////////////////////////////////////////////////////////
	//Load functions
	//////////////////////////////////////////////////////////////////////////

#ifndef COMPILE_LEVEL_EDITOR
	Model* ResourceManager::LoadCollada(std::string p_path)
	{
		
		if(m_models.find(p_path) == m_models.end())
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::START_PRINT, "[MODEL] Starting to load: '%s'", p_path.c_str());
			Model* model = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + ".DAE", p_path, false);

			if(model)
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[MODEL] Loaded: '%s'", p_path.c_str());
				m_models[p_path] = model;
				return m_models[p_path];
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "[MODEL] NOT LOADED: '%s'", p_path.c_str());
				return nullptr;
			}
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Model already exists: %s", p_path.c_str());
			return m_models[p_path];
		}
		
	}

	Sound::SoundAudioInterface* ResourceManager::LoadSoundAudio( std::string p_name )
	{
		if(m_soundAudios.find(p_name) == m_soundAudios.end())
		{
			Sound::SoundAudioInterface* soundaudio = m_context->m_sound->CreateSoundAudio();
			if(soundaudio)
			{
				std::string filePath = m_workingDirectory + "Assets/Audio/" + p_name;
				if(soundaudio->LoadSound(filePath))
				{
					m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[SOUND] Loaded: '%s'", p_name.c_str());
					m_soundAudios[p_name] = soundaudio;
					return soundaudio;
				}
				else
				{
					m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error when loading sound: '%s'", p_name.c_str());
					delete soundaudio;
					return nullptr;
				}
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "Error when creating sound: '%s'", p_name.c_str());
				return nullptr;
			}
		}
		else
		{
			return m_soundAudios[p_name];
		}
		
	}

	std::string ResourceManager::LoadScript( std::string p_scriptName )
	{
		if(m_scripts.find(p_scriptName) == m_scripts.end())
		{
			if(m_context->m_script->LoadScript(p_scriptName + ".lua") == 1)
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::FATAL_ERROR, "[SCRIPT] Error when loading: '%s.lua'", p_scriptName.c_str());
				return "";
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[SCRIPT] Loaded: '%s.lua'", p_scriptName.c_str());
			}
			m_scripts[p_scriptName] = p_scriptName;
			
			return p_scriptName;
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Script already exists: %s.lua", p_scriptName.c_str());
			return p_scriptName;
		}
		
	}

	std::string ResourceManager::ForceLoadScript( std::string p_scriptName )
	{
		// Do not reload global variables.
		if (p_scriptName == "Global")
			return p_scriptName;
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Force loaded script: '%s.lua', it may already exist in Resource Manager!", p_scriptName.c_str());
		m_context->m_script->LoadScript(p_scriptName + ".lua");
		m_scripts[p_scriptName] = p_scriptName;
		return p_scriptName;
	}
#endif

	Render::EffectInterface* ResourceManager::LoadEffect(std::string p_path)
	{
		if(m_effects.find(p_path) == m_effects.end())
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Loading effect: %s", p_path.c_str());

			m_effectImporter->Load(m_workingDirectory + "Assets\\Effects\\" + p_path + ".effect");
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[EFFECT] Loaded: '%s'", p_path.c_str());
			m_effects[p_path] = m_effectImporter->m_effect;

			if(m_effectImporter->m_effect)
				return m_effects[p_path];
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Error loading effect '%s'", p_path.c_str());
				return nullptr;
			}
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Effect already exists: %s", p_path.c_str());
			return m_effects[p_path];
		}
	}

	Render::TextureInterface* ResourceManager::LoadTexture( std::string p_path, Render::TextureType::TextureType p_type )
	{
		if(m_textures.find(p_path) == m_textures.end())
		{
			Render::TextureInterface* tex;
			if(p_type == Render::TextureType::TEXTURE_2D)
			{
				tex = m_textureImporter->LoadTexture(m_workingDirectory + "Assets\\Textures\\" + p_path + ".dds");
			}
			else if(p_type == Render::TextureType::TEXTURE_CUBEMAP)
			{
				tex = m_textureImporter->LoadCubeTexture(m_workingDirectory + "Assets\\Textures\\" + p_path + ".dds");
			}

			if(tex)
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[TEXTURE] Loaded: '%s'", p_path.c_str());
				m_textures[p_path] = tex;
				return m_textures[p_path];
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "[TEXTURE] Error loading '%s'", p_path.c_str());
				return m_defaultTexture;
			}
		}
		else
		{
			return m_textures[p_path];
		}	
	}

	std::vector<ParticleSystemStruct*> ResourceManager::LoadParticleEmitter( std::string p_handle, bool p_fullPath )
	{
		std::vector<ParticleSystemStruct*> particleStruct;
		if(m_particles.find(p_handle) == m_particles.end())
		{
			if(p_fullPath)
				particleStruct = m_particleImporter->LoadParticleSystem(p_handle);
			else
				particleStruct = m_particleImporter->LoadParticleSystem(m_workingDirectory + "Assets\\Particles\\" + p_handle + ".particle");
			
			if(particleStruct.size() != 0)
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[PARTICLE] Loaded: '%s'", p_handle.c_str());
				m_particles[p_handle] = particleStruct;
				return particleStruct;
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Error loading particle '%s'", p_handle.c_str());
				return particleStruct;
			}
		}
		else
		{
			return m_particles[p_handle];
		}
	}

	Model* ResourceManager::CreateModel(const std::string& p_path)
	{
		if(m_models.find(p_path) == m_models.end())
		{
			Model* model = new Model();
			m_meshes[p_path + "0"] = m_context->m_renderer->CreateMesh();
			model->m_meshes[0] = m_meshes[p_path + "0"];
			model->m_meshes[1] = nullptr;

			if(model)
			{
				m_models[p_path] = model;
				return m_models.find(p_path)->second;
			}
			else
			{
				return nullptr;
			}
		}
		return m_models[p_path];
	}

	Render::TextureInterface* ResourceManager::CreateTexture(const std::string& p_path)
	{
		if(m_textures.find(p_path) == m_textures.end())
		{
			Render::TextureInterface* texture = m_context->m_renderer->CreateTexture();
	
			if(texture)
			{
				m_textures[p_path] = texture;
				return m_textures[p_path];
			}
			else
			{
				return nullptr;
			}
		}
		return m_textures[p_path];
	}

	bool ResourceManager::RenameModel(Model* p_model, const std::string& p_name)
	{
		// Look if new name dosent exist.
		
		std::string oldName = ResolveStringFromModel(p_model);

		// Remove the model from the old name.
		m_models[oldName] = nullptr;	

		// Set the model.
		m_models[p_name] = p_model;

		return false;
	}


	//////////////////////////////////////////////////////////////////////////
	//Get functions
	//////////////////////////////////////////////////////////////////////////
	Model* ResourceManager::GetModel( std::string p_handle )
	{
		if(m_models.find(p_handle) != m_models.end())
		{
			return m_models[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get model: %s, but it has never been loaded!", p_handle.c_str());
			return nullptr;
		}
	}

	Render::EffectInterface* ResourceManager::GetEffect(std::string p_handle)
	{
		if(m_effects.find(p_handle) != m_effects.end())
		{
			return m_effects[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get effect: %s, but it has never been loaded!", p_handle.c_str());
			return nullptr;
		}
	}

	Render::TextureInterface* ResourceManager::GetTexture( std::string p_handle )
	{
		if(m_textures.find(p_handle) != m_textures.end())
		{
			return m_textures[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get texture: %s, but it has never been loaded!", p_handle.c_str());
			return nullptr;
		}
	}

	Render::MeshInterface* ResourceManager::GetMesh( std::string p_handle )
	{
		if(m_meshes.find(p_handle) != m_meshes.end())
		{
			return m_meshes[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get mesh: %s, but it has never been loaded!", p_handle.c_str());
			return nullptr;
		}
	}

	const std::string& ResourceManager::ResolveStringFromTexture(Render::TextureInterface* p_texture)
	{
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			if((*itr).second == p_texture)
				return (*itr).first;
		}
		assert(false);
	}
#ifndef COMPILE_LEVEL_EDITOR
	Physics::PhysicsMeshInterface* ResourceManager::GetPhysicsMesh( std::string p_handle )
	{
		if(m_physicMeshes.find(p_handle) != m_physicMeshes.end())
		{
			return m_physicMeshes[p_handle].get();
		}
		else
		{
			std::string nonIndexed = p_handle.substr(0, p_handle.size()-1);
			delete m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + nonIndexed + ".DAE", p_handle, true);
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::PINK_PRINT, "TESTING LOADING PHYSICS MESH ONLY! %s", p_handle.c_str());
			return m_physicMeshes[p_handle].get();
		}
	}
	
	std::string ResourceManager::GetScript( std::string p_scriptName )
	{
		if(m_scripts.find(p_scriptName) != m_scripts.end())
		{
			return p_scriptName;
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get script: %s.lua, but it has never been loaded!", p_scriptName.c_str());
			return "";
		}
	}
	void ResourceManager::ReloadAllScripts()
	{
		for(auto itr = m_scripts.begin(); itr != m_scripts.end(); ++itr)
		{
			ForceLoadScript((*itr).second);
		}
	}
#endif


	const std::string& ResourceManager::ResolveStringFromEffect(Render::EffectInterface* p_effect)
	{
		for(auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
		{
			if((*itr).second == p_effect)
				return (*itr).first;
		}
		assert(false);
	}

	const std::string ResourceManager::ResolveStringFromModel(Model* p_model)
	{
		for(auto itr = m_models.begin(); itr != m_models.end(); ++itr)
		{
			if((*itr).second == p_model)
				return (*itr).first;
		}
		return "";
	}

	const std::string& ResourceManager::GetWorkingDirectory()
	{
		return m_workingDirectory;
	}

	void ResourceManager::SetupDefaultResources()
	{
		m_defaultTexture = m_context->m_renderer->CreateTexture();
		m_defaultTexture->CreateEmptyTexture(4, 4, Render::TextureFormat::TEXTURE_RGBA);
	}

	void ResourceManager::AddProtectedResource(const std::string& p_name)
	{
		m_protectedResources.push_back(p_name);
	}

	//////////////////////////////////////////////////////////////////////////
	//Remove functions
	//////////////////////////////////////////////////////////////////////////
	void ResourceManager::Clean()
	{
		std::vector<std::map<std::string, Model*>::iterator> m_ittsToRemove;

		// Clean models.
		for(auto itr = m_models.begin(); itr != m_models.end(); ++itr)
		{
			if(std::find(m_protectedResources.begin(), m_protectedResources.end(), (*itr).first) == m_protectedResources.end())
			{
				m_ittsToRemove.push_back(itr);
			}			
		}
		for(auto itrb = m_ittsToRemove.begin(); itrb != m_ittsToRemove.end(); ++itrb)
		{
			RemoveModel((*itrb)->second);
		}

		// Clear physics meshes.
		m_physicMeshes.clear();

		// Clean particles.
		for(auto itr = m_particles.begin(); itr != m_particles.end(); ++itr)
		{
			for(auto itrp = (*itr).second.begin(); itrp != (*itr).second.end(); ++itrp)
			{
				delete (*itrp);
			}
		}
		m_particles.clear();

		// Clean sounds.
		for(auto itr = m_soundAudios.begin(); itr != m_soundAudios.end(); ++itr)
		{
			delete (*itr).second;
		}
		m_soundAudios.clear();
	
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			//std::cout << (*itr).first << std::endl;
			m_context->m_renderer->ReleaseTexture((*itr).second);
		}
		m_textures.clear();

		// Clear render resource references.
		m_meshes.clear();

		m_context->m_renderer->CleanResources(Render::RenderResources::RR_MATERIAL);

		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "ResourceManager clean.");
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Models %d", m_models.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Meshes %d", m_meshes.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "PhysicsMeshes %d", m_physicMeshes.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Effects %d", m_effects.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Textures %d", m_textures.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Sounds %d", m_soundAudios.size());
		m_context->m_logger->LogText(LogTag::GAME, LogLevel::PINK_PRINT, "Particles %d", m_particles.size());
	}

	void ResourceManager::RemoveModel(Model* p_model)
	{
		// Remove rendering meshes.
		m_context->m_renderer->RemoveMesh(p_model->m_meshes[0]);
		m_context->m_renderer->RemoveMesh(p_model->m_meshes[1]);

		// Remove physics meshes.
		for(auto itr = p_model->m_physicsMeshes.begin(); itr != p_model->m_physicsMeshes.end(); ++itr)
		{
			(*itr) = nullptr;
		}
#ifndef COMPILE_LEVEL_EDITOR
		// Remove animation.
		delete p_model->m_animation;
		p_model->m_animation = nullptr;
#endif
		// Remove model from map.
		std::string modelName = ResolveStringFromModel(p_model);
		if(modelName != "")
		{
			m_models.erase(modelName);
		}
	}

	void ResourceManager::RemoveRenderingMeshesFromModel(Model* p_model)
	{
		m_context->m_renderer->RemoveMesh(p_model->m_meshes[0]);
		m_context->m_renderer->RemoveMesh(p_model->m_meshes[1]);
	}

#ifndef COMPILE_LEVEL_EDITOR
	//Export
	void ResourceManager::ExportCollada( Model* p_model, const std::string& p_fileName )
	{
		if(p_model == nullptr)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[EXPORT] Failed, model is NULL");
			return;
		}

		bool errCheck = m_modelExporter->ExportMesh(p_model->m_meshes[0], m_workingDirectory, p_fileName);

		if(errCheck)
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "[EXPORT] Successfully exported model");
		else
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "[EXPORT] Failed to export model");
	}

	Physics::PhysicsMeshInterface* ResourceManager::CreatePhysicsMesh( RootEngine::Model* p_model )
	{
		std::string handle = ResolveStringFromMesh(p_model->m_meshes[0]);
		
		if(m_physicMeshes.find(handle) != m_physicMeshes.end())
		{
			return m_physicMeshes[handle].get();
		}
		else
		{
			std::shared_ptr<Physics::PhysicsMeshInterface> pmesh = m_context->m_physics->CreatePhysicsMesh();

			pmesh->Init(p_model->m_positions, (int)p_model->m_positions.size(), p_model->m_indices, (int)p_model->m_indices.size(), (int)p_model->m_indices.size() / 3);

			m_context->m_resourceManager->m_physicMeshes[handle] = pmesh;
			p_model->m_physicsMeshes.push_back(pmesh);
			return m_physicMeshes[handle].get();
		}
	}

	std::string ResourceManager::ResolveStringFromMesh( Render::MeshInterface* p_mesh )
	{
		for(auto itr = m_meshes.begin(); itr != m_meshes.end(); ++itr)
		{
			if((*itr).second == p_mesh)
				return (*itr).first;
		}
		return "";
	}

#endif
}
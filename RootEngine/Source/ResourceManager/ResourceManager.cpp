#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/GameSharedContext.h>
namespace RootEngine
{

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		auto modelitr = m_models.begin();
		for(; modelitr != m_models.end(); modelitr++)
		{
			delete (*modelitr).second;
			(*modelitr).second = nullptr;
		}
		m_models.clear();
	}

	void ResourceManager::Init(std::string p_workingDirectory, GameSharedContext* p_context)
	{
		m_context = p_context;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(m_context->m_renderer));
#ifndef COMPILE_LEVEL_EDITOR
		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(p_context));
#endif
		m_textureImporter = std::shared_ptr<TextureImporter>(new TextureImporter(m_context->m_logger, m_context->m_renderer));
		
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);
	}

	//////////////////////////////////////////////////////////////////////////
	//Load functions
	//////////////////////////////////////////////////////////////////////////

#ifndef COMPILE_LEVEL_EDITOR
	Model* ResourceManager::LoadCollada(std::string p_path)
	{
		if(m_models.find(p_path) == m_models.end())
		{
			Model* model = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + ".DAE");

			if(model)
			{
				m_models[p_path] = model;
				return m_models[p_path];
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Model already exists: %s", p_path.c_str());
			return m_models[p_path];
		}
		
	}

	std::string ResourceManager::LoadScript( std::string p_scriptName )
	{
		if(m_scripts.find(p_scriptName) == m_scripts.end())
		{
			m_context->m_script->LoadScript(p_scriptName + ".lua");
			m_scripts[p_scriptName] = p_scriptName;
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "Loaded script: %s.lua", p_scriptName.c_str());
			return p_scriptName;
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Script already exists: %s.lua", p_scriptName.c_str());
			return "";
		}
		
	}

	std::string ResourceManager::ForceLoadScript( std::string p_scriptName )
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Force loaded script: %s.lua, it may already exist in Resource Manager!", p_scriptName.c_str());
		m_context->m_script->LoadScript(p_scriptName + ".lua");
		m_scripts[p_scriptName] = p_scriptName;
		return p_scriptName;
	}
#endif

	Render::EffectInterface* ResourceManager::LoadEffect(std::string p_path)
	{
		if(m_effects.find(p_path) == m_effects.end())
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Loading effect: %s", p_path.c_str());

			m_effectImporter->Load(m_workingDirectory + "Assets\\Effects\\" + p_path + ".effect");

			m_effects[p_path] = m_effectImporter->m_effect;

			if(m_effectImporter->m_effect)
				return m_effects[p_path].get();
			else
				return nullptr;
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Effect already exists: %s", p_path.c_str());
			return m_effects[p_path].get();
		}

		
	}

	Render::TextureInterface* ResourceManager::LoadTexture( std::string p_path, Render::TextureType::TextureType p_type )
	{
		if(m_textures.find(p_path) == m_textures.end())
		{
			std::shared_ptr<Render::TextureInterface> tex;
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
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::SUCCESS, "Loaded texture '%s'", p_path.c_str());
				m_textures[p_path] = tex;
				return m_textures[p_path].get();
			}
			else
			{
				m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Error loading texture '%s'", p_path.c_str());
				return nullptr;
			}
		}
		else
		{
			//m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Texture already exists: %s", p_path.c_str());
			return m_textures[p_path].get();
		}
		
	}

	Model* ResourceManager::CreateModel(const std::string& p_path)
	{
		if(m_models.find(p_path) == m_models.end())
		{
			Model* model = new Model();
			model->m_meshes.resize(1);
			m_meshes[p_path + "0"] = m_context->m_renderer->CreateMesh();
			model->m_meshes[0] = m_meshes[p_path + "0"].get();
			
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
			return m_effects[p_handle].get();
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
			return m_textures[p_handle].get();
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
			return m_meshes[p_handle].get();
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
			if((*itr).second.get() == p_texture)
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
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Trying to get physics mesh: %s, but it has never been loaded!", p_handle.c_str());
			return nullptr;
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
#endif
	const std::string& ResourceManager::ResolveStringFromEffect(Render::EffectInterface* p_effect)
	{
		for(auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
		{
			if((*itr).second.get() == p_effect)
				return (*itr).first;
		}
		assert(false);
	}

	const std::string& ResourceManager::ResolveStringFromModel(Model* p_model)
	{
		for(auto itr = m_models.begin(); itr != m_models.end(); ++itr)
		{
			if((*itr).second == p_model)
				return (*itr).first;
		}
		assert(false);
	}

	const std::string& ResourceManager::GetWorkingDirectory()
	{
		return m_workingDirectory;
	}

	

	

	

}
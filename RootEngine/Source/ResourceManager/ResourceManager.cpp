#include <RootEngine/Include/ResourceManager/ResourceManager.h>

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

		auto effectitr = m_effects.begin();
		for(; effectitr != m_effects.end(); effectitr++)
		{
			delete (*effectitr).second;
			(*effectitr).second = nullptr;
		}
		m_effects.clear();

	}

	void ResourceManager::Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer, Logging* p_logger)
	{
		m_logger			= p_logger;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(p_renderer));
		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(m_logger, p_renderer, this));
		m_textureImporter = std::shared_ptr<TextureImporter>(new TextureImporter(m_logger, p_renderer));
		
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);
	}

	//////////////////////////////////////////////////////////////////////////
	//Load functions
	//////////////////////////////////////////////////////////////////////////
	void ResourceManager::LoadCollada(std::string p_path)
	{
		m_models[p_path] = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + "\\" + p_path + ".DAE");
	}

	void ResourceManager::LoadMesh( std::string p_path )
	{
		// TODO 
	}

	void ResourceManager::LoadEffect(std::string p_path)
	{
		m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Loading effect: %s", p_path.c_str());

		m_effectImporter->Load(m_workingDirectory + "Assets\\Effects\\" + p_path + ".effect");

		m_effects[p_path] = m_effectImporter->m_effect;
	}

	bool ResourceManager::LoadTexture( std::string p_path )
	{
		if(m_textureImporter->LoadTexture(m_workingDirectory + "Assets\\Textures\\" + p_path))
		{
			m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Successfully loaded texture '%s'\n", p_path.c_str());
			m_textures[p_path] = m_textureImporter->GetTexture();
			return true;
		}
		else
		{
			m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Error loading texture '%s'\n", p_path.c_str());
			return false;
		}
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
			m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Model has not been loaded: %s", p_handle.c_str());
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
			m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Effect has not been loaded: %s", p_handle.c_str());
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
			m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Texture has not been loaded: %s", p_handle.c_str());
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
			m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Mesh has not been loaded: %s", p_handle.c_str());
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
	}

	const std::string& ResourceManager::ResolveStringFromEffect(Render::EffectInterface* p_effect)
	{
		for(auto itr = m_effects.begin(); itr != m_effects.end(); ++itr)
		{
			if((*itr).second == p_effect)
				return (*itr).first;
		}
	}

	const std::string& ResourceManager::ResolveStringFromModel(Model* p_model)
	{
		for(auto itr = m_models.begin(); itr != m_models.end(); ++itr)
		{
			if((*itr).second == p_model)
				return (*itr).first;
		}
	}

	const std::string& ResourceManager::GetWorkingDirectory()
	{
		return m_workingDirectory;
	}
}
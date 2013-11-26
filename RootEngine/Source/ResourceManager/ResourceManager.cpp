#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootEngine
{

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer, Logging* p_logger)
	{
		m_logger			= p_logger;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(p_renderer));
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);

		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(m_logger, p_renderer));
	}

	void ResourceManager::LoadCollada(std::string p_path)
	{
		m_models[p_path] = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + "\\" + p_path + ".DAE");
	}

	void ResourceManager::LoadEffect(std::string p_path)
	{
		m_effectImporter->Load(m_workingDirectory + "Assets//Shaders//" + p_path + ".effect");
		m_effects[p_path] = m_effectImporter->m_effect;
	}

	std::shared_ptr<Render::EffectInterface> ResourceManager::GetEffect(std::string p_handle)
	{
		return m_effects[p_handle];
	}

	std::shared_ptr<Model> ResourceManager::GetModel( std::string p_handle )
	{
		return m_models[p_handle];
	}

}
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootEngine
{
	ResourceManager::ResourceManager(Logging* p_logger) : m_logger(p_logger)
	{
		m_modelImporter = new ModelImporter(m_logger);
	}

	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		delete m_modelImporter;
	}

	void ResourceManager::Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer)
	{
		m_workingDirectory = p_workingDirectory;
		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(p_renderer));
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);
	}

	void ResourceManager::LoadCollada(std::string p_path)
	{
		Model* testmodel = m_modelImporter->LoadModel(p_path);
	}

	void ResourceManager::LoadEffect(std::string p_path)
	{
		m_effectImporter->Load(m_workingDirectory + "Assets//Scripts//" + p_path + ".yaml");
		m_effects[p_path] = m_effectImporter->m_effect;
	}

	Render::Mesh* ResourceManager::GetMesh(std::string p_handle)
	{
		return nullptr;
	}

	std::shared_ptr<Render::EffectInterface> ResourceManager::GetEffect(std::string p_handle)
	{
		return m_effects[p_handle];
	}
}
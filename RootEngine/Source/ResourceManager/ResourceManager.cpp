#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace RootEngine
{
	ResourceManager::ResourceManager(Logging* p_logger) : m_logger(p_logger)
	{
		m_modelImporter = new ModelImporter(m_logger);
	}

	ResourceManager::~ResourceManager()
	{
		delete m_modelImporter;
	}

	void ResourceManager::LoadCollada(std::string p_path)
	{
		Model* testmodel = m_modelImporter->LoadModel(p_path);
	}

	Render::Mesh* ResourceManager::GetMesh(std::string p_handle)
	{
		return nullptr;
	}
}
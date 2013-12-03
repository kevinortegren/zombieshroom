#include <Utility\ECS\Include\EntityExporter.h>
#include <Utility\ECS\Include\EntityManager.h>

#include <iostream>
#include <fstream>
#include <sstream>

void ECS::EntityExporter::SetExporter(COMPEXPORT p_exporter)
{
	m_exporter = p_exporter;
}

void ECS::EntityExporter::Export(const std::string& p_filepath)
{
	// Sequence = Loop
	// Map = Table
	// Value

	YAML::Emitter out;

	for(int i = 0; i < m_entityManager->m_entities.size(); i++)
	{
		std::cout << "Entity " << m_entityManager->m_entities[i]->GetId() << std::endl;
		out << m_entityManager->m_entities[i]->GetId();
	}

	// For each component type.
	for(int i = 0; i < m_entityManager->m_components.size(); i++)
	{
		std::stringstream ss;
		ss << "Component" << i;

		// For each entity using this component.
		for(int j = 0; j < m_entityManager->m_components[i].size(); j++)
		{
			if(m_entityManager->m_components[i][j] == nullptr)
				continue;

			out << YAML::Key << "Entity" << YAML::Value << j;
			m_exporter(out, m_entityManager->m_components[i][j].get(), i);
		}
	}

	std::ofstream file;
	file.open(p_filepath);
	file << out.c_str();
}
#include <Utility\ECS\Include\World.h>

#include <yaml-cpp\yaml.h>
#include <iostream>
#include <fstream>
#include <sstream>

ECS::EntityManager* ECS::World::GetEntityManager() 
{ 
	return &m_entityManager; 
}
ECS::EntitySystemManager* ECS::World::GetSystemManager() 
{ 
	return &m_systemManager; 
}
ECS::TagManager* ECS::World::GetTagManager()
{
	return &m_tagManager;
}
ECS::GroupManager* ECS::World::GetGroupManager()
{
	return &m_groupManager;
}
void ECS::World::SetDelta(float p_dt)
{
	m_dt = p_dt;
}
float ECS::World::GetDelta()
{
	return m_dt;
}
void ECS::World::SetExporter(COMPEXPORT p_exporter)
{
	m_exporter = p_exporter;
}
void ECS::World::Export()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;
	for(int i = 0; i < m_entityManager.m_entities.size(); i++)
	{
		std::cout << "Entity " << m_entityManager.m_entities[i]->GetId() << std::endl;
		out << m_entityManager.m_entities[i]->GetId();
	}
	out << YAML::EndSeq;
	// For each component type.
	for(int i = 0; i < m_entityManager.m_components.size(); i++)
	{
		std::stringstream ss;
		ss << "Component" << i;

		out << YAML::Key << ss.str();
		out << YAML::Value << YAML::BeginMap;

		// For each entity using this component.
		for(int j = 0; j < m_entityManager.m_components[i].size(); j++)
		{
			if(m_entityManager.m_components[i][j] == nullptr)
				continue;

			out << YAML::Key << "Entity" << YAML::Value << j;
			m_exporter(out, m_entityManager.m_components[i][j].get(), i);
		}

		out << YAML::EndMap;
	}

	out << YAML::EndMap;
	std::ofstream file;
	file.open("export.yaml");
	file << out.c_str();
}




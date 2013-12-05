#include <Utility\ECS\Include\EntityExporter.h>
#include <Utility\ECS\Include\World.h>

#include <iostream>
#include <fstream>
#include <sstream>

void ECS::EntityExporter::SetExporter(COMPEXPORT p_exporter)
{
	m_exporter = p_exporter;
}

void ECS::EntityExporter::Export(const std::string& p_filepath)
{
	YAML::Emitter out;
	out << YAML::BeginSeq;

	// Export Entities.
	out << YAML::BeginMap;
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;
	for(unsigned int i = 0; i < m_world->GetEntityManager()->m_entities.size(); i++)
	{
		std::cout << "Entity " << m_world->GetEntityManager()->m_entities[i]->GetId() << std::endl;
		out << m_world->GetEntityManager()->m_entities[i]->GetId();
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export Components.
	out << YAML::BeginMap;
	out << YAML::Key << "Components";
	out << YAML::Value << YAML::BeginSeq;
	// For each component type.
	for(unsigned int i = 0; i < m_world->GetEntityManager()->m_components.size(); i++)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Type" << YAML::Value << i;
		out << YAML::Key << "Data" << YAML::Value << YAML::BeginSeq;
		// For each entity using this component.
		for(unsigned int j = 0; j < m_world->GetEntityManager()->m_components[i].size(); j++)
		{
			if(m_world->GetEntityManager()->m_components[i][j] == nullptr)
				continue;
			out << YAML::BeginMap;
			out << YAML::Key << "Entity" << YAML::Value << j;
			m_exporter(out, m_world->GetEntityManager()->m_components[i][j].get(), i);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;
	}
	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export Tags.
	out << YAML::BeginMap;
	out << YAML::Key << "Tags";
	out << YAML::Value << YAML::BeginSeq;
	out << YAML::BeginMap;
	for(auto itr = m_world->GetTagManager()->m_tags.begin(); itr != m_world->GetTagManager()->m_tags.end(); ++itr)
	{
		out << YAML::Key << "Id" << YAML::Value << (*itr).second->GetId();
		out << YAML::Key << "Tag" << YAML::Value << (*itr).first;
	}
	out << YAML::EndMap;

	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export Groups.
	out << YAML::BeginMap;
	out << YAML::Key << "Groups";
	out << YAML::Value << YAML::BeginSeq;
	out << YAML::BeginMap;
	for(auto itr = m_world->GetGroupManager()->m_groups.begin(); itr != m_world->GetGroupManager()->m_groups.end(); ++itr)
	{
		out << YAML::Key << "Id" << YAML::Value << (*itr).second->GetId();
		out << YAML::Key << "Group" << YAML::Value << (*itr).first;
	}
	out << YAML::EndMap;

	out << YAML::EndSeq;

	std::ofstream file;
	file.open(p_filepath);
	file << out.c_str();
}
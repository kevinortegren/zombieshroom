#include <Utility\ECS\Include\EntityExporter.h>
#include <Utility\ECS\Include\World.h>

#include <iostream>
#include <fstream>
#include <sstream>

void ECS::EntityExporter::SetExporter(COMPEXPORT p_exporter)
{
	m_exporter = p_exporter;
}

void ECS::EntityExporter::Export(const std::string& p_filepath, std::map<ECS::Entity*, std::string>* p_entityNames)
{
	std::string content = Export(p_entityNames);

	std::ofstream file;
	file.open(p_filepath);
	file << content.c_str();
}

std::string ECS::EntityExporter::Export(std::map<ECS::Entity*, std::string>* p_entityNames)
{
	std::vector<bool> nonExportIds;
	nonExportIds.resize(m_world->GetEntityManager()->GetNumEntities());

	auto range = m_world->GetGroupManager()->GetEntitiesInGroup("NonExport");
	for(auto itr = range.first; itr != range.second; ++itr)
	{
		if((*itr).second->GetId() == -1)
			continue;

		nonExportIds[itr->second->m_id] = true;
	}

	YAML::Emitter out;
	out << YAML::BeginSeq;

	// Export Entities.
	out << YAML::BeginMap;
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;

	for(int i = 0; i < m_world->GetEntityManager()->m_nextID; i++)
	{
		if(m_world->GetEntityManager()->m_entities[i].m_id == -1)
			continue;
		int id = m_world->GetEntityManager()->m_entities[i].m_id;
		if(nonExportIds[id])
			continue;

		out << m_world->GetEntityManager()->m_entities[i].m_id;
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

			if(nonExportIds[j])
				continue;

			out << YAML::BeginMap;
			out << YAML::Key << "Entity" << YAML::Value << j;
			m_exporter(out, m_world->GetEntityManager()->m_components[i][j], i);
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
	
	for(auto itr = m_world->GetTagManager()->m_tags.begin(); itr != m_world->GetTagManager()->m_tags.end(); ++itr)
	{
		if((*itr).second->GetId() == -1)
			continue;

		if(nonExportIds[(*itr).second->m_id])
			continue;

		out << YAML::BeginMap;
		out << YAML::Key << "Id" << YAML::Value << (*itr).second->m_id;
		out << YAML::Key << "Tag" << YAML::Value << (*itr).first;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export Groups.
	out << YAML::BeginMap;
	out << YAML::Key << "Groups";
	out << YAML::Value << YAML::BeginSeq;
	
	for(auto itr = m_world->GetGroupManager()->m_groups.begin(); itr != m_world->GetGroupManager()->m_groups.end(); ++itr)
	{
		if((*itr).first != "NonExport")
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Id" << YAML::Value << (*itr).second->m_id;
			out << YAML::Key << "Group" << YAML::Value << (*itr).first;
			out << YAML::EndMap;
		}
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export Storage.
	out << YAML::BeginMap;
	out << YAML::Key << "Storage";
	out << YAML::Value << YAML::BeginSeq;

	for(auto itr = m_world->GetStorage()->m_values.begin(); itr != m_world->GetStorage()->m_values.end(); ++itr)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Key" << YAML::Value << (*itr).first;
		out << YAML::Key << "Value" << YAML::Value << (*itr).second;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	// Export names
	if (p_entityNames != nullptr)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "EntityNames" << YAML::Value << YAML::BeginSeq;
		
		for(int i = 0; i < m_world->GetEntityManager()->m_nextID; i++)
		{
			if(m_world->GetEntityManager()->m_entities[i].m_id == -1)
				continue;

			if(nonExportIds[m_world->GetEntityManager()->m_entities[i].m_id])
				continue;

			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << m_world->GetEntityManager()->m_entities[i].m_id;
			out << YAML::Key << "Name" << YAML::Value << p_entityNames->find(&m_world->GetEntityManager()->m_entities[i])->second.c_str();
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;
	}

	out << YAML::EndSeq;

	return std::string(out.c_str());
}
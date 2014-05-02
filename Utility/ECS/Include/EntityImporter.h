#pragma once

#include <string>
#include <Utility\ECS\Include\Entity.h>
#include <yaml-cpp\yaml.h>

namespace ECS
{
	class World;
	class EntityImporter
	{
	public:
		EntityImporter(World* p_world)
			: m_world(p_world) {}
		typedef void (*COMPIMPORT)(ECS::World*, int, Entity*, const YAML::Node&);
		void SetImporter(COMPIMPORT p_importer);

		void Import(const std::string& p_filename, std::map<ECS::Entity*, std::string>* p_entityNames = nullptr);

	private:
		World* m_world;
		COMPIMPORT m_importer;
	};
}
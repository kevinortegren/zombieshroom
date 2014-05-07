#pragma once

#include <map>
#include <Utility\ECS\Include\Component.h>
#include <Utility/ECS/Include/Entity.h>
#include <yaml-cpp\yaml.h>

namespace ECS
{
	class World;
	class EntityExporter
	{
	public:
		EntityExporter(World* p_world)
			: m_world(p_world) {}

		typedef void (*COMPEXPORT)(YAML::Emitter& p_emitter, ComponentInterface* p_component, int p_type);

		void SetExporter(COMPEXPORT p_exporter);
		void Export(const std::string& p_filepath, std::map<ECS::Entity*, std::string>* p_entityNames = nullptr);
		std::string Export(std::map<ECS::Entity*, std::string>* p_entityNames = nullptr);
	private:
		World* m_world;
		COMPEXPORT m_exporter;
	};
}
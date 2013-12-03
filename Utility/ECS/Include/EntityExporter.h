#pragma once

#include <Utility\ECS\Include\Component.h>
#include <yaml-cpp\yaml.h>

namespace ECS
{
	class EntityManager;
	class EntityExporter
	{
	public:
		EntityExporter(EntityManager* p_entityManager)
			: m_entityManager(p_entityManager) {}

		typedef void (*COMPEXPORT)(YAML::Emitter& p_emitter, ComponentInterface* p_component, int p_type);

		void SetExporter(COMPEXPORT p_exporter);
		void Export(const std::string& p_filepath);

	private:
		EntityManager* m_entityManager;
		COMPEXPORT m_exporter;
	};
}
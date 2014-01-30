#pragma once
#include <yaml-cpp\yaml.h>
#include "OnCollide.h"
#include "OnCreate.h"
#include "OnDestroy.h"
#include "Entity.h"

namespace AbilityEditorNameSpace
{
	class Exporter
	{
	public:
		Exporter();
		~Exporter();
		void Export(const std::string& p_filepath, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy );
	private:
		void ExportComponent(YAML::Emitter& p_emitter, AbilityComponents::MainComponent* p_component, unsigned int p_type);
		void ExportEntity(YAML::Emitter& p_emitter, AbilityEditorNameSpace::Entity* p_entity);
	};
}
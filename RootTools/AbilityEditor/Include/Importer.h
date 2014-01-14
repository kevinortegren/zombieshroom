#pragma once
#include <yaml-cpp\yaml.h>
#include "OnCollide.h"
#include "OnCreate.h"
#include "OnDestroy.h"
namespace AbilityEditorNameSpace
{
	class Importer
	{
	public:
		Importer();
		~Importer();
		void Import(std::string &p_filename, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy);
	private:
		AbilityEntity::Entity* ImportEntity(YAML::Emitter& p_emitter);
	};
}
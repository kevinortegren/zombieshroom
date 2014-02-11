#pragma once

#include "OnCollide.h"
#include "OnCreate.h"
#include "OnDestroy.h"
#include "Entity.h"
#include <iostream>
#include <fstream>

namespace AbilityEditorNameSpace
{
	class ScriptGenerator
	{
	public:
		ScriptGenerator();
		~ScriptGenerator();

		void GenerateScript(QString p_filePath, QString p_name, Entity* p_entity, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy);

	private:
		void WriteVariables();
		void WriteOnCreate(OnCreate* p_onCreate);
		void WriteChargeDone();
		void WriteChannelingDone();
		void WriteOnCollide(OnCreate* p_onCreate, OnCollide* p_onCollide);
		void WriteOnDestroy(OnDestroy* p_onDestroy);

	private:
		std::string m_name;
		std::ofstream m_file;

		float m_damage;
		float m_knockback;

		Entity* m_entity;
	};
}
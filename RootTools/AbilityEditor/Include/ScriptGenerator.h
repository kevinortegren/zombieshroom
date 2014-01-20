#pragma once

#include "OnCollide.h"
#include "OnCreate.h"
#include "OnDestroy.h"
#include <iostream>
#include <fstream>

namespace AbilityEditorNameSpace
{
	class ScriptGenerator
	{
	public:
		ScriptGenerator();
		~ScriptGenerator();

		void GenerateScript(QString p_filePath, QString p_scriptName, OnCreate* p_onCreate, OnCollide* p_onCollide, OnDestroy* p_onDestroy);

	private:
		void WriteOnCreate(OnCreate* p_onCreate);
		void WriteOnCollide(OnCreate* p_onCollide);
		void WriteOnDestroy(OnCreate* p_onDestroy);

	private:
		std::string m_name;
		std::ofstream m_file;

		int m_entityOffset;
	};
}
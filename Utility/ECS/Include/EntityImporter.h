#pragma once

#include <string>

namespace ECS
{
	class EntityImporter
	{
	public:
		typedef void (*COMPIMPORT)();
		void SetImporter(COMPIMPORT p_importer);

		void Import(const std::string& p_filename);

	private:
		COMPIMPORT m_importer;
	};
}
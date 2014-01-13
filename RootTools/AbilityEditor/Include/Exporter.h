#pragma once
#include <yaml-cpp\yaml.h>
namespace Exporter
{
	class Exporter
	{
	public:
		Exporter();
		~Exporter();
		void Export(const std::string& p_filepath);
	private:
	};
}
#pragma once

#include <string>
#include <map>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>

namespace Render
{
	class Mesh;
}

namespace RootEngine
{

	class ResourceManagerInterface
	{
	public:
		virtual void LoadCollada(std::string p_path) = 0;

		virtual Render::Mesh* GetMesh(std::string p_handle) = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		ResourceManager(Logging* p_logger);
		~ResourceManager();

		void LoadCollada(std::string p_path);

		Render::Mesh* GetMesh(std::string p_handle);


	private:
		std::map<std::string, Model*> m_models;
		std::map<std::string, Render::Mesh*> m_meshes;
		//std::map<std::string, Render::Texture*> m_textures;

		ModelImporter* m_modelImporter;
		Logging* m_logger;
	};
}
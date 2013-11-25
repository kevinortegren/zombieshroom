#pragma once

#include <string>
#include <map>

namespace Render
{
	class Mesh;
}

namespace RootEngine
{
	class Model;

	class ResourceManagerInterface
	{
	public:
		virtual void LoadCollada(std::string p_path) = 0;

		virtual Render::Mesh* GetMesh(std::string p_handle) = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		ResourceManager();
		~ResourceManager();

		void LoadCollada(std::string p_path);

		Render::Mesh* GetMesh(std::string p_handle);


	private:
		std::map<std::string, Model*> m_models;
		std::map<std::string, Render::Mesh*> m_meshes;
		//std::map<std::string, Render::Texture*> m_textures;
	};
}
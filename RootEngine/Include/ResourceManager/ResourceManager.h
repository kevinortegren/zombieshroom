#pragma once

#include <string>
#include <map>

#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/EffectImporter.h>

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
		virtual void LoadEffect(std::string p_path) = 0;

		virtual Render::Mesh* GetMesh(std::string p_handle) = 0;
		virtual std::shared_ptr<Render::EffectInterface> GetEffect(std::string p_handle) = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		ResourceManager(Logging* p_logger);
		ResourceManager();
		~ResourceManager();
		
		void Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer);

		void LoadCollada(std::string p_path);
		void LoadEffect(std::string p_path);

		Render::Mesh* GetMesh(std::string p_handle);
		std::shared_ptr<Render::EffectInterface> GetEffect(std::string p_handle);

	private:
		std::map<std::string, Model*> m_models;
		std::map<std::string, Render::Mesh*> m_meshes;
		std::map<std::string, std::shared_ptr<Render::EffectInterface>> m_effects;
		//std::map<std::string, Render::Texture*> m_textures;


		ModelImporter* m_modelImporter;
		Logging* m_logger;
		std::shared_ptr<EffectImporter> m_effectImporter;
		std::string m_workingDirectory;
	};
}
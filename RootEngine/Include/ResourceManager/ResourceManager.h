#pragma once

#include <string>
#include <map>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/EffectImporter.h>
#include <RootEngine/Include/TextureImporter.h>


namespace RootEngine
{
	class ResourceManagerInterface
	{
	public:
		//Load functions
		virtual void LoadCollada(std::string p_path) = 0;
		virtual void LoadMesh(std::string p_path) = 0;
		virtual void LoadEffect(std::string p_path) = 0;
		virtual bool LoadTexture(std::string p_path) = 0;

		//Get functions
		virtual Model* GetModel(std::string p_handle) = 0;
		virtual Render::EffectInterface*	GetEffect(std::string p_handle) = 0;
		virtual Render::TextureInterface*	GetTexture(std::string p_handle) = 0;
		virtual Render::MeshInterface*		GetMesh(std::string p_handle) = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		ResourceManager();
		~ResourceManager();
		
		void Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer, Logging* p_logger);

		//Load functions
		void LoadCollada(std::string p_path);
		void LoadMesh(std::string p_path);
		void LoadEffect(std::string p_path);
		bool LoadTexture(std::string p_path);

		//Get functions
		Model* GetModel(std::string p_handle);
		Render::EffectInterface*	GetEffect(std::string p_handle);
		Render::TextureInterface*	GetTexture(std::string p_handle);
		Render::MeshInterface*		GetMesh(std::string p_handle);

	private:
		//Resources
		std::map<std::string, Model*> m_models;
		std::map<std::string, Render::MeshInterface*>		m_meshes;
		std::map<std::string, Render::EffectInterface*>		m_effects;
		std::map<std::string, Render::TextureInterface*>	m_textures;

		//Importers
		std::shared_ptr<ModelImporter> m_modelImporter;
		std::shared_ptr<EffectImporter> m_effectImporter;
		std::shared_ptr<TextureImporter> m_textureImporter;

		//Other
		Logging* m_logger;
		std::string m_workingDirectory;
	};
}
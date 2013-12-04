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
		virtual void LoadEffect(std::string p_path) = 0;
		virtual bool LoadTexture(std::string p_path) = 0;

#ifndef COMPILE_LEVEL_EDITOR
		virtual void LoadCollada(std::string p_path) = 0;
		virtual Model* GetModel(std::string p_handle) = 0;
		virtual const std::string& ResolveStringFromModel(Model* p_model) = 0;
		virtual Render::MeshInterface*		GetMesh(std::string p_handle) = 0;
#endif

		//Get functions
		virtual Render::EffectInterface*	GetEffect(std::string p_handle) = 0;
		virtual Render::TextureInterface*	GetTexture(std::string p_handle) = 0;
		
		// Resolve functions.
		virtual const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture) = 0;
		virtual const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect) = 0;
		
		virtual const std::string& GetWorkingDirectory() = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		ResourceManager();
		~ResourceManager();
		
		void Init(std::string p_workingDirectory, Render::RendererInterface* p_renderer, Logging* p_logger);

#ifndef COMPILE_LEVEL_EDITOR
		void LoadCollada(std::string p_path);
		Model* GetModel(std::string p_handle);
#endif

		void LoadEffect(std::string p_path);
		bool LoadTexture(std::string p_path);

		Render::EffectInterface* GetEffect(std::string p_handle);
		Render::TextureInterface* GetTexture(std::string p_handle);

#ifndef COMPILE_LEVEL_EDITOR
		std::map<std::string, Model*> m_models;
#endif

		std::map<std::string, Render::MeshInterface*> m_meshes;
		std::map<std::string, Render::EffectInterface*> m_effects;
		std::map<std::string, Render::TextureInterface*> m_textures;

		const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture);
		const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect);

		const std::string& GetWorkingDirectory();

	private:
		Logging* m_logger;

#ifndef COMPILE_LEVEL_EDITOR
		//Load functions
		void LoadCollada(std::string p_path);
		void LoadMesh(std::string p_path);
		
		//Get functions
		Model* GetModel(std::string p_handle);		
		Render::MeshInterface*		GetMesh(std::string p_handle);

		const std::string& ResolveStringFromModel(Model* p_model);
		

	private:
		//Resources
		std::map<std::string, Model*> m_models;
		std::map<std::string, Render::MeshInterface*>		m_meshes;
		std::map<std::string, Render::EffectInterface*>		m_effects;
		
		//Importers
		std::shared_ptr<ModelImporter> m_modelImporter;
#endif

		std::shared_ptr<EffectImporter> m_effectImporter;
		std::shared_ptr<TextureImporter> m_textureImporter;

		std::string m_workingDirectory;
	};
}
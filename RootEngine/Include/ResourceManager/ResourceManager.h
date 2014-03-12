#pragma once
#include <string>
#include <map>
#include <vector>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/EffectImporter.h>
#include <RootEngine/Include/TextureImporter.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootEngine/Include/ParticleImporter.h>
#include <RootEngine/Sound/Include/SoundAudio.h>

#pragma warning(disable: 4715)//Temporary solution

namespace Physics 
{
	class PhysicsInterface;
}

namespace RootEngine
{
	struct GameSharedContext;

	class ResourceManagerInterface abstract
	{
		
	public:
		#ifndef COMPILE_LEVEL_EDITOR
			virtual Model*	LoadCollada(std::string p_path) = 0;
			virtual Sound::SoundAudioInterface* LoadSoundAudio(std::string p_name, unsigned p_flags) = 0;
			virtual Physics::PhysicsMeshInterface* GetPhysicsMesh(std::string p_handle) = 0;

			virtual std::string ForceLoadScript(std::string p_scriptName) = 0;
			virtual std::string LoadScript(std::string p_scriptName) = 0;
			virtual std::string GetScript(std::string p_scriptName) = 0;
			virtual void		ReloadAllScripts() = 0;
			
		#endif

		virtual Model* CreateModel(const std::string& p_path) = 0;
		virtual Render::TextureInterface* CreateTexture(const std::string& p_path) = 0;

		virtual bool RenameModel(Model* p_model, const std::string& p_name) = 0;

		virtual Render::MeshInterface*		GetMesh(std::string p_handle) = 0;
		virtual Render::EffectInterface*	LoadEffect(std::string p_path) = 0;
		virtual Render::TextureInterface*	LoadTexture(std::string p_path, Render::TextureType::TextureType p_type) = 0;
		virtual std::vector<ParticleSystemStruct*> LoadParticleEmitter(std::string p_handle, bool p_fullPath) = 0;

		virtual const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture) = 0;
		virtual const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect) = 0;

		virtual const std::string& GetWorkingDirectory() = 0;
		virtual const std::string ResolveStringFromModel(Model* p_model) = 0;

		virtual Model*								GetModel(std::string p_handle) = 0;
		virtual Render::EffectInterface*			GetEffect(std::string p_handle) = 0;
		virtual Render::TextureInterface*			GetTexture(std::string p_handle) = 0;

		// Remove methods.
		virtual void RemoveModel(Model* p_model) = 0;
		virtual void RemoveRenderingMeshesFromModel(Model* p_model) = 0;
	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		friend class ModelImporter;

		ResourceManager();
		~ResourceManager();
		
		void Init(std::string p_workingDirectory, GameSharedContext* p_context );

		#ifndef COMPILE_LEVEL_EDITOR
			Model*	LoadCollada(std::string p_path);
			Sound::SoundAudioInterface* LoadSoundAudio(std::string p_name, unsigned p_flags);
			Physics::PhysicsMeshInterface*	GetPhysicsMesh(std::string p_handle);

			std::string ForceLoadScript(std::string p_scriptName);
			std::string LoadScript(std::string p_scriptName);
			std::string GetScript(std::string p_scriptName);
			void		ReloadAllScripts();
		#endif

		Model* CreateModel(const std::string& p_path);
		Render::TextureInterface* CreateTexture(const std::string& p_path);

		bool RenameModel(Model* p_model, const std::string& p_name);

		Render::MeshInterface*		GetMesh(std::string p_handle);
		Render::EffectInterface*	LoadEffect(std::string p_path);
		Render::TextureInterface*	LoadTexture(std::string p_path, Render::TextureType::TextureType p_type);
		std::vector<ParticleSystemStruct*> LoadParticleEmitter(std::string p_handle, bool p_fullPath);

		Model*							GetModel(std::string p_handle);
		Render::EffectInterface*		GetEffect(std::string p_handle);
		Render::TextureInterface*		GetTexture(std::string p_handle);

		const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture);
		const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect);
		const std::string ResolveStringFromModel(Model* p_model);

		const std::string& GetWorkingDirectory();

		// Remove methods.
		void RemoveModel(Model* p_model);
		void RemoveRenderingMeshesFromModel(Model* p_model);

	private:
		// Resources owned by the resource manager.
		std::map<std::string, Model*> m_models;
		std::map<std::string, std::vector<ParticleSystemStruct*>> m_particles;
		std::map<std::string, Sound::SoundAudioInterface*> m_soundAudios;
		std::map<std::string, std::string> m_scripts;

		// Resources owned by the renderer.
		std::map<std::string, Render::MeshInterface*> m_meshes;
		std::map<std::string, Render::EffectInterface*>	m_effects;
		std::map<std::string, Render::TextureInterface*> m_textures;

		// Resources shared with physics module.
		std::map<std::string, std::shared_ptr<Physics::PhysicsMeshInterface>> m_physicMeshes;

		//Importers
#ifndef COMPILE_LEVEL_EDITOR
		std::shared_ptr<ModelImporter>		m_modelImporter;
#endif
		std::shared_ptr<EffectImporter>		m_effectImporter;
		std::shared_ptr<TextureImporter>	m_textureImporter;
		std::shared_ptr<ParticleImporter>	m_particleImporter;

		//Other
		GameSharedContext* m_context;
		std::string m_workingDirectory;

		void SetupDefaultResources();
		Render::TextureInterface* m_defaultTexture;
	};
}
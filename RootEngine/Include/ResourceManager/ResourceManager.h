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

namespace Physics 
{
	class PhysicsInterface;
}

namespace RootEngine
{
	
	struct GameSharedContext;

	struct MESH_DESC 
	{
		std::string							handle;
		std::vector<Render::Vertex1P1N1UV>	verts;
		std::vector<unsigned int>			indices;
		GLenum								primitive;
		unsigned int						faces;
	};

	class ResourceManagerInterface abstract
	{
		
	public:

		//Load functions
		virtual Model*						LoadCollada(std::string p_path) = 0;
		virtual Render::EffectInterface*	LoadEffect(std::string p_path) = 0;
		virtual Render::TextureInterface*	LoadTexture(std::string p_path) = 0;

		//Get functions

		// Resolve functions.
		virtual const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture) = 0;
		virtual const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect) = 0;
		virtual const std::string& ResolveStringFromModel(Model* p_model) = 0;

		virtual const std::string& GetWorkingDirectory() = 0;

		virtual Model*								GetModel(std::string p_handle) = 0;
		virtual Render::EffectInterface*			GetEffect(std::string p_handle) = 0;
		virtual Render::TextureInterface*			GetTexture(std::string p_handle) = 0;
		virtual Render::MeshInterface*				GetMesh(std::string p_handle) = 0;
		virtual Physics::PhysicsMeshInterface*		GetPhysicsMesh(std::string p_handle) = 0;

	};

	class ResourceManager : public ResourceManagerInterface
	{
	public:
		friend class ModelImporter;

		ResourceManager();
		~ResourceManager();
		
		void Init(std::string p_workingDirectory, GameSharedContext* p_context );

		//Load functions
		Model*						LoadCollada(std::string p_path);
		Render::EffectInterface*	LoadEffect(std::string p_path);
		Render::TextureInterface*	LoadTexture(std::string p_path);

		//Get functions
		Model*							GetModel(std::string p_handle);
		Render::EffectInterface*		GetEffect(std::string p_handle);
		Render::TextureInterface*		GetTexture(std::string p_handle);
		Render::MeshInterface*			GetMesh(std::string p_handle);
		Physics::PhysicsMeshInterface*	GetPhysicsMesh(std::string p_handle);

		const std::string& ResolveStringFromTexture(Render::TextureInterface* p_texture);
		const std::string& ResolveStringFromEffect(Render::EffectInterface* p_effect);
		const std::string& ResolveStringFromModel(Model* p_model);

		const std::string& GetWorkingDirectory();

	private:
		//Resources
		std::map<std::string, Model*> m_models;
		std::map<std::string, std::shared_ptr<Render::MeshInterface>>			m_meshes;
		std::map<std::string, std::shared_ptr<Render::EffectInterface>>			m_effects;
		std::map<std::string, std::shared_ptr<Render::TextureInterface>>		m_textures;
		std::map<std::string, std::shared_ptr<Physics::PhysicsMeshInterface>>	m_physicMeshes;

		//Importers
		std::shared_ptr<ModelImporter>		m_modelImporter;
		std::shared_ptr<EffectImporter>		m_effectImporter;
		std::shared_ptr<TextureImporter>	m_textureImporter;

		//Other
		GameSharedContext* m_context;
		std::string m_workingDirectory;
	};
}
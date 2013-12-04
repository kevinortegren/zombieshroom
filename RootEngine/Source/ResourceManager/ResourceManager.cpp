#include <RootEngine/Include/ResourceManager/ResourceManager.h>
#include <RootEngine/Physics/Include/RootPhysics.h>
#include <RootEngine/Include/GameSharedContext.h>
namespace RootEngine
{

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		auto modelitr = m_models.begin();
		for(; modelitr != m_models.end(); modelitr++)
		{
			delete (*modelitr).second;
			(*modelitr).second = nullptr;
		}
		m_models.clear();

		/*auto meshitr = m_meshes.begin();
		for(; meshitr != m_meshes.end(); meshitr++)v
		{
			delete (*meshitr).second;
			(*meshitr).second = nullptr;
		}
		m_meshes.clear();*/

		auto effectitr = m_effects.begin();
		for(; effectitr != m_effects.end(); effectitr++)
		{
			delete (*effectitr).second;
			(*effectitr).second = nullptr;
		}
		m_effects.clear();

	}

	void ResourceManager::Init(std::string p_workingDirectory, GameSharedContext* p_context)
	{
		m_context = p_context;
		m_workingDirectory	= p_workingDirectory;

		m_effectImporter = std::shared_ptr<EffectImporter>(new EffectImporter(m_context->m_renderer));
		m_modelImporter = std::shared_ptr<ModelImporter>(new ModelImporter(m_context->m_logger, m_context->m_renderer, this));
		m_textureImporter = std::shared_ptr<TextureImporter>(new TextureImporter(m_context->m_logger, m_context->m_renderer));
		
		m_effectImporter->SetWorkingDirectory(m_workingDirectory);
	}

	void ResourceManager::AddMesh( MESH_DESC p_desc )
	{

		Physics::PhysicsMeshInterface* pmesh	= m_context->m_physics->CreatePhysicsMesh();
		Render::MeshInterface* mesh				= m_context->m_renderer->CreateMesh();

		pmesh->Init(p_desc.verts, (int)p_desc.verts.size(), p_desc.indices, (int)p_desc.indices.size(), p_desc.faces);
		mesh->SetVertexBuffer(m_context->m_renderer->CreateBuffer());
		mesh->SetElementBuffer(m_context->m_renderer->CreateBuffer());
		mesh->SetVertexAttribute(m_context->m_renderer->CreateVertexAttributes());
		mesh->CreateIndexBuffer(&p_desc.indices[0], p_desc.indices.size());
		mesh->CreateVertexBuffer1P1N1UV(&p_desc.verts[0], p_desc.verts.size());
		mesh->SetPrimitiveType(p_desc.primitive);


		//Check if mesh init correctly
		m_meshes[p_desc.handle]			= mesh;
		m_physicMeshes[p_desc.handle]	= pmesh;

	}
	//////////////////////////////////////////////////////////////////////////
	//Load functions
	//////////////////////////////////////////////////////////////////////////
	Model* ResourceManager::LoadCollada(std::string p_path)
	{
		Model* model = m_modelImporter->LoadModel(m_workingDirectory + "Assets\\Models\\" + p_path + ".DAE");

		if(model)
		{
			m_models[p_path] = model;
			return m_models[p_path];
		}
		else
		{
			return nullptr;
		}
	}

	Render::EffectInterface* ResourceManager::LoadEffect(std::string p_path)
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Loading effect: %s", p_path.c_str());

		m_effectImporter->Load(m_workingDirectory + "Assets\\Effects\\" + p_path + ".effect");

		m_effects[p_path] = m_effectImporter->m_effect;

		if(m_effectImporter->m_effect)
			return m_effects[p_path];
		else
		{
			return nullptr;
		}
	}

	Render::TextureInterface* ResourceManager::LoadTexture( std::string p_path )
	{
		Render::TextureInterface* tex = m_textureImporter->LoadTexture(m_workingDirectory + "Assets\\Textures\\" + p_path + ".dds");

		if(tex)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Successfully loaded texture '%s'\n", p_path.c_str());
			m_textures[p_path] = tex;
			return m_textures[p_path];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Error loading texture '%s'\n", p_path.c_str());
			return nullptr;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Get functions
	//////////////////////////////////////////////////////////////////////////
	Model* ResourceManager::GetModel( std::string p_handle )
	{

		if(m_models.find(p_handle) != m_models.end())
		{
			return m_models[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Model has not been loaded: %s", p_handle.c_str());
			return nullptr;
		}
	}

	Render::EffectInterface* ResourceManager::GetEffect(std::string p_handle)
	{
		if(m_effects.find(p_handle) != m_effects.end())
		{
			return m_effects[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Effect has not been loaded: %s", p_handle.c_str());
			return nullptr;
		}
	}

	Render::TextureInterface* ResourceManager::GetTexture( std::string p_handle )
	{
		if(m_textures.find(p_handle) != m_textures.end())
		{
			return m_textures[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Texture has not been loaded: %s", p_handle.c_str());
			return nullptr;
		}
	}

	Render::MeshInterface* ResourceManager::GetMesh( std::string p_handle )
	{
		if(m_meshes.find(p_handle) != m_meshes.end())
		{
			return m_meshes[p_handle];
		}
		else
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::WARNING, "Mesh has not been loaded: %s", p_handle.c_str());
			return nullptr;
		}
	}

	Physics::PhysicsMeshInterface* ResourceManager::GetPhysicsMesh( std::string p_handle )
	{
		return nullptr;
	}

}
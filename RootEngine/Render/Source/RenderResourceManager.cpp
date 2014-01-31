#include <RootEngine/Render/Include/RenderResourceManager.h>
#include <RenderExtern.h>

namespace Render
{
	RenderResourceManager::RenderResourceManager()
	{
		m_buffers.reserve(10000);
		m_textures.reserve(1000);
		m_materials.reserve(10000);
		m_meshes.reserve(10000);
		m_vaos.reserve(10000);
		m_effects.reserve(1000);
	}

	RenderResourceManager::~RenderResourceManager()
	{

	}


	void RenderResourceManager::PrintResourceUsage()
	{
		//TODO: Call this from profiler overlay.

		int bufferSize = 0;
		for(auto itr = m_buffers.begin(); itr != m_buffers.end(); ++itr)
		{
			bufferSize += (*itr)->GetBufferSize();
		}

		bufferSize /= 1000;

		g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Buffer memory usage: %d kb", bufferSize);

		int textureSize = 0;
		for(auto itr = m_textures.begin(); itr != m_textures.end(); ++itr)
		{
			// TODO: Add mipmaps to the calculation.
			textureSize += ((*itr)->GetWidth() * (*itr)->GetHeight() * (*itr)->GetBytesPerPixel()) / (*itr)->GetCompressRatio();			
		}

		textureSize /= 1000;

		g_context.m_logger->LogText(LogTag::RENDER, LogLevel::DEBUG_PRINT, "Texture memory usage: %d kb", textureSize);

	}

	BufferInterface* RenderResourceManager::CreateBuffer(GLenum p_type)
	{ 
		Buffer* buffer = new Buffer(p_type);
		m_buffers.push_back(buffer);
		return buffer;
	}

	void RenderResourceManager::ReleaseBuffer(BufferInterface* p_buffer)
	{
		auto itr = std::find(m_buffers.begin(), m_buffers.end(), p_buffer);
		m_buffers.erase(itr);
	}

	TextureInterface* RenderResourceManager::CreateTexture()
	{
		Texture* texture = new Texture();
		m_textures.push_back(texture);
		return texture;
	}

	void RenderResourceManager::ReleaseTexture(TextureInterface* p_texture)
	{
		auto itr = std::find(m_textures.begin(), m_textures.end(), p_texture);
		m_textures.erase(itr);
	}

	Material* RenderResourceManager::CreateMaterial()
	{
		Material mat = Material(m_materials.size());
		m_materials.push_back(std::move(mat));
		return &m_materials[m_materials.size()-1];
	}

	VertexAttributesInterface* RenderResourceManager::CreateVertexAttributes()
	{ 
		VertexAttributes* vao = new VertexAttributes();
		m_vaos.push_back(vao);
		return vao;
	}

	MeshInterface* RenderResourceManager::CreateMesh() 
	{ 
		Mesh* mesh = new Mesh();
		m_meshes.push_back(mesh);
		return mesh;
	}

	EffectInterface* RenderResourceManager::CreateEffect() 
	{ 
		Effect* effect = new Effect();
		m_effects.push_back(effect);
		return effect;
	}
}
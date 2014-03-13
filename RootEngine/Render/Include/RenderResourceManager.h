#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>

namespace Render
{
	namespace RenderResources
	{
		enum RenderResources
		{
			RR_BUFFER,
			RR_TEXTURE,
			RR_MATERIAL,
			RR_VAO,
			RR_MESH,
			RR_EFFECT,
			RR_ALL = (RR_BUFFER | RR_TEXTURE | RR_MATERIAL | RR_VAO | RR_MESH | RR_EFFECT)
		};
	}

	class RenderResourceManager
	{
	public:

		RenderResourceManager();
		~RenderResourceManager();

		void PrintResourceUsage(int& p_bufferUsage, int& p_textureUsage, int& p_numBuffers, int& p_numTextures);

		// Allocate GPU buffer.
		BufferInterface* CreateBuffer(GLenum p_type);
		void ReleaseBuffer(BufferInterface* p_buffer);

		// Allocate GPU texture.
		TextureInterface* CreateTexture();
		void ReleaseTexture(TextureInterface* p_texture);

		// Create state objects.
		Material* CreateMaterial(const std::string& p_name);
		VertexAttributesInterface* CreateVertexAttributes();
		MeshInterface* CreateMesh();
		EffectInterface* CreateEffect();

		std::string GetStringFromMaterial(Material* p_material);

		void RemoveMesh(MeshInterface* p_mesh);
		void RemoveVAO(VertexAttributesInterface* p_vao);

		void Clean(unsigned p_cleanFlag);

	private:

		std::map<std::string, int> m_materialNameMap;

		std::vector<Render::BufferInterface*> m_buffers;
		std::vector<Render::TextureInterface*> m_textures;
		std::vector<Render::Material> m_materials;
		std::vector<Render::VertexAttributesInterface*> m_vaos;
		std::vector<Render::EffectInterface*> m_effects;
		std::vector<Render::MeshInterface*> m_meshes;
	};
}
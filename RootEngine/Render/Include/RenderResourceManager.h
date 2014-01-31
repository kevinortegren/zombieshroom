#pragma once

#include <RootEngine/Render/Include/Buffer.h>
#include <RootEngine/Render/Include/Effect.h>
#include <RootEngine/Render/Include/VertexAttributes.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>

namespace Render
{
	class RenderResourceManager
	{
	public:

		RenderResourceManager();
		~RenderResourceManager();

		void PrintResourceUsage();

		// Allocate GPU buffer.
		BufferInterface* CreateBuffer(GLenum p_type);
		void ReleaseBuffer(BufferInterface* p_buffer);

		// Allocate GPU texture.
		TextureInterface* CreateTexture();
		void ReleaseTexture(TextureInterface* p_texture);

		// Create state objects.
		Material* CreateMaterial();
		VertexAttributesInterface* CreateVertexAttributes();
		MeshInterface* CreateMesh();
		EffectInterface* CreateEffect();

	private:

		std::vector<Render::BufferInterface*> m_buffers;
		std::vector<Render::TextureInterface*> m_textures;
		std::vector<Render::Material> m_materials;
		std::vector<Render::VertexAttributesInterface*> m_vaos;
		std::vector<Render::EffectInterface*> m_effects;
		std::vector<Render::MeshInterface*> m_meshes;
	};
}
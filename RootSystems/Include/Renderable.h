#pragma once

#include <Utility\ECS\Include\Component.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>

#include <memory>

namespace RootForce
{
	struct Renderable : public ECS::Component<Renderable>
	{
		Render::MeshInterface* m_mesh;
		Render::Material m_material;
	};
}
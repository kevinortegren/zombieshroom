#pragma once

#include <ECS/Component.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>

#include <memory>

namespace RootForce
{
	struct Renderable : public ECS::Component<Renderable>
	{
		Renderable(){m_mesh = nullptr;}
		Render::MeshInterface* m_mesh;
		Render::Material m_material;
	};
}
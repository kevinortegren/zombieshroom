#pragma once

#include <ECS/Component.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <memory>

namespace RootForce
{
	struct Renderable : public ECS::Component<Renderable>
	{
		std::shared_ptr<Render::Mesh> m_mesh;
	};
}
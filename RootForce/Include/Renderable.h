#pragma once

#include <Utility\ECS\Include\Component.h>
#include <RootEngine/Render/Include/Mesh.h>
#include <RootEngine/Render/Include/Material.h>
#include <RootEngine/Include/ModelImporter.h>

#include <memory>

namespace RootForce
{
	struct Renderable : public ECS::Component<Renderable>
	{
		Renderable()
			: m_model(nullptr) {}

		RootEngine::Model* m_model;
		Render::Material m_material;
	};
}
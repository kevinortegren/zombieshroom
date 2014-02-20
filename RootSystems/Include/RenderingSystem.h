#pragma once

#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Include/ModelImporter.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <Utility/ECS/Include/World.h>
#include <RootSystems/Include/Transform.h>

#include <RootEngine/Include/GameSharedContext.h>
extern RootEngine::GameSharedContext g_engineContext;

namespace RootForce
{
	namespace RenderPass
	{
		enum RenderPass
		{
			RENDERPASS_DEFAULT,
			RENDERPASS_SKYBOX,
			RENDERPASS_PARTICLES,
			RENDERPASS_WATER
		};
	}

	struct Renderable : public ECS::Component<Renderable>
	{
		Renderable()
			: m_model(nullptr), m_material(nullptr), m_pass(0), m_renderFlags(1) {}
		~Renderable();

		RootEngine::Model* m_model;
		Render::Material* m_material;
		std::map<Render::Semantic::Semantic, void*> m_params;
		unsigned m_pass;
		unsigned m_renderFlags;
	};

	struct MatrixData
	{
		glm::mat4x4 m_model;
		glm::mat4x4 m_normal;
	};

	struct RenderingSystem : public ECS::EntitySystem
	{
		RenderingSystem(ECS::World* p_world)
			: ECS::EntitySystem(p_world)
		{
			SetUsage<Renderable>();
			SetUsage<Transform>();
			m_renderer = nullptr;
			m_logger = nullptr;
		}
		void Init();
		void Begin();
		void ProcessEntity(ECS::Entity* p_entity);
		void End();
		void SetRendererInterface(Render::RendererInterface* p_renderer);
		void SetLoggingInterface(Logging::LoggingInterface* p_logger);

		ECS::ComponentMapper<Renderable> m_renderables;
		ECS::ComponentMapper<Transform> m_transforms;

		std::map<ECS::Entity*, MatrixData> m_matrices;

	private:
		Render::RendererInterface* m_renderer;
		Logging::LoggingInterface* m_logger;
	};
}
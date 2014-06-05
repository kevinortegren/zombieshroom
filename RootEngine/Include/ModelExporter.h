#pragma once

#ifndef COMPILE_LEVEL_EDITOR
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/Exporter.hpp>      // C++ exporter interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#endif

#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <RootEngine/Render/Include/Vertex.h>
#include <RootEngine/Include/Logging/Logging.h>
#include <RootEngine/Render/Include/Mesh.h>

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace RootEngine
{
	struct GameSharedContext;

#ifndef COMPILE_LEVEL_EDITOR
	class ModelExporter
	{
	public:
		
		ModelExporter(GameSharedContext* p_context);
		~ModelExporter();

		bool ExportMesh(Render::MeshInterface* p_mesh);
	private:

		GameSharedContext* m_context;
	};
#endif
}



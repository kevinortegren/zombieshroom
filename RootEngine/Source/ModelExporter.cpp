#include <RootEngine/Include/ModelExporter.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>
#include <iostream>
#include <fstream>

#ifndef COMPILE_LEVEL_EDITOR

namespace RootEngine
{
	ModelExporter::ModelExporter(GameSharedContext* p_context)
		: m_context(p_context)
	{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::INIT_PRINT, "Model exporter initialized!");
	}

	ModelExporter::~ModelExporter()
	{
		
	}

	bool ModelExporter::ExportMesh( Render::MeshInterface* p_mesh, const std::string& p_path, const std::string& p_fileName )
	{
		if(p_mesh == nullptr)
			return false;

		if(p_mesh->GetVertexAttribute() == nullptr)
			return false;
		
		if(p_mesh->GetVertexBuffer() == nullptr)
			return false;

		unsigned numberOfAttribs = p_mesh->GetVertexAttribute()->GetNumAttributes();
		unsigned numElements = p_mesh->GetVertexBuffer()->GetNumElements();

		if(numberOfAttribs != 3) 
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Number of attribs is %d", numberOfAttribs);
			return false;
		}

		Render::Vertex1P1N1UV* vertexData = static_cast<Render::Vertex1P1N1UV*>(p_mesh->GetVertexBuffer()->MapBuffer(GL_READ_WRITE));
		
		if(vertexData == nullptr)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not map vertex buffer!");
			return false;
		}
		
		//Hand crafted COLLADA exporter
		std::ofstream myfile;
		myfile.open (p_path + "Assets/Models/" + p_fileName + ".dae");
		myfile << "<?xml version=\"1.0\"?>\n<COLLADA xmlns=\\\"http:/www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n<asset>\n<up_axis>Y_UP</up_axis>\n</asset>\n<library_geometries>\n<geometry id=\"Terrain\" name=\"Terrain\">\n<mesh>\n<source id=\"Terrain-positions\" name=\"Terrain-positions\">\n<float_array id=\"Terrain-positions-array\" count=\"";
		myfile << numElements*3 << "\">";

		for (unsigned i = 0; i < numElements; ++i)
		{
			myfile << vertexData[i].m_pos.x << " " << vertexData[i].m_pos.y << " " << vertexData[i].m_pos.z << " ";
		}
		
		myfile << "</float_array>\n<technique_common>\n<accessor count=\"";
		myfile << numElements;
		myfile << "\" source=\"#Terrain-positions-array\" stride=\"3\">\n<param name=\"X\" type=\"float\"/>\n<param name=\"Y\" type=\"float\"/>\n<param name=\"Z\" type=\"float\"/>\n</accessor>\n</technique_common>\n</source>\n<source id=\"Terrain-normals\" name=\"Terrain-normals\">\n<float_array id=\"Terrain-normals-array\" count=\"";
		myfile << numElements*3 << "\">";

		for (unsigned i = 0; i < numElements; ++i)
		{
			myfile << vertexData[i].m_normal.x << " " << vertexData[i].m_normal.y << " " << vertexData[i].m_normal.z << " ";
		}

		myfile << "</float_array>\n<technique_common>\n<accessor count=\"";
		myfile << numElements;
		myfile << "\" source=\"#Terrain-normals-array\" stride=\"3\">\n<param name=\"X\" type=\"float\"/>\n<param name=\"Y\" type=\"float\"/>\n<param name=\"Z\" type=\"float\"/>\n</accessor>\n</technique_common>\n</source>\n<source id=\"Terrain-map1\" name=\"Terrain-map1\">\n<float_array id=\"Terrain-map1-array\" count=\"";
		myfile << numElements*2 << "\">";

		for (unsigned i = 0; i < numElements; ++i)
		{
			myfile << vertexData[i].m_UV.x << " " << vertexData[i].m_UV.y << " ";
		}

		//Done with vertex buffer, unmap it!
		if(p_mesh->GetVertexBuffer()->UnmapBuffer() == false)
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not unmap vertex buffer!");

		//Map index buffer
		unsigned* elemetData = static_cast<unsigned*>(p_mesh->GetElementBuffer()->MapBuffer(GL_READ_WRITE));

		if(elemetData == nullptr)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not map element buffer!");
			return false;
		}

		unsigned numIndices = p_mesh->GetElementBuffer()->GetNumElements();
		unsigned numTriangles = numIndices / 3;

		myfile << "</float_array>\n<technique_common>\n<accessor count=\"";
		myfile << numElements;
		myfile << "\" source=\"#Terrain-map1-array\" stride=\"2\">\n<param name=\"S\" type=\"float\"/>\n<param name=\"T\" type=\"float\"/>\n</accessor>\n</technique_common>\n</source>\n<vertices id=\"Terrain-vertices\" name=\"Terrain-vertices\">\n<input semantic=\"POSITION\" source=\"#Terrain-positions\"/>\n</vertices>\n<triangles count=\"";
		myfile <<  numTriangles;
		myfile << "\" material=\"initialShadingGroup\">\n<input offset=\"0\" semantic=\"VERTEX\" source=\"#Terrain-vertices\"/>\n<input offset=\"0\" semantic=\"NORMAL\" source=\"#Terrain-normals\"/>\n<input offset=\"0\" semantic=\"TEXCOORD\" source=\"#Terrain-map1\" set=\"0\"/>\n<p>";
		
		for (unsigned i = 0; i < numIndices; ++i)
		{
			myfile << elemetData[i] << " ";
		}

		myfile << "</p>\n</triangles>\n</mesh>\n</geometry>\n</library_geometries>\n<library_visual_scenes>\n<visual_scene id=\"SceneNode\" name=\"Terrain\">\n<node id=\"Root\" name=\"Root\">\n<instance_geometry url=\"#Terrain\"/>\n</node>\n</visual_scene>\n</library_visual_scenes>\n<scene>\n<instance_visual_scene url=\"#SceneNode\"/>\n</scene>\n</COLLADA>";
		myfile.close();

		//Done with elemet buffer, unmap it!
		if(p_mesh->GetElementBuffer()->UnmapBuffer() == false)
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not unmap element buffer!");

		return true;
	}

}

#endif

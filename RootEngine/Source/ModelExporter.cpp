#include <RootEngine/Include/ModelExporter.h>
#include <RootEngine/Include/GameSharedContext.h>
#include <RootEngine/Physics/Include/PhysicsMesh.h>

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

	bool ModelExporter::ExportMesh( Render::MeshInterface* p_mesh )
	{
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
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not map buffer!");
			return false;
		}
	

	/*	unsigned numberOfExportFormats = exporter.GetExportFormatCount();
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "#Export formats %d", numberOfExportFormats); 

		for (unsigned i = 0; i < numberOfExportFormats; ++i)
		{
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "___________EXPORTFORMAT___________"); 
			const aiExportFormatDesc* exportFormat = exporter.GetExportFormatDescription(i);
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "%s", exportFormat->description); 
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "%s", exportFormat->id); 
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "%s", exportFormat->fileExtension); 
		}*/
		
		/*	for (unsigned i = 0; i < numElements; ++i)
		{
		m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::DEBUG_PRINT, "Model UV: x %f  y %f", vertexData[i].m_UV.x, vertexData[i].m_UV.y); 
		}*/


		if(p_mesh->GetVertexBuffer()->UnmapBuffer() == false)
			m_context->m_logger->LogText(LogTag::RESOURCE, LogLevel::NON_FATAL_ERROR, "Model exporter could not unmap buffer!");

		return true;
	}

}

#endif

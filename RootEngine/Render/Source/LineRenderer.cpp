#include <RootEngine/Render/Include/LineRenderer.h>
#include <RootEngine/Render/Include/Renderer.h>
#include <RootEngine/Render/Include/RenderExtern.h>
#include <RootEngine/Include/ResourceManager/ResourceManager.h>

namespace Render
{
	void LineRenderer::Init(GLRenderer* p_renderer)
	{
		m_lineMesh.SetVertexBuffer(p_renderer->CreateBuffer(GL_ARRAY_BUFFER));
		m_lineMesh.SetVertexAttribute(p_renderer->CreateVertexAttributes());
		m_lineMesh.SetPrimitiveType(GL_LINES);
		m_lineMesh.CreateVertexBuffer1P1C(0, 0);

		auto m_debugEffect = g_context.m_resourceManager->LoadEffect("Color");
		m_debugTech = m_debugEffect->GetTechniques()[0];
	}

	void LineRenderer::AddLine(Line& line)
	{
		m_lines.push_back(line);
	}

	void LineRenderer::RenderLines()
	{
		Vertex1P1C* lineVertices = new Vertex1P1C[m_lines.size()*2];
		for(unsigned int i = 0; i < m_lines.size(); i++)
		{
			lineVertices[i*2].m_pos = m_lines[i].m_fromPoint;
			lineVertices[i*2].m_color = m_lines[i].m_color;
			lineVertices[i*2+1].m_pos = m_lines[i].m_toPoint;
			lineVertices[i*2+1].m_color = m_lines[i].m_color;
		}

		m_debugTech->GetPrograms()[0]->Apply();

		m_lineMesh.GetVertexBuffer()->BufferData(m_lines.size()*2, sizeof(Vertex1P1C), lineVertices);
		m_lineMesh.Bind();
		m_lineMesh.Draw();
		m_lineMesh.Unbind();

		delete [] lineVertices;
		m_lines.clear();
	}

}




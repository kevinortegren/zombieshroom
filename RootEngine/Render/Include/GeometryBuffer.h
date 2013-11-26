#pragma once

#include <RootEngine/Render/Include/Texture.h>

namespace Render
{
	class GeometryBuffer
	{
	public:
		~GeometryBuffer();
		void Init(int p_width, int p_height);
		void Bind();
		void Unbind();
		void Read();

	private:
		GLuint m_fbo;
		Render::Texture m_color;
	};
}
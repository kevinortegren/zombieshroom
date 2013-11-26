#pragma once

#include <GL/glew.h>
#include <gli/gli.hpp>

namespace Render
{
	struct Texture
	{
		bool Load(const std::string& filepath);
		void Enable(GLuint shaderProgram, unsigned int slot);
		glm::vec2 GetInverseTextureSize() const;
		glm::vec2 GetSize() const;
	
		// TODO: Share this.
		GLuint m_textureHandle;
		int m_textureWidth;
		int m_textureHeight;
	};
}

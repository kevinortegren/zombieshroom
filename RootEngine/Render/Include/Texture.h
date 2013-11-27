#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace Render
{
	struct TextureInterface
	{
		virtual bool Load(const std::string& filepath) = 0;
		virtual void Enable() = 0;
		virtual unsigned int GetID() = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual glm::vec2 GetSize() = 0;
	};

	struct Texture : public TextureInterface
	{
		bool Load(const std::string& filepath);
		void Enable();
		unsigned int GetID();
		unsigned int GetWidth();
		unsigned int GetHeight();
		glm::vec2 GetSize();

		glm::vec2 GetInverseTextureSize() const;
		
	
		// TODO: Share this.
		GLuint m_textureHandle;
		int m_textureWidth;
		int m_textureHeight;
	};
}

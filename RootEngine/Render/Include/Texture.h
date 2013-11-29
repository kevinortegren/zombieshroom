#pragma once

#include <GL/glew.h>
#include <gli/gli.hpp>
#include <string>

namespace Render
{
	struct TextureInterface
	{
		virtual bool Load(const std::string& filepath) = 0;
		virtual void Enable(unsigned int slot) = 0;
		virtual unsigned int GetID() = 0;
		virtual unsigned int GetWidth() = 0;
		virtual unsigned int GetHeight() = 0;
		virtual glm::vec2 GetSize() = 0;
		virtual glm::vec2 GetInverseTextureSize() const = 0;
		virtual GLuint GetHandle() const = 0;
	};

	class Texture : public TextureInterface
	{
	public:
		~Texture();

		bool Load(const std::string& filepath);
		void Enable(unsigned int slot);

		unsigned int GetID();
		unsigned int GetWidth();
		unsigned int GetHeight();
		GLuint GetHandle() const;

		glm::vec2 GetSize();
		glm::vec2 GetInverseTextureSize() const;
		
	private:
		GLuint m_textureHandle;
		int m_textureWidth;
		int m_textureHeight;
	};
}

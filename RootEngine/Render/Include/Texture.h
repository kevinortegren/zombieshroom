#pragma once

#include <GL/glew.h>
#include <gli/gli.hpp>

namespace Render
{
	class TextureInterface
	{
		virtual bool Load(int p_width, int p_height) = 0;
		virtual bool Load(const std::string& filepath) = 0;

		virtual void Enable(unsigned int slot) = 0;

		virtual glm::vec2 GetInverseTextureSize() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual GLuint GetHandle() const = 0;
	};

	class Texture : public TextureInterface
	{
	public:
		~Texture();

		bool Load(int p_width, int p_height);
		bool Load(const std::string& filepath);

		void Enable(unsigned int slot);
		glm::vec2 GetInverseTextureSize() const;
		glm::vec2 GetSize() const;
		GLuint GetHandle() const;
	
	private:
		GLuint m_textureHandle;
		int m_textureWidth;
		int m_textureHeight;
	};
}

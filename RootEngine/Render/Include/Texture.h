#pragma once

#include <GL/glew.h>
#include <gli/gli.hpp>
#include <string>

namespace Render
{
	namespace TextureType
	{
		enum TextureType
		{
			TEXTURE_2D,
			TEXTURE_CUBEMAP
		};
	}

	namespace TextureFormat
	{
		enum TextureFormat
		{
			TEXTURE_RGBA,
			TEXTURE_BGRA,
			TEXTURE_RGB,
			TEXTURE_BGR,
			TEXTURE_DEPTH_COMPONENT
		};
	}

	struct TextureInterface
	{
		virtual bool Load(const std::string filepath) = 0;
		virtual bool LoadCubeMap(const std::string& filepath) = 0;

		virtual void Bind(unsigned int slot) = 0;
		virtual void Unbind(unsigned int slot) = 0;
		
		virtual void CreateEmptyTexture(int p_width, int p_height, int p_format) = 0;
		virtual void SetParameter(int p_name, int p_parameter) = 0;
		virtual void BufferData(void* pixels) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual glm::vec2 GetSize() const = 0;
		virtual glm::vec2 GetInverseTextureSize() const = 0;
		virtual GLuint GetHandle() const = 0;
		virtual GLenum GetTarget() const = 0;
	};

	class Texture : public TextureInterface
	{
	public:
		friend class GLRenderer;
		
		bool Load(const std::string filepath);
		bool LoadCubeMap(const std::string& filepath);

		void Bind(unsigned int p_slot);
		void Unbind(unsigned int slot);

		void CreateEmptyTexture(int p_width, int p_height, int p_format);
		void SetParameter(int p_name, int p_parameter);	
		void BufferData(void* pixels);

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		GLuint GetHandle() const;
		GLenum GetTarget() const;

		glm::vec2 GetSize() const;
		glm::vec2 GetInverseTextureSize() const;
		
	private:
		Texture();
		~Texture();

		GLuint m_textureHandle;
		GLenum m_target;
		int m_textureWidth;
		int m_textureHeight;
		GLenum m_textureFormat;
		GLenum m_textureType;
	};
}

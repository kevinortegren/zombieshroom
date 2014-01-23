#include <RootEngine/Render/Include/Texture.h>
#include <gli/gli.hpp>

#include <RootEngine/Render/Include/RenderExtern.h>

namespace Render
{
	Texture::Texture()
		: m_textureHandle(0), m_target(0), m_textureWidth(0), m_textureHeight(0),
		m_textureFormat(0), m_textureType(0) 
	{
		glGenTextures(1, &m_textureHandle);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureHandle);
	}

	bool Texture::Load(const std::string filepath)
	{
		gli::texture2D texture(gli::loadStorageDDS(filepath));

		if(texture.empty())
		{
			return false;
		}

		m_target = GL_TEXTURE_2D;

		m_textureWidth = texture.dimensions().x;
		m_textureHeight = texture.dimensions().y;

		glBindTexture(m_target, m_textureHandle);

		SetParameter(GL_TEXTURE_BASE_LEVEL, 0);
		SetParameter(GL_TEXTURE_MAX_LEVEL, GLint(texture.levels() - 1));; 
		SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexStorage2D(m_target,
			GLint(texture.levels()),
			GLenum(gli::internal_format(texture.format())),
			GLsizei(texture.dimensions().x),
			GLsizei(texture.dimensions().y));

		if(gli::is_compressed(texture.format()))
		{
			for(gli::texture2D::size_type Level = 0; Level < texture.levels(); ++Level)
			{
				 glCompressedTexSubImage2D(m_target,
					GLint(Level),
					0, 0,
					GLsizei(texture[Level].dimensions().x),
					GLsizei(texture[Level].dimensions().y),
					GLenum(gli::internal_format(texture.format())),
					GLsizei(texture[Level].size()),
					texture[Level].data()); 
			}
		}
		else
		{
			for(gli::texture2D::size_type Level = 0; Level < texture.levels(); ++Level)
			{
				 glTexSubImage2D(m_target,
					GLint(Level),
					0, 0,
					GLsizei(texture[Level].dimensions().x),
					GLsizei(texture[Level].dimensions().y),
					GLenum(gli::external_format(texture.format())),
					GLenum(gli::type_format(texture.format())),
					texture[Level].data()); 
			}
		} 

		return true;
	}

	bool Texture::LoadCubeMap(const std::string& filepath)
	{
		gli::textureCube cube(gli::loadStorageDDS(filepath));
		assert(!cube.empty());

		m_target = GL_TEXTURE_CUBE_MAP;

		glBindTexture(m_target, m_textureHandle);

		glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, GLint(cube.levels() - 1)); 

		GLenum faces[] = { 
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		};

		for(gli::texture2D::size_type i = 0; i < cube.faces(); i++)  {

			 glCompressedTexImage2D(faces[i],
				0,
				GLenum(gli::internal_format(cube.format())),
				cube[i].dimensions().x,
				cube[i].dimensions().y,
				0,
				GLsizei(cube[i].size()),
				cube[i].data()); 
		}

		return true;
	}

	void Texture::Bind(unsigned int p_slot)
	{
		glActiveTexture(GL_TEXTURE0 + p_slot);
		glBindTexture(m_target, m_textureHandle);
	}

	void Texture::Unbind(unsigned int p_slot)
	{
		glActiveTexture(GL_TEXTURE0 + p_slot);
		glBindTexture(m_target, 0);
	}



	void Texture::CreateEmptyTexture(int p_width, int p_height, int p_format)
	{
		m_target = GL_TEXTURE_2D;
		m_textureWidth = p_width;
		m_textureHeight = p_height;
		
		glBindTexture(m_target, m_textureHandle);
		if(p_format == TextureFormat::TEXTURE_RGBA || p_format == TextureFormat::TEXTURE_BGRA)
		{
			m_textureType = GL_UNSIGNED_BYTE;

			if(p_format == TextureFormat::TEXTURE_RGBA)
				m_textureFormat = GL_RGBA;
			else
				m_textureFormat = GL_BGRA;

			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexImage2D(m_target, 0, GL_RGBA, p_width, p_height, 0, m_textureFormat, m_textureType, NULL);
		}
		else if(p_format == TextureFormat::TEXTURE_RGB || p_format == TextureFormat::TEXTURE_BGR)
		{
			m_textureType = GL_UNSIGNED_BYTE;

			if(p_format == TextureFormat::TEXTURE_RGB)
				m_textureFormat = GL_RGB;
			else
				m_textureFormat = GL_BGR;
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D(m_target, 0, GL_RGB, p_width, p_height, 0, m_textureFormat, m_textureType, NULL);
		}
		else if(p_format == TextureFormat::TEXTURE_DEPTH_COMPONENT)
		{
			m_textureType = GL_FLOAT;
			m_textureFormat = GL_DEPTH_COMPONENT;

			glTexImage2D(m_target, 0, GL_DEPTH_COMPONENT32, p_width, p_height, 0, m_textureFormat, m_textureType, NULL);
		}
		else
		{
			g_context.m_logger->LogText(LogTag::RENDER, LogLevel::WARNING, "Tried to create empty texture with unsupported format!");
		}
	}

	void Texture::SetParameter(int p_name, int p_parameter)
	{
		glBindTexture(m_target, m_textureHandle);

		glTexParameteri(m_target, p_name, p_parameter);
	}

	void Texture::BufferData(void* pixels)
	{
		glBindTexture(m_target, m_textureHandle);

		glTexSubImage2D(m_target,
			0,
			0, 0,
			m_textureWidth,
			m_textureHeight,
			m_textureFormat,
			m_textureType,
			pixels); 

		glBindTexture(m_target, 0);
	}

	unsigned int Texture::GetWidth() const
	{
		return m_textureWidth;
	}

	unsigned int Texture::GetHeight() const
	{
		return m_textureHeight;
	}

	glm::vec2 Texture::GetSize() const
	{
		return glm::vec2(m_textureWidth, m_textureHeight);
	}

	GLuint Texture::GetHandle() const
	{
		return m_textureHandle;
	}

	GLenum Texture::GetTarget() const
	{
		return m_target;
	}

	glm::vec2 Texture::GetInverseTextureSize() const
	{
		return glm::vec2(1 / (float)m_textureWidth, 1 / (float)m_textureWidth);
	}
}


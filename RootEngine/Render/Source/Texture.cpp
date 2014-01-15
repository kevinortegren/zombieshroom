#include <RootEngine/Render/Include/Texture.h>
#include <gli/gli.hpp>

namespace Render
{
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

		glGenTextures(1, &m_textureHandle);
		glBindTexture(m_target, m_textureHandle);

		//glGenSamplers(1, &m_samplerHandle);
		//glBindSampler(0, m_samplerHandle);

		glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, GLint(texture.levels() - 1)); 

		glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		//glTexParameterf(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		//glTexParameterf(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

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

		glGenTextures(1, &m_textureHandle);
		glBindTexture(m_target, m_textureHandle);

		glTexParameteri(m_target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_target, GL_TEXTURE_MAX_LEVEL, GLint(cube.levels() - 1)); 

		//glTexParameterf(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		//glTexParameterf(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

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

	void Texture::Enable(unsigned int slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(m_target, m_textureHandle);
	}

	unsigned int Texture::GetID()
	{
		return m_textureHandle;
	}

	unsigned int Texture::GetWidth()
	{
		return m_textureWidth;
	}

	unsigned int Texture::GetHeight()
	{
		return m_textureHeight;
	}

	glm::vec2 Texture::GetSize()
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


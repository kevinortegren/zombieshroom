#include <RootEngine/Render/Include/Texture.h>

Render::Texture::~Texture()
{
	glDeleteTextures(1, &m_textureHandle);
}

bool Render::Texture::Load(int p_width, int p_height, GLenum p_format)
{

	return true;
}

bool Render::Texture::Load(const std::string& filepath)
{
	gli::texture2D texture(gli::loadStorageDDS(filepath));
	assert(!texture.empty());

	m_textureWidth = texture.dimensions().x;
	m_textureHeight = texture.dimensions().y;

	glGenTextures(1, &m_textureHandle);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(texture.levels() - 1)); 

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	 glTexStorage2D(GL_TEXTURE_2D,
		GLint(texture.levels()),
		GLenum(gli::internal_format(texture.format())),
		GLsizei(texture.dimensions().x),
		GLsizei(texture.dimensions().y)); 

	if(gli::is_compressed(texture.format()))
	{
		for(gli::texture2D::size_type Level = 0; Level < texture.levels(); ++Level)
		{
			 glCompressedTexSubImage2D(GL_TEXTURE_2D,
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
			 glTexSubImage2D(GL_TEXTURE_2D,
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

void Render::Texture::Enable(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}

glm::vec2 Render::Texture::GetInverseTextureSize() const
{
	return glm::vec2(1 / (float)m_textureWidth, 1 / (float)m_textureWidth);
}

glm::vec2 Render::Texture::GetSize() const
{
	return glm::vec2(m_textureWidth, m_textureHeight);
}

GLuint Render::Texture::GetHandle() const
{
	return m_textureHandle;
}


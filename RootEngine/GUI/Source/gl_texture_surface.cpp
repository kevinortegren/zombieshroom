// Author: princeofcode
// URL: https://gist.github.com/princeofcode/5133370 (02-12-2013)
#include "gl_texture_surface.h"
#include <iostream>

GLRAMTextureSurface::GLRAMTextureSurface(int width, int height) : texture_id_(0),
  buffer_(0), bpp_(4), rowspan_(0), width_(width), height_(height) {
  rowspan_ = width_ * bpp_;
  buffer_ = new unsigned char[rowspan_ * height_];
  needs_update_ = false;

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0,
               bpp_ == 3 ? GL_RGB : GL_BGRA, GL_UNSIGNED_BYTE, buffer_);
}

GLRAMTextureSurface::~GLRAMTextureSurface() {
  glDeleteTextures(1, &texture_id_);
  delete[] buffer_;
}

GLuint GLRAMTextureSurface::GetTexture() const {
  //const_cast<GLRAMTextureSurface*>(this)->UpdateTexture();
	
	const_cast<GLRAMTextureSurface*>(this)->m_needsUpdateMutex.lock();
	const_cast<GLRAMTextureSurface*>(this)->m_needsUpdateMutex.unlock();
  return texture_id_;
}

void GLRAMTextureSurface::Paint(unsigned char* src_buffer,
                    int src_row_span,
                    const Awesomium::Rect& src_rect,
                    const Awesomium::Rect& dest_rect) {
	m_needsUpdateMutex.lock();
  for (int row = 0; row < dest_rect.height; row++)
    memcpy(buffer_ + (row + dest_rect.y) * rowspan_ + (dest_rect.x * 4),
      src_buffer + (row + src_rect.y) * src_row_span + (src_rect.x * 4),
      dest_rect.width * 4);
	
  // If texture can be updated from here, do it
  // unless a full update is needed already
  if(!needs_update_
	  && src_rect.x == 0 && src_rect.y == 0
	  && src_rect.width == dest_rect.width
	  && src_rect.height == dest_rect.height)
  {
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexSubImage2D(GL_TEXTURE_2D, 0, dest_rect.x, dest_rect.y, dest_rect.width, dest_rect.height,
		bpp_ == 3 ? GL_RGB : GL_BGRA, GL_UNSIGNED_BYTE,
		src_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
  }
  else
  {
	needs_update_ = true;
  }
	m_needsUpdateMutex.unlock();
}

void GLRAMTextureSurface::Scroll(int dx,
                    int dy,
                    const Awesomium::Rect& clip_rect) {
  if (abs(dx) >= clip_rect.width || abs(dy) >= clip_rect.height)
    return;

	m_needsUpdateMutex.lock();

  if (dx < 0 && dy == 0) {
    // Area shifted left by dx
    unsigned char* tempBuffer = new unsigned char[(clip_rect.width + dx) * 4];

    for (int i = 0; i < clip_rect.height; i++) {
      memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * rowspan_ +
        (clip_rect.x - dx) * 4, (clip_rect.width + dx) * 4);
      memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x) * 4,
        tempBuffer, (clip_rect.width + dx) * 4);
    }

    delete[] tempBuffer;
  } else if (dx > 0 && dy == 0) {
    // Area shifted right by dx
    unsigned char* tempBuffer = new unsigned char[(clip_rect.width - dx) * 4];

    for (int i = 0; i < clip_rect.height; i++) {
      memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * rowspan_ +
        (clip_rect.x) * 4, (clip_rect.width - dx) * 4);
      memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x + dx) * 4,
        tempBuffer, (clip_rect.width - dx) * 4);
    }

    delete[] tempBuffer;
  } else if (dy < 0 && dx == 0) {
    // Area shifted down by dy
    for (int i = 0; i < clip_rect.height + dy ; i++)
      memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x * 4),
      buffer_ + (i + clip_rect.y - dy) * rowspan_ + (clip_rect.x * 4),
      clip_rect.width * 4);
  } else if (dy > 0 && dx == 0) {
    // Area shifted up by dy
    for (int i = clip_rect.height - 1; i >= dy; i--)
      memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x * 4),
      buffer_ + (i + clip_rect.y - dy) * rowspan_ + (clip_rect.x * 4),
      clip_rect.width * 4);
  }

  needs_update_ = true;
	m_needsUpdateMutex.unlock();
}

void GLRAMTextureSurface::UpdateTexture()
{
	m_needsUpdateMutex.lock();
	if (needs_update_)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_,
			bpp_ == 3 ? GL_RGB : GL_BGRA, GL_UNSIGNED_BYTE,
			buffer_);
		glBindTexture(GL_TEXTURE_2D, 0);
		needs_update_ = false;
	}
	m_needsUpdateMutex.unlock();
}


GLTextureSurfaceFactory::GLTextureSurfaceFactory() {
}

GLTextureSurfaceFactory::~GLTextureSurfaceFactory() {
}

Awesomium::Surface* GLTextureSurfaceFactory::CreateSurface(Awesomium::WebView* view,
                                          int width,
                                          int height) {
  return new GLRAMTextureSurface(width, height);
}

void GLTextureSurfaceFactory::DestroySurface(Awesomium::Surface* surface) {
  delete static_cast<GLRAMTextureSurface*>(surface);
}

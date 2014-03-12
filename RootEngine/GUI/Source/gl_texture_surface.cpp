#include <GL/glew.h>
#include "gl_texture_surface.h"
#include <iostream>

//GLuint GLTextureSurface::m_bufferTexture = 0;

GLTextureSurface::GLTextureSurface(int p_width, int p_height) : 
	m_width(p_width), m_height(p_height)
{
	// Use the int-division to round downwards
	int tilesX = (p_width-1)/TILE_SIZE+1;
	int tilesY = (p_height-1)/TILE_SIZE+1;

	m_tiles.resize(tilesX);
	for(int x = 0; x < tilesX; x++)
	{
		m_tiles[x].resize(tilesY);
	}
}

GLTextureSurface::~GLTextureSurface()
{
}

std::vector<std::vector<SurfaceTile>>* GLTextureSurface::GetTiles() const
{
	return const_cast<std::vector<std::vector<SurfaceTile>>*>(&m_tiles);
}

void GLTextureSurface::Paint(unsigned char* p_buffer, int p_row_span, const Awesomium::Rect& p_src_rect, const Awesomium::Rect& p_dest_rect)
{
	for (int row = 0; row < p_dest_rect.height; ++row)
	{
		int tileY = (p_dest_rect.y + row)/TILE_SIZE;
		int tileRow = p_dest_rect.y + row - tileY*TILE_SIZE;
		int startX = p_dest_rect.x/TILE_SIZE;
		int endX = (p_dest_rect.x + p_dest_rect.width -1)/TILE_SIZE;

		for (int tileX = startX; tileX <= endX; ++tileX)
		{
			int tileRowStart = max(p_dest_rect.x - tileX*TILE_SIZE, 0);
			int tileRowEnd = max(min(p_dest_rect.x + p_dest_rect.width - tileX*TILE_SIZE, TILE_SIZE), 0);
			int srcStart = tileRowStart + tileX*TILE_SIZE - p_dest_rect.x + p_src_rect.x;
			SurfaceTile& tile = m_tiles[tileX][tileY];

			memcpy(tile.Buffer.data() + tileRow * tile.Rowspan + tileRowStart * tile.Bpp,
				p_buffer + (row + p_src_rect.y) * p_row_span + srcStart * tile.Bpp,
				(tileRowEnd - tileRowStart) * tile.Bpp);

			tile.NeedsUpdate = true;
		}
	}
}

void GLTextureSurface::Scroll(int dx, int dy, const Awesomium::Rect& clip_rect)
{
	if (abs(dx) >= clip_rect.width || abs(dy) >= clip_rect.height)
		return;

	//m_needsUpdateMutex.lock();

	//if (dx < 0 && dy == 0)
	//{
	//	// Area shifted left by dx
	//	unsigned char* tempBuffer = new unsigned char[(clip_rect.width + dx) * 4];

	//	for (int i = 0; i < clip_rect.height; i++)
	//	{
	//		memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * rowspan_ +
	//		(clip_rect.x - dx) * 4, (clip_rect.width + dx) * 4);
	//		memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x) * 4,
	//		tempBuffer, (clip_rect.width + dx) * 4);
	//	}

	//	delete[] tempBuffer;
	//}
	//else if (dx > 0 && dy == 0)
	//{
	//	// Area shifted right by dx
	//	unsigned char* tempBuffer = new unsigned char[(clip_rect.width - dx) * 4];

	//	for (int i = 0; i < clip_rect.height; i++)
	//	{
	//		memcpy(tempBuffer, buffer_ + (i + clip_rect.y) * rowspan_ +
	//		(clip_rect.x) * 4, (clip_rect.width - dx) * 4);
	//		memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x + dx) * 4,
	//		tempBuffer, (clip_rect.width - dx) * 4);
	//	}

	//	delete[] tempBuffer;
	//}
	//else if (dy < 0 && dx == 0)
	//{
	//	// Area shifted down by dy
	//	for (int i = 0; i < clip_rect.height + dy ; i++)
	//		memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x * 4),
	//		buffer_ + (i + clip_rect.y - dy) * rowspan_ + (clip_rect.x * 4),
	//		clip_rect.width * 4);
	//}
	//else if (dy > 0 && dx == 0)
	//{
	//	// Area shifted up by dy
	//	for (int i = clip_rect.height - 1; i >= dy; i--)
	//		memcpy(buffer_ + (i + clip_rect.y) * rowspan_ + (clip_rect.x * 4),
	//		buffer_ + (i + clip_rect.y - dy) * rowspan_ + (clip_rect.x * 4),
	//		clip_rect.width * 4);
	//}

	//needs_update_ = true;
	//m_needsUpdateMutex.unlock();
}

void GLTextureSurface::UpdateTexture()
{
	for(auto& row : m_tiles)
	{
		for(SurfaceTile& tile : row)
		{
			if(tile.FenceID > 0 && glClientWaitSync(tile.FenceID, GL_SYNC_FLUSH_COMMANDS_BIT, 0) != GL_TIMEOUT_EXPIRED)
			{
				glDeleteSync(tile.FenceID);
				tile.FenceID = 0;
				
				glBindTexture(GL_TEXTURE_2D, tile.Texture);
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, tile.PBO[tile.ActiveBuffer]);

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TILE_SIZE, TILE_SIZE,
					tile.Bpp == 3 ? GL_RGB : GL_BGRA, GL_UNSIGNED_BYTE, 0);
		
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, 0);
			}
			if (tile.NeedsUpdate)
			{
				glBindTexture(GL_TEXTURE_2D, tile.Texture);
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, tile.PBO[!tile.ActiveBuffer]);
				glBufferDataARB(GL_PIXEL_UNPACK_BUFFER, tile.Buffer.size(), 0, GL_STREAM_DRAW_ARB);

				GLubyte* dataPtr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY_ARB);
				if(dataPtr)
				{
					memcpy(dataPtr, tile.Buffer.data(), tile.Buffer.size());
				}
				glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER);
		
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER, 0);
				tile.NeedsUpdate = false;
				tile.ActiveBuffer = !tile.ActiveBuffer;

				tile.FenceID = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
			}
		}
		Sleep(1);
	}
}


GLTextureSurfaceFactory::GLTextureSurfaceFactory()
{
}

GLTextureSurfaceFactory::~GLTextureSurfaceFactory()
{
}

Awesomium::Surface* GLTextureSurfaceFactory::CreateSurface(Awesomium::WebView* view, int width, int height)
{
  return new GLTextureSurface(width, height);
}

void GLTextureSurfaceFactory::DestroySurface(Awesomium::Surface* surface)
{
  delete static_cast<GLTextureSurface*>(surface);
}

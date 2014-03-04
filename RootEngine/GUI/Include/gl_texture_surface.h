#pragma once

#include <Awesomium/Surface.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>

#define TILE_SIZE 256

//class GLTextureSurface : public Awesomium::Surface
//{
//public:
//	virtual void Paint(unsigned char* src_buffer, int src_row_span, const Awesomium::Rect& src_rect, const Awesomium::Rect& dest_rect) = 0;
//
//	virtual void Scroll(int dx, int dy, const Awesomium::Rect& clip_rect) = 0;
//
//	virtual GLuint GetTiles() const = 0;
//	virtual int width() const = 0;
//	virtual int height() const = 0;
//	virtual int size() const = 0;
//};

struct SurfaceTile
{
	std::vector<char> Buffer;
	GLuint Texture;
	bool NeedsUpdate;
	int Bpp, Rowspan;

	/*SurfaceTile()
		: Texture(0), NeedsUpdate(false), Bpp(0), Rowspan(0)
	{
	}*/
	SurfaceTile()
		: Texture(0), NeedsUpdate(false), Bpp(4), Rowspan(TILE_SIZE*4)
	{
		Buffer.resize(TILE_SIZE*Rowspan);
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TILE_SIZE, TILE_SIZE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	~SurfaceTile()
	{
		glDeleteTextures(1, &Texture);
	}
};

class GLTextureSurface : public Awesomium::Surface
{
public:
	GLTextureSurface(int width, int height);
	virtual ~GLTextureSurface();

	const std::vector<std::vector<SurfaceTile>>* GetTiles() const;

	int width() const { return m_width; }

	int height() const { return m_height; }

	//int size() const { return m_rowspan * m_height; }

	void UpdateTexture();

protected:
	virtual void Paint(unsigned char* src_buffer, int src_row_span, const Awesomium::Rect& src_rect, const Awesomium::Rect& dest_rect);
	virtual void Scroll(int dx, int dy, const Awesomium::Rect& clip_rect);

	//GLuint texture_id_;
	static GLuint m_bufferTexture;
	//unsigned char* buffer_;
	std::vector<std::vector<SurfaceTile>> m_tiles;
	int m_bpp, m_rowspan, m_width, m_height;
	//bool needs_update_, needs_swap;
	//std::mutex m_needsUpdateMutex;
};

class GLTextureSurfaceFactory : public Awesomium::SurfaceFactory
{
public:
	GLTextureSurfaceFactory();

	virtual ~GLTextureSurfaceFactory();

	virtual Awesomium::Surface* CreateSurface(Awesomium::WebView* view, int width, int height);

	virtual void DestroySurface(Awesomium::Surface* surface);
};

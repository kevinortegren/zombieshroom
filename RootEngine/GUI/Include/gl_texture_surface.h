#pragma once

#include <Awesomium/Surface.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <vector>
#include <mutex>

#define TILE_SIZE 128

class SurfaceTile
{
public:
	std::vector<char> Buffer;
	GLuint Texture[2];
	bool NeedsUpdate;
	int Bpp, Rowspan;
	std::mutex TextureMutex;
	int ActiveTexture;
	GLsync FenceID;

	SurfaceTile()
		: NeedsUpdate(false), Bpp(4), Rowspan(TILE_SIZE*4), ActiveTexture(0), FenceID(0)
	{
		Buffer.resize(TILE_SIZE*Rowspan);
		glGenTextures(2, Texture);
		glBindTexture(GL_TEXTURE_2D, Texture[0]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TILE_SIZE, TILE_SIZE);
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TILE_SIZE, TILE_SIZE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	SurfaceTile(SurfaceTile& p_other)
	{
		int i = 0;
	}
	~SurfaceTile()
	{
		glDeleteTextures(2, Texture);
	}
	void operator=(SurfaceTile& p_other)
	{
		int i = 0;
	}
};

#pragma pack(1)
struct TileData
{
	float x, y;
	float u, v;
	float tex;

	TileData(float p_x, float p_y, float p_u, float p_v, int p_tex)
	{
		x = p_x;
		y = p_y;
		u = p_u;
		v = p_v;
		tex = (float)p_tex;
	}
};
#pragma pack()

class GLTextureSurface : public Awesomium::Surface
{
public:
	GLTextureSurface(int width, int height);
	virtual ~GLTextureSurface();

	std::vector<std::vector<SurfaceTile>>* GetTiles() const;

	int width() const { return m_width; }

	int height() const { return m_height; }

	//int size() const { return m_rowspan * m_height; }
	//int GetActiveTexture() { return m_activeTexture; }

	void UpdateTexture();

protected:
	virtual void Paint(unsigned char* src_buffer, int src_row_span, const Awesomium::Rect& src_rect, const Awesomium::Rect& dest_rect);
	virtual void Scroll(int dx, int dy, const Awesomium::Rect& clip_rect);

	//GLuint texture_id_;
	//static GLuint m_bufferTexture;
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

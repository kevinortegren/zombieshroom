#pragma once

#include <RootEngine\Render\Include\Mesh.h>
#include <RootEngine\Render\Include\Effect.h>
#include <RootEngine\Render\Include\Texture.h>

namespace Render
{
	class GLRenderer;

	class GlowDevice
	{
	public:
		~GlowDevice();
		void Init(GLRenderer* p_renderer, int p_width, int p_height);
		void Process(GLRenderer* p_renderer, Mesh* m_mesh);
		void Resize(int p_width, int p_height);

		void SetGlowFactor(float p_factor);
		void SetGlowStrength(float p_strength);
		void SetGlowRadius(float p_radius);

	private:
		GLuint m_glowFramebuffer;
		EffectInterface* m_glowEffect;
		TextureInterface* m_glowTexture;
		int m_width;
		int m_height;
	};
}
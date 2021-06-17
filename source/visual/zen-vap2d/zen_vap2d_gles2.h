/**
 dont include this file.
 */
#pragma once

#if !INCLUDE_VAP2D_GLES
#error "don't include this file"
#endif

#include "zen_gles2.h"

namespace Zen { namespace Vap2d {
	typedef Zen::GL::eVertexMode eVertexMode;
	
	extern GL::Render * GetGLRender();
	
	struct _Buffer
	{
		Zen::GL::ArrayBuffer v;
		size_t size = 0;
		Zen::Color4f color;
	};

	struct SpriteBuffer : _Buffer
	{
	};

	struct ParticleBuffer : _Buffer
	{
	};

	struct DoodleBuffer : _Buffer
	{
	};

	struct Texture2DBuffer
	{
		GL::Texture2DBuffer v;
	};

	inline void _SetClearColor(Zen::Color4f color)
	{
		GetGLRender()->setClearColor(color);
	}

	
	class _RenderKeeper
	{
	public:
		_RenderKeeper()
		{
			GetGLRender()->clear();
		}
		
		virtual ~_RenderKeeper()
		{
		}
	};

	/**
	 only for gl
	 */
	void _PerformBlend(eBlend blend);
}}


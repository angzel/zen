#pragma once

#include "zen_metal.h"
#include "zen_color.h"
#include "zen_matrix.h"

namespace Zen { namespace Vap2d {
	typedef Metal::eMode eMode;
	
	struct _Buffer
	{
		Metal::Buffer v;
		size_t v_size;
		Metal::Buffer u;
		float _cache_alpha = 0;

		struct ExtraBuffer
		{
			Matrix4 m;
			Zen::Color4f c;
		};
		static const size_t u_length = sizeof(ExtraBuffer);
		static const size_t u_m_off = 0;
		static const size_t u_c_off = sizeof(Matrix4);
	};

	struct SpriteBuffer : _Buffer
	{
	};

	struct ParticleBuffer : _Buffer
	{
	};

	struct DoodleBuffer : _Buffer
	{
		struct ExtraBuffer
		{
			Matrix4 m;
			Zen::Color4f c;
		};
	};

	struct Texture2DBuffer
	{
		Metal::Texture v;
	};

	inline void _SetClearColor(Zen::Color4f color)
	{
		Metal::Render::S()->setClearColor(color);
	}

	inline void _StartRender()
	{
		Metal::Render::S()->start();
	}

	inline void _EndRender()
	{
		Metal::Render::S()->end();
	}

}}

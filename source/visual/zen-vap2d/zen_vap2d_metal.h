#pragma once

#include "zen_metal.h"
#include "zen_color.h"
#include "zen_matrix.h"
#include "zen_metal_prepare.h"

namespace Zen { namespace Vap2d {
	typedef Metal::eVertexMode eVertexMode;
	
	struct _Buffer
	{
		Metal::Buffer v;
		Metal::Buffer u;
		size_t v_size = 0;
		float _cache_alpha = 0;

		typedef VertexExtraIN ExtraBuffer;
		static const size_t u_length = sizeof(ExtraBuffer);
		static const size_t u_m_off = 0;
		static const size_t u_c_off = sizeof(matrix_float4x4);
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
		Metal::Texture v;
	};

	inline void _SetClearColor(Zen::Color4f color)
	{
		Metal::Render::Get()->setClearColor(color);
	}

	class _RenderKeeper
	{
	public:
		_RenderKeeper()
		{
			Metal::Render::Get()->start();
		}
		
		virtual ~_RenderKeeper()
		{
			Metal::Render::Get()->end();
		}
	};

	/**
	 only for metal.
	 */
	std::pair<Metal::eBlendFactor, Metal::eBlendFactor>
	_GetMetalBlend(eBlend blend);
}}

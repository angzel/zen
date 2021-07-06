#pragma once

#include "zen_app_config.h"

#if ZEN_DRAW_METAL

#include "zen_metal.h"
#include "zen_color.h"
#include "zen_matrix.h"
#include "zen_metal_prepare.h"

namespace Zen { namespace Vap {
	typedef Metal::eVertexMode eVertexMode;
	
	template<class tUniformIN>
	class GUBuffer {
	public:
		static const size_t u_length = sizeof(tUniformIN);
		static const size_t u_matrix_off = 0;
		static const size_t u_color_off = sizeof(matrix_float4x4);
	protected:
		Metal::Buffer m_u;
		float m_alpha = 0;
		int m_mat_version = -1;
	public:
		GUBuffer()
		{
			m_u.create(u_length);
		}
		
		void updateMatrix(Matrix4 const & m, int version, bool dirty)
		{
			if(!dirty && m_mat_version == version) return;
				
			m_u.update(u_matrix_off, sizeof(m), &m);
			m_mat_version = version;
		}
		
		void updateColor(Color4f const & c, float alpha, bool dirty)
		{
			if(!dirty && alpha == m_alpha) return;
			m_u.update(u_color_off, sizeof(c), &c);
			m_alpha = alpha;
		}
		Metal::Buffer & u() { return m_u; }
	};
	
	template<class tVertexIN>
	class GVertexBuffer
	{
	protected:
		Metal::Buffer m_v;
		size_t m_v_count = 0;
	public:
		
		Metal::Buffer & v() { return m_v; }
		
		size_t v_count()
		{
			return m_v_count;
		}
		void createVertexBuffer(tVertexIN const * src, size_t count)
		{
			if(count) m_v.create(sizeof(tVertexIN)*count, src);
			m_v_count = count;
		}
		
		void updateVertexBuffer(tVertexIN const * src, size_t count, size_t index)
		{
			if(index >= m_v_count) return;
			if(count + index > m_v_count) count = m_v_count - index;
			m_v.update(index*sizeof(tVertexIN), count*sizeof(tVertexIN), src);
		}
	};

	struct RBufferTexture
	: RBuffer
	,GVertexBuffer<VertexIN_ShaderT>
	, GUBuffer<UniformIN_Shader>
	{
		/**
		 4 vertex points for sprite label &... (with a rect view).
		 */
		void initBufferV4();
		void updateBufferV4(float x0, float y0, float x1, float y1);
	};

	struct RBufferPoints
	: RBuffer
	, GVertexBuffer<VertexIN_ShaderP>
	, GUBuffer<UniformIN_ShaderP>
	{
		float m_scale_x = 0;
		void updateSizeRatio(Vector4 const & size_ratio, float scale_x)
		{
			if(m_scale_x == scale_x) return;
				
			m_u.update(UniformIN_ShaderP_S_OFF, sizeof(Vector4), &size_ratio);
			m_scale_x = scale_x;
		}
	};

	struct RBufferColors
	: RBuffer
	, GVertexBuffer<VertexIN_ShaderC>
	, GUBuffer<UniformIN_Shader>
	{

	};
	
	struct TextureBuffer
	{
		Metal::Texture mt_buffer;
	};
	
	struct MetalHelper
	{
	   static std::pair<Metal::eBF, Metal::eBF>
	   GetBlends(eBlend blend);
	};
	/**
	 only for metal.
	 */
}}

#endif

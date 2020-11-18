
#include "zen_vap2d_config.h"

#if ZEN_APP_DRAW_API_METAL

#include "zen_metal_id.h"

#include "zen_vap2d.h"
#include "zen_metal_shader.h"

namespace Zen { namespace Vap2d {

		//	static eBlend S_last_blend = eBlend::Inherit;

	std::pair<Metal::eBlendFactor, Metal::eBlendFactor> _GetMetalBlend(eBlend blend)
	{
		switch (blend) {
			case eBlend::Add:
				return {
					Metal::eBlendFactor::SourceAlpha,
					Metal::eBlendFactor::One
				};
				break;
			case eBlend::Inverse:
				return {
					Metal::eBlendFactor::OneMinusSourceColor,
					Metal::eBlendFactor::OneMinusSourceAlpha
				};
				break;
			case eBlend::Normal:
			default:
				return {
					Metal::eBlendFactor::SourceAlpha,
					Metal::eBlendFactor::OneMinusSourceAlpha
				};
				break;
		}
	}

	template<typename tBuffer>
	inline static void S_init_texture_buffer(tBuffer & buffer)
	{
		SamplerVertexIN coords[] = {
			{ { 0, 0, }, { 0, 1, } },
			{ { 1, 0, }, { 1, 1, } },
			{ { 0, 1, }, { 0, 0, } },
			{ { 1, 1, }, { 1, 0, } },
		};
		buffer.v.create(sizeof(coords), coords);
	}
	template<typename tBuffer>
	inline void S_update_texture_buffer(tBuffer & buffer, float x0, float y0, float x1, float y1)
	{
		SamplerVertexIN coords[] = {
			{ { 0, 0, }, { x0, y1, } },
			{ { 1, 0, }, { x1, y1, } },
			{ { 0, 1, }, { x0, y0, } },
			{ { 1, 1, }, { x1, y0, } },
		};
		buffer.v.update(0, sizeof(coords), coords);
	}

	template<typename tBuffer>
	inline void S_init_extra_buffer(tBuffer & buffer)
	{
		buffer.u.create(tBuffer::u_length);
	}

	/**
	 set extra matrix to buffer
	 */
	template<typename tBuffer>
	inline void S_update_extra_buffer_m(tBuffer & buffer, Matrix4 const & m)
	{
		buffer.u.update(tBuffer::u_m_off, sizeof(m), &m);
	}

	/**
	 set extra color to buffer
	 */
	template<typename tBuffer>
	inline void S_update_extra_buffer_c(tBuffer &  buffer, Zen::Color4f const & c)
	{
		buffer.u.update(tBuffer::u_c_off, sizeof(c), &c);
	}

	void Sprite::_initSprite()
	{
		m_gpus.v_size = 4;
		S_init_texture_buffer(m_gpus);
		S_init_extra_buffer(m_gpus);
	}
	void Sprite::_drawSprite()
	{
		if(m_is_flip_dirty)
		{
			m_is_flip_dirty = false;
			m_is_buffer_dirty = true;
		}
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;
			float x0 = m_texture_rect.x * m_texture->using_size().w;
			float y0 = m_texture_rect.y * m_texture->using_size().h;
			float x1 = m_texture_rect.w * m_texture->using_size().w + x0;
			float y1 = m_texture_rect.h * m_texture->using_size().w + y0;
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);

			S_update_texture_buffer(m_gpus, x0, y0, x1, y1);
		}

		bool dirty = this->updateMatrix();
		if(dirty)
		{
			S_update_extra_buffer_m(m_gpus, m_world_matrix);
		}

		float alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();
		if(m_is_color_dirty || m_gpus._cache_alpha != alpha)
		{
			m_is_color_dirty = false;
			m_gpus._cache_alpha = alpha;
			Zen::Color4f c = m_color;
			c.alpha *= alpha;
			S_update_extra_buffer_c(m_gpus, c);
		}

		auto r = Metal::Render::S();
		
		Metal::SharedPipe pipe = nullptr;
		if(m_blend == eBlend::None)
		{
			pipe = Metal::Pipes::S()->getShaderSampler
			(m_is_grey_mode, m_texture->format() == ePixel::Grey);
		}
		else
		{
			auto blends = _GetMetalBlend(m_blend);
			pipe = Metal::Pipes::S()->getShaderSampler
			(m_is_grey_mode, m_texture->format() == ePixel::Grey,
			 blends.first, blends.second);
		}

		r->activePipe(pipe->getID());
		r->bindTexture(ZEN_SH_S0, m_texture->buffer().v.getID());
		r->setVertexBuffer(ZEN_SH_V, m_gpus.v.getID(), 0);
		r->setVertexBuffer(ZEN_SH_M, m_gpus.u.getID(), 0);
		r->setFragmentBuffer(ZEN_SH_F_C, m_gpus.u.getID(), m_gpus.u_c_off);
		r->drawPrimitives(Metal::eMode::TriangleStrip, 0, 4);
	}
	void Label::_initLabel()
	{
		m_gpus.v_size = 4;
		S_init_texture_buffer(m_gpus);
		S_init_extra_buffer(m_gpus);

	}
	void Label::_drawLabel()
	{
		if(m_is_flip_dirty)
		{
			m_is_flip_dirty = false;
			m_is_buffer_dirty = true;
		}
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;

			float x0 = 0;
			float y0 = 0;
			float x1 = m_texture->using_size().w;
			float y1 = m_texture->using_size().h;

			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);

			S_update_texture_buffer(m_gpus, x0, y0, x1, y1);
		}

		bool dirty = this->updateMatrix();
		if(dirty)
		{
			S_update_extra_buffer_m(m_gpus, m_world_matrix);
		}

		float alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();
		if(m_is_color_dirty || m_gpus._cache_alpha != alpha)
		{
			m_is_color_dirty = false;
			m_gpus._cache_alpha = alpha;
			Zen::Color4f c = m_color;
			c.alpha *= alpha;
			S_update_extra_buffer_c(m_gpus, c);
		}

		auto r = Metal::Render::S();

		Metal::SharedPipe pipe = nullptr;
		if(m_blend == eBlend::None)
		{
			pipe = Metal::Pipes::S()->getShaderSampler
			(m_is_grey_mode, m_texture->format() == ePixel::Grey);
		}
		else
		{
			auto blends = _GetMetalBlend(m_blend);
			pipe = Metal::Pipes::S()->getShaderSampler
			(m_is_grey_mode, m_texture->format() == ePixel::Grey,
			 blends.first, blends.second);
		}
		r->activePipe(pipe->getID());
		r->bindTexture(ZEN_SH_S0, m_texture->buffer().v.getID());
		r->setVertexBuffer(ZEN_SH_V, m_gpus.v.getID(), 0);
		r->setVertexBuffer(ZEN_SH_M, m_gpus.u.getID(), 0);
		r->setFragmentBuffer(ZEN_SH_F_C, m_gpus.u.getID(), m_gpus.u_c_off);
		r->drawPrimitives(Metal::eMode::TriangleStrip, 0, 4);
	}

	void Particle::_initParticle()
	{
		m_gpus.v_size = 0;
		S_init_extra_buffer(m_gpus);
	}

	void Particle::_drawParticle()
	{
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;

			if(m_gpus.v_size == m_dots.size())
			{
				m_gpus.v.update(0, sizeof(Dot) * m_dots.size(), m_dots.data());
			}
			else
			{
				m_gpus.v.create(sizeof(Dot) * m_dots.size(), m_dots.data());
				m_gpus.v_size = m_dots.size();
			}
		}

		bool dirty = this->updateMatrix();
		if(dirty)
		{
			S_update_extra_buffer_m(m_gpus, m_world_matrix);
		}

		float alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();
		if(m_is_color_dirty || m_gpus._cache_alpha != alpha)
		{
			m_is_color_dirty = false;
			m_gpus._cache_alpha = alpha;
			Zen::Color4f c = m_color;
			c.alpha *= alpha;
			S_update_extra_buffer_c(m_gpus, c);
		}

		auto r = Metal::Render::S();

		Metal::SharedPipe pipe = nullptr;
		if(m_blend == eBlend::None)
		{
			pipe = Metal::Pipes::S()->getShaderParticle
			(m_is_grey_mode, m_texture->format() == ePixel::Grey);
		}
		else
		{
			auto blends = _GetMetalBlend(m_blend);
			pipe = Metal::Pipes::S()->getShaderParticle
			(m_is_grey_mode, m_texture->format() == ePixel::Grey,
			 blends.first, blends.second);
		}

		r->activePipe(pipe->getID());
		r->bindTexture(ZEN_SH_S0, m_texture->buffer().v.getID());
		r->setVertexBuffer(ZEN_SH_V, m_gpus.v.getID(), 0);
		r->setVertexBuffer(ZEN_SH_M, m_gpus.u.getID(), 0);
		r->setFragmentBuffer(ZEN_SH_F_C, m_gpus.u.getID(), m_gpus.u_c_off);
		r->drawPrimitives(Metal::eMode::Point, 0, m_gpus.v_size);
	}

	void Doodle::_initDoodle()
	{
		m_gpus.v_size = 0;
		S_init_extra_buffer(m_gpus);
	}
	void Doodle::_drawDoodle()
	{
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;

			if(m_gpus.v_size == m_dots.size())
			{
				m_gpus.v.update(0, sizeof(Dot) * m_dots.size(), m_dots.data());
			}
			else
			{
				m_gpus.v.create(sizeof(Dot) * m_dots.size(), m_dots.data());
				m_gpus.v_size = m_dots.size();
			}
		}

		bool dirty = this->updateMatrix();
		if(dirty)
		{
			S_update_extra_buffer_m(m_gpus, m_world_matrix);
		}

		float alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();
		if(m_is_color_dirty || m_gpus._cache_alpha != alpha)
		{
			m_is_color_dirty = false;
			m_gpus._cache_alpha = alpha;
			Zen::Color4f c = m_color;
			c.alpha *= alpha;
			S_update_extra_buffer_c(m_gpus, c);
		}

		auto r = Metal::Render::S();
		
		Metal::SharedPipe pipe = nullptr;
		if(m_blend == eBlend::None)
		{
			pipe = Metal::Pipes::S()->getShaderColor(m_is_grey_mode);
		}
		else
		{
			auto blends = _GetMetalBlend(m_blend);
			pipe = Metal::Pipes::S()->getShaderColor
			(m_is_grey_mode, blends.first, blends.second);
		}
		r->activePipe(pipe->getID());
		r->setVertexBuffer(ZEN_SH_V, m_gpus.v.getID(), 0);
		r->setVertexBuffer(ZEN_SH_M, m_gpus.u.getID(), 0);
		r->setFragmentBuffer(ZEN_SH_F_C, m_gpus.u.getID(), m_gpus.u_c_off);
		r->drawPrimitives(m_mode, 0, m_gpus.v_size);

	}

	void Texture::set(size_t width, size_t height, ePixel format, void const * data)
	{
		if(format == ePixel::GA)
		{
			auto image = Image::Create(ePixel::RGBA, width, height);
			image->copy(format, data);
			return this->set(image.get());
		}
		else if(format == ePixel::RGB)
		{
			auto image = Image::CreateWidthByte(ePixel::RGBA, width, height, 0xff);
			image->copyColor(format, data);
			return this->set(image.get());
		}
		else
		{
			m_format = format;
			m_size = { (float)width, (float)height };
			m_using_size = { 1, 1 };
			m_texture.v.create(width, height, format==ePixel::Grey, data, 0);
		}
	}
}}


#endif

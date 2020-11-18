
#include "zen_vap2d_config.h"

#if defined(ZEN_OS_IOS) || defined(ZEN_OS_ANDROID)
#if ZEN_APP_DRAW_API_GLES

#include "zen_vap2d.h"

namespace Zen { namespace Vap2d {

	static float S_coords[] = {
		0, 1, 1, 1, 0, 0, 1, 0, // sampler
		0, 0, 1, 0, 0, 1, 1, 1, // vertex
	};

	static eBlend S_last_blend = eBlend::None;

	void _PerformBlend(eBlend blend)
	{
		if(blend == eBlend::Inherit)
		{
			blend = DrawStack::S()->getTopBlend();
		}
		auto s = GL::Render::S();
		if(blend == S_last_blend) return;
		switch (blend) {
			case eBlend::Normal:
				s->enableBlendFunction
				(GL::eSrcBlend::SrcAlpha,
				 GL::eDstBlend::OneMinusSrcAlpha);
				break;
			case eBlend::Add:
				s->enableBlendFunction
				(GL::eSrcBlend::SrcAlpha,
				 GL::eDstBlend::One);
				break;
			case eBlend::Inverse:
				s->enableBlendFunction
				(GL::eSrcBlend::OneMinusDstColor,
				 GL::eDstBlend::OneMinusSrcAlpha);
				break;
			case eBlend::None:
				s->disableBlend();
				break;
			default:
				break;
		}
		S_last_blend = blend;
	}

	void Sprite::_initSprite()
	{
		m_gpus.v.create(sizeof(S_coords), S_coords);
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
			float y1 = m_texture_rect.h * m_texture->using_size().h + y0;
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);

			float coords[] = { x0, y1, x1, y1, x0, y0, x1, y0, };

			m_gpus.v.update(0, sizeof(float)*8, coords);
		}

		this->updateMatrix();

		auto alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();

		auto sha = GL::ShaderPrograms::S()
		->getShaderSampler(m_is_grey_mode, m_texture->format() == ePixel::Grey);

		_PerformBlend(m_blend);

		auto s = GL::Render::S();
		
		s->activeProgram(sha->program.getID());

		s->enableBuffer(m_gpus.v.getID());

		s->setVertexBuffer(sha->a_sampler_coord, 2, GL::eType::Float, false, 0, 0);
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, 0, 8*sizeof(float));

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setSampler(sha->u_sampler, 1, m_texture->buffer().v.getID());
		s->setUniform(sha->u_transform, m_world_matrix);
		s->drawArray(GL::eMode::TriangleStrip, 0, 4);
	}

	void Label::_initLabel()
	{
		m_gpus.v.create(sizeof(S_coords), S_coords);
		m_gpus.size = 4;
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

			float coords[] = { x0, y1, x1, y1, x0, y0, x1, y0, };

			m_gpus.v.update(0, sizeof(float)*8, coords);
		}

		this->updateMatrix();

		auto alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();

		auto sha = GL::ShaderPrograms::S()
		->getShaderSampler(m_is_grey_mode, m_texture->format() == ePixel::Grey);

		_PerformBlend(m_blend);

		auto s = GL::Render::S();

		s->activeProgram(sha->program.getID());
		s->enableBuffer(m_gpus.v.getID());

		s->setVertexBuffer(sha->a_sampler_coord, 2, GL::eType::Float, false, 0, 0);
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, 0, 8*sizeof(float));

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setSampler(sha->u_sampler, 1, m_texture->buffer().v.getID());
		s->setUniform(sha->u_transform, m_world_matrix);
		s->drawArray(GL::eMode::TriangleStrip, 0, 4);
	}

	void Particle::_initParticle()
	{

	}
	void Particle::_drawParticle()
	{
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;

			static const size_t step = sizeof(Dot);

			if(m_gpus.size != m_dots.size())
			{
				m_gpus.size = m_dots.size();
				m_gpus.v.create(step * m_dots.size(), m_dots.data());
			}
			else
			{
				m_gpus.v.update(0, step * m_dots.size(), m_dots.data());
			}
		}

		this->updateMatrix();

		auto alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();

		auto sha = GL::ShaderPrograms::S()
		->getShaderParticle(m_is_grey_mode, m_texture->format() == ePixel::Grey);

		_PerformBlend(m_blend);

		auto s = GL::Render::S();

		s->activeProgram(sha->program.getID());

		s->enableBuffer(m_gpus.v.getID());

		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, true , sizeof(Dot), 0);
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(Dot), 4*sizeof(float));
		s->setVertexBuffer(sha->a_size,  1, GL::eType::Float, false, sizeof(Dot), 6*sizeof(float));

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setSampler(sha->u_sampler, 1, m_texture->buffer().v.getID());
		s->setUniform(sha->u_transform, m_world_matrix);
		s->drawArray(GL::eMode::Point, 0, m_dots.size());

	}

	void Doodle::_initDoodle()
	{
		
	}
	void Doodle::_drawDoodle()
	{
		if(m_is_buffer_dirty)
		{
			m_is_buffer_dirty = false;
			if(m_gpus.size != m_dots.size())
			{
				m_gpus.size = m_dots.size();
				m_gpus.v.create(sizeof(Dot)* m_dots.size(), m_dots.data());
			}
			else
			{
				m_gpus.v.update(0, sizeof(Dot)* m_dots.size(), m_dots.data());
			}
		}

		this->updateMatrix();

		auto alpha = DrawStack::S()->getTopAlpha() * this->getAlpha();

		auto sha = GL::ShaderPrograms::S()
		->getShaderColor(m_is_grey_mode);

		_PerformBlend(m_blend);

		auto s = GL::Render::S();

		s->activeProgram(sha->program.getID());
		s->enableBuffer(m_gpus.v.getID());

		s->setVertexBuffer(sha->a_color, 4, GL::eType::Float, false, sizeof(Dot), 0);
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, sizeof(Dot), 4*sizeof(float));

		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setUniform(sha->u_transform, m_world_matrix);

		s->drawArray(m_mode, 0, m_dots.size());
	}
}}

#include "zen_numerical.h"

namespace Zen { namespace Vap2d {


#if ZEN_GL_2D_ANY_SZ
//#if 0
	void Texture::set(size_t width, size_t height, ePixel format, void const * data)
	{
		if(format == ePixel::GA)
		{
			auto image = Image::CreateWidthByte(ePixel::RGBA, width, height, 0xff);
			image->copy(format, data);
			this->set(image.get());
		}
		else
		{
			m_format = format;
			m_size = { (float)width, (float)height };
			m_texture.v.createWithData(width, height, GL::eFilter::Linear, format, data);
			m_using_size = { 1.f, 1.f };
		}
	}
#else
	void Texture::set(size_t width, size_t height, ePixel format, void const * data)
	{
		musts(width < 0xffffff && height < 0xffffff, "");

		if(format == ePixel::GA)
		{
			auto image = Image::CreateWidthByte(ePixel::RGBA, width, height, 0xff);
			image->copy(format, data);
			this->set(image.get());
		}
		else
		{
			m_format = format;
			m_size = { (float)width, (float)height };

			auto w = Zen::GetMinPowerTwo((uint32_t)width);
			auto h = Zen::GetMinPowerTwo((uint32_t)height);

			if(w == width && h == height)
			{
				m_texture.v.createWithData(width, height, GL::eFilter::Linear, format, data);
				m_using_size = { 1.0f, 1.0f };
			}
			else
			{
				m_texture.v.create(width, height, GL::eFilter::Linear, format);
				m_texture.v.fill(0, 0, width, height, data);
				m_using_size = { (float)width /(float)w, (float)height /(float)h };
			}
		}
	}
#endif
}}

#endif
#endif

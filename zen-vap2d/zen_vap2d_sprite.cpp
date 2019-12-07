
#include "zen_vap2d_sprite.h"
#include "zen_gles2_shader_sampler.h"

namespace Zen { namespace Vap2d {

	Sprite::Sprite()
	: m_texture_buffer(Zen::GL::EBufferType::ArrayBuffer)
	{
		m_texture_rect = RectMake(0, 0, 1, 1);
		float coords[] = {
			0, 1, 0, 0, 1, 0, 1, 1,
			0, 0, 0, 1, 1, 1, 1, 0,
		};
		m_texture_buffer.create();
		m_texture_buffer.bindData(sizeof(coords), coords);
		m_color.set(1, 1, 1, 1);
		m_shader = nullptr;
		std::cout << "size:" << m_texture_buffer.size() << std::endl;
	}

	void Sprite::setTexture(std::shared_ptr<Zen::GL::TextureSrc const> texture)
	{
		m_texture_src = texture;
		m_is_texture_dirty = true;
	}

	void Sprite::setTextureRect(Zen::Rect rect)
	{
		m_texture_rect = rect;
		m_is_texture_dirty = true;
	}

	Zen::GL::SharedTexture Sprite::getTexture()
	{
		return m_texture_src;
	}

	Zen::Rect Sprite::getTextureRect()
	{
		return m_texture_rect;
	}
	void Sprite::setTextureFlipX(bool x)
	{
		m_is_flip_x = x;
		m_is_texture_dirty = true;
	}

	void Sprite::setTextureFlipY(bool y)
	{
		m_is_flip_y = y;
		m_is_texture_dirty = true;
	}

	bool Sprite::isTextureFlipX()
	{
		return m_is_flip_x;
	}

	bool Sprite::isTextureFlipY()
	{
		return m_is_flip_y;
	}

	void Sprite::draw()
	{
		if(!m_texture_src) return;

		this->updateWorldMatrix();

		if(m_is_texture_dirty)
		{
			float x0 = m_texture_rect.x * m_texture_src->gl_size.w;
			float y0 = m_texture_rect.y * m_texture_src->gl_size.h;
			float x1 = m_texture_rect.w * m_texture_src->gl_size.w + x0;
			float y1 = m_texture_rect.h * m_texture_src->gl_size.w + y0;
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);
			float coords[] = { x0, y1, x0, y0, x1, y0, x1, y1 };
			m_texture_buffer.updateData(0, sizeof(float)*8, coords);
			m_is_texture_dirty = false;
		}

		auto alpha = RenderStack::GetDefault()->getTopAlpha() * this->getAlpha();

		using namespace Zen::GL;
		ShaderSampler const * ss = m_shader;
		if(ss == nullptr)
		{
			if(m_texture_src->format == ETextureFmt::Alpha) ss = ShaderSampler::GetAlpha();
			else ss = ShaderSampler::GetNormal();
		}
		RenderStack::GetDefault()->performBlend(m_blend);
		
		Render::BindTexture(m_texture_src->texture, 1);
		Render::ActiveProgram(ss->program);
		Render::BindBuffer(m_texture_buffer);

		Render::SetVertexAttribBuffer(ss->a_coord, 2, EType::Float, false, 0, 8*sizeof(float));
		Render::SetVertexAttribBuffer(ss->a_sampler_coord, 2, EType::Float, false, 0, 0);
		Render::EnableVertexAttrib(ss->a_coord);
		Render::EnableVertexAttrib(ss->a_sampler_coord);
		Render::SetUniformFloat(ss->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		Render::SetUniformInt(ss->u_sampler, 1);
		Render::SetUniformMat(ss->u_transform, m_world_matrix);
		Render::DrawArray(EDrawMode::TriangleFan, 0, 4);

		Render::UnbindBuffer(m_texture_buffer);
	}

	void Sprite::setTextureColor(Zen::Color4f color)
	{
		m_color = color;
	}

	Zen::Color4f Sprite::getTextureColor()
	{
		return m_color;
	}
	void Sprite::setEffect(const Zen::GL::ShaderSampler *shader)
	{
		m_shader = shader;
	}

}}


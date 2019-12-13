
#include "zen_vap2d_sprite.h"
#include "zen_gles2_shader_sampler.h"
#include "zen_vap2d_root.h"

namespace Zen { namespace Vap2d {

	Sprite::Sprite(std::string const & name)
	: LNode(name)
	{
		float coords[] = {
			0, 1, 1, 1, 0, 0, 1, 0,// sampler
			0, 0, 1, 0, 0, 1, 1, 1, // vertex
		};
		m_texture_buffer.bindData(sizeof(coords), coords);
	}

	void Sprite::setTexture(SharedTexture texture)
	{
		m_texture = texture;
		m_is_texture_dirty = true;
	}

	void Sprite::setTextureRect(Zen::Rect rect)
	{
		m_texture_rect = rect;
		m_is_texture_dirty = true;
	}

	SharedTexture Sprite::getTexture()
	{
		return m_texture;
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

	void Sprite::clearSpriteDirty()
	{
		if(!m_is_texture_dirty) return;
		m_is_texture_dirty = false;

		float x0 = m_texture_rect.x * m_texture->using_size().w;
		float y0 = m_texture_rect.y * m_texture->using_size().h;
		float x1 = m_texture_rect.w * m_texture->using_size().w + x0;
		float y1 = m_texture_rect.h * m_texture->using_size().w + y0;
		if(m_is_flip_x) std::swap(x0, x1);
		if(m_is_flip_y) std::swap(y0, y1);
		float coords[] = {
			x0, y1, x1, y1, x0, y0, x1, y0,// sampler
		};
		m_texture_buffer.updateData(0, sizeof(float)*8, coords);
		/**
		 if the size is zero, reset as texture size.
		 */
		if(this->getWidth() == 0 || this->getHeight() == 0)
		{
			auto w = m_texture->size().w * m_texture_rect.w;
			auto h = m_texture->size().h * m_texture_rect.h;
			this->setSize(w, h);
		}
	}
	void Sprite::draw()
	{
		if(!m_texture) return;

		this->clearSpriteDirty();

		this->updateWorldMatrix();

		auto alpha = RenderStack::GetDefault()->getTopAlpha() * this->getAlpha();

		using namespace Zen::GL;
		std::shared_ptr<ShaderSampler const> ss = m_shader;
		if(ss == nullptr)
		{
			if(m_texture->format() == EBPP::Grey) ss = ShaderSampler::GetAlpha();
			else ss = ShaderSampler::GetNormal();
		}
		
		RenderStack::GetDefault()->performBlend(m_blend);
		
		Render::BindTexImage2D(m_texture->image2d().getObject(), 1);
		Render::ActiveProgram(ss->program.getObject());
		Render::BindArrayBuffer(m_texture_buffer.getObject());

		Render::SetVertexAttribBuffer(ss->a_coord, 2, EType::Float, false, 0, 8*sizeof(float));
		Render::SetVertexAttribBuffer(ss->a_sampler_coord, 2, EType::Float, false, 0, 0);
		Render::EnableVertexAttrib(ss->a_coord);
		Render::EnableVertexAttrib(ss->a_sampler_coord);
		Render::SetUniformFloat(ss->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		Render::SetUniformInt(ss->u_sampler, 1);
		Render::SetUniformMat(ss->u_transform, m_world_matrix);
		Render::DrawArray(EDrawMode::TriangleStrip, 0, 4);

		Render::BindArrayBuffer(0);
	}

	void Sprite::setColor(Zen::Color4f color)
	{
		m_color = color;
	}

	Zen::Color4f Sprite::getColor()
	{
		return m_color;
	}

	void Sprite::setEffect(std::shared_ptr<Zen::GL::ShaderSampler const> shader)
	{
		m_shader = shader;
	}

}}


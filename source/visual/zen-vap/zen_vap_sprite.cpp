
#include "zen_vap_sprite.h"
#include "zen_vap_root.h"
#include "zen_vap_render.h"

namespace Zen { namespace Vap {
	
	Sprite::Sprite(std::string const & name)
	: Node2d(name), View2d(true)
	{
		initR_();
	}
	
	void Sprite::setTexture(TextureShared texture)
	{
		m_texture = texture;
		m_is_texture_dirty = true;
	}
	
	void Sprite::setTextureRect(Zen::Rect4f rect)
	{
		m_texture_rect = rect;
		m_is_texture_dirty = true;
	}
	
	TextureShared Sprite::getTexture()
	{
		return m_texture;
	}
	
	Zen::Rect4f Sprite::getTextureRect()
	{
		return m_texture_rect;
	}
	
	/// content size {
	void Sprite::enableFixedSize(bool v)
	{
		if(m_is_fixed_sized == v) return;
		
		if(!m_is_fixed_sized) // old setting.
		{
			this->setScale2_(m_fixed_size.w, m_fixed_size.h);
		}
		else
		{
			this->updateContent();
			this->setScale2_(m_texture_size.w, m_texture_size.h);
		}
		m_is_fixed_sized = v;
	}
	
	bool Sprite::isFixedSizeEnabled()
	{
		return m_is_fixed_sized;
	}
	
	void Sprite::setFixedSize(Size2 size)
	{
		m_fixed_size = size;
		if(m_is_fixed_sized)
		{
			this->setScale2_(m_fixed_size.w, m_fixed_size.h);
		}
	}
	
	Size2 Sprite::getFixedSize()
	{
		return m_fixed_size;
	}
	
	Size2 Sprite::getContentSize()
	{
		if(m_is_fixed_sized) return m_fixed_size;
		else
		{
			this->updateContent();
			return m_texture_size;
		}
	}
	/// } content size
	void Sprite::updateContent()
	{
		if(m_is_texture_dirty) {
			
			m_is_texture_dirty = false;
			m_RBuf.is_dirty = true;
			
			m_texture_size.w = m_texture->size().w * m_texture_rect.w;
			m_texture_size.h = m_texture->size().h * m_texture_rect.h;
			
			if(!m_is_fixed_sized)
			{
				this->setScale2_(m_texture_size.w, m_texture_size.h);
			}
		}
	}
	void Sprite::draw()
	{
		if(!m_texture) return;
		this->updateContent();
		this->updateMatrix();
		if(m_is_flip_dirty)
		{
			m_is_flip_dirty = false;
			m_RBuf.is_dirty = true;
		}
		this->drawR_();
	}
#if ZEN_DRAW_GLES
	
	/// s_coords
	/// LB, RB, LT, RT
	static float s_coords[] = {
		0, 1, 1, 1, 0, 0, 1, 0, // sampler
		0, 0, 1, 0, 0, 1, 1, 1, // vertex
	};
	static const size_t s_vertex_coord_off = sizeof(float) * 8;
	static const size_t s_texture_coord_off = 0;
//	static const size_t s_vertex_coord_size = sizeof(float) * 8;
	static const size_t s_texture_coord_size = sizeof(float) * 8;
	
	void Sprite::initR_()
	{
		m_RBuf.v.create(sizeof(s_coords), s_coords);
	}
	void Sprite::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			float x0 = m_texture_rect.x * m_texture->using_size().w;
			float y0 = m_texture_rect.y * m_texture->using_size().h;
			float x1 = m_texture_rect.w * m_texture->using_size().w + x0;
			float y1 = m_texture_rect.h * m_texture->using_size().h + y0;
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);
			
			float coords[] = { x0, y1, x1, y1, x0, y0, x1, y0, };
			
			m_RBuf.v.update(s_texture_coord_off, s_texture_coord_size, coords);
		}
		
		auto alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
				
		auto sha = GL::ShaderPrograms::Get()
		->getShaderT(m_is_grey_mode, m_texture->format());
		
		GLHelper::PerformBlend(m_blend);
		
		auto s = GLHelper::GetGLRender();
		
		s->activeProgram(sha->program.getID());
		
		s->enableBuffer(m_RBuf.v.getID());
		
		s->setVertexBuffer(sha->a_sampler_coord, 2, GL::eType::Float, false, 0, s_texture_coord_off);
		s->setVertexBuffer(sha->a_coord, 2, GL::eType::Float, false, 0, s_vertex_coord_off);
		
		s->setUniform(sha->u_color, m_color.red, m_color.green, m_color.blue, m_color.alpha*alpha);
		s->setSampler(sha->u_sampler, 1, m_texture->buffer().gl_buffer.getID());
		s->setUniform(sha->u_transform, this->getMatrix());
		s->drawArray(GL::eVertexMode::TriangleStrip, 0, 4);
	}
	
#elif ZEN_DRAW_METAL
	void Sprite::initR_()
	{
		m_RBuf.initBufferV4();
	}
	void Sprite::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			float x0 = m_texture_rect.x * m_texture->using_size().w;
			float y0 = m_texture_rect.y * m_texture->using_size().h;
			float x1 = m_texture_rect.w * m_texture->using_size().w + x0;
			float y1 = m_texture_rect.h * m_texture->using_size().w + y0;
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);
			
			m_RBuf.updateBufferV4(x0, y0, x1, y1);
		}
		
		m_RBuf.updateMatrix(this->getMatrix(), getMatrixVersion(), false);
		
		float alpha = DrawStack::Get()->getTopAlpha() * this->getAlpha();
		m_RBuf.updateColor(getColor(), alpha, m_is_color_dirty);
		m_is_color_dirty = false;
		
		auto r = Metal::Render::Get();
		
		auto blends = MetalHelper::GetBlends(m_blend);
		
		auto pipe = Metal::PipeT::GetShared(m_is_grey_mode, m_texture->format(), blends.first, blends.second);
		
		r->activePipe(pipe->getID());
		
		if(m_texture) r->bindTexture(pipe->TextureIndex, m_texture->buffer().mt_buffer.getID());
		r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
		r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
		
		r->drawPrimitives(Metal::eVertexMode::TriangleStrip, 0, 4);
	}
#endif
}}


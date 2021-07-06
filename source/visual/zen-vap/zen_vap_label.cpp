
#include "zen_vap_label.h"
#include "zen_utf8.h"
#include "zen_ticker.h"

namespace Zen { namespace Vap {
	
	Label::Label(std::string const & name)
	: Node2d(name), View2d(true)
	{
		initR_();
	}
	
	void Label::setFont(std::shared_ptr<FontBrush> font)
	{
		if(font == m_font) return;
		m_font = font;
		m_is_text_dirty = true;
	}
	void Label::setFontSize(float size)
	{
		if(size == m_font_size) return;
		m_font_size = size;
		m_is_size_dirty = true;
	}
	float Label::getFontSize()
	{
		return m_font_size;
	}
	
	void Label::setMaxWidthInPixels(int pixels)
	{
		if(pixels == m_max_width_px) return;
		m_max_width_px = pixels;
		m_is_text_dirty = true;
	}
	
	void Label::setCharsSpacingInPixels(int pixels)
	{
		if(pixels == m_char_spacing_px) return;
		m_char_spacing_px = pixels;
		m_is_text_dirty = true;
	}
	
	void Label::setLinesSpacingInPixels(int pixels)
	{
		if(pixels == m_line_spacing_px) return;
		m_line_spacing_px = pixels;
		m_is_text_dirty = true;
	}
	
	void Label::setTextAlignment(float alignment)
	{
		if(alignment == m_alignment) return;
		m_alignment = alignment;
		m_is_text_dirty = true;
	}
	
	void Label::setUnderlineInPixels(int width)
	{
		if(width == m_underline_px) return;
		m_underline_px = width;
		m_is_text_dirty = true;
	}
	
	void Label::setText(std::string const & text)
	{
		if(m_text == text) return;
		m_text = text;
		m_is_text_dirty = true;
	}
	
	std::string Label::getText()
	{
		return m_text;
	}
	
	void Label::setBGAlpha(uint8_t a)
	{
		m_bg_alpha = a;
	}
	void Label::updateContent()
	{
		if(m_is_text_dirty)
		{
			m_is_text_dirty = false;
			
			auto text32 = Zen::UTF8ToUnicode(m_text);
			
			auto style = FontRender::Style();
			style.alignment = m_alignment;
			style.word_space = m_char_spacing_px;
			style.line_space = m_line_spacing_px;
			style.width_limit = m_max_width_px;
			auto render = FontRender::Create(m_font, text32, style);
			
			auto w = render->getOutputWidth();
			auto h = render->getOutputHeight();
			
			m_is_texture_empty = (w == 0 || h == 0);
			
			if(!m_is_texture_empty)
			{
				auto set = FontRender::RenderSetting();
				set.underline = m_underline_px;
				set.fill = m_bg_alpha;
				auto image = render->render(w, h, set);
				
				if(m_texture == nullptr)
				{
					m_texture = Texture::Create();
				}
				m_texture->set(image.get());
			}
			
			m_is_size_dirty = true;
			m_RBuf.is_dirty = true;
		}
		if(m_is_size_dirty)
		{
			m_is_size_dirty = false;
			
			if(m_is_texture_empty)
			{
				m_texture_size = { 0.f, 0.f };
			}
			else
			{
				float s = m_font_size / m_font->getConfig().width;
				m_texture_size = { m_texture->size().w * s, m_texture->size().h * s };
			}
			
			if(!m_is_fixed_sized)
			{
				this->setScale2_(m_texture_size.w, m_texture_size.h);
			}
		}
	}
	
	/// content size {
	void Label::enableFixedSize(bool v)
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
	
	bool Label::isFixedSizeEnabled()
	{
		return m_is_fixed_sized;
	}
	
	void Label::setFixedSize(Size2 size)
	{
		m_fixed_size = size;
		if(m_is_fixed_sized)
		{
			this->setScale2_(size.w, size.h);
		}
	}
	
	Size2 Label::getFixedSize()
	{
		return m_fixed_size;
	}
	
	Size2 Label::getContentSize()
	{
		if(m_is_fixed_sized) return m_fixed_size;
		else
		{
			this->updateContent();
			return m_texture_size;
		}
	}
	/// } content size
	
	
	void Label::draw()
	{
		if(m_text.empty() || m_font == nullptr) return;
		
		this->updateContent();
		
		if(m_is_texture_empty) return;
		
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

	void Label::initR_()
	{
		m_RBuf.v.create(sizeof(s_coords), s_coords);
	}
	void Label::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			
			float x0 = 0;
			float y0 = 0;
			float x1 = m_texture->using_size().w;
			float y1 = m_texture->using_size().h;
			
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
		s->setUniform(sha->u_transform, getMatrix());
		s->drawArray(GL::eVertexMode::TriangleStrip, 0, 4);
	}
#elif ZEN_DRAW_METAL
	
	void Label::initR_()
	{
		m_RBuf.initBufferV4();
	}
	void Label::drawR_()
	{
		if(m_RBuf.is_dirty)
		{
			m_RBuf.is_dirty = false;
			
			float x0 = 0;
			float y0 = 0;
			float x1 = m_texture->using_size().w;
			float y1 = m_texture->using_size().h;
			
			if(m_is_flip_x) std::swap(x0, x1);
			if(m_is_flip_y) std::swap(y0, y1);
			
			m_RBuf.updateBufferV4(x0, y0, x1, y1);
		}
		
		this->updateMatrix();
		m_RBuf.updateMatrix(getMatrix(), getMatrixVersion(), false);
		float alpha = getAlpha() * DrawStack::Get()->getTopAlpha();
		m_RBuf.updateColor(getColor(), alpha, m_is_color_dirty);
		m_is_color_dirty = false;
		
		auto r = Metal::Render::Get();
		
		auto blends = MetalHelper::GetBlends(m_blend);
		
		auto pipe = Metal::PipeT::GetShared(m_is_grey_mode, m_texture->format(), blends.first, blends.second);
		
		r->activePipe(pipe->getID());
		
		if(m_texture)
			r->bindTexture(pipe->TextureIndex, m_texture->buffer().mt_buffer.getID());
		r->setVertexBuffer(pipe->VertAttribIndex, m_RBuf.v().getID(), 0);
		r->setVertexBuffer(pipe->UniformIndex, m_RBuf.u().getID(), 0);
		
		r->drawPrimitives(Metal::eVertexMode::TriangleStrip, 0, 4);
	}
#endif
	
}}

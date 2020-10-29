
#include "zen_vap2d_label.h"
#include "zen_utf8.h"
#include "zen_ticker.h"

namespace Zen { namespace Vap2d {

	Label::Label(std::string const & name)
	: FinalNode(name)
	{
		_initLabel();
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

	void Label::clearSizeDirty()
	{
		if(m_is_text_dirty)
		{
			m_is_text_dirty = false;

			auto text32 = Zen::UTF8ToUnicode(m_text);

			auto render = FontRender::Create
			(m_font, text32, m_alignment, m_char_spacing_px,
			 m_line_spacing_px, m_max_width_px);

			auto w = render->getOutputWidth();
			auto h = render->getOutputHeight();
			if(w == 0 || h == 0)
			{
				m_is_texture_empty = true;
				return;
			}

			auto image = render->render(w, h, 0, 0, m_underline_px);

			if(m_texture == nullptr)
			{
				m_texture = Texture::Create();
			}
			m_texture->set(image.get());
			m_is_size_dirty = true;
			m_is_texture_empty = false;

			m_is_buffer_dirty = true;
		}
		if(m_is_size_dirty)
		{
			m_is_size_dirty = false;
			float s = m_font_size / m_font->getConfig().width;
			this->_setScale2(m_texture->size().w * s, m_texture->size().h * s);
		}
	}

	void Label::draw()
	{
		if(m_text.empty() || m_font == nullptr) return;

		this->clearSizeDirty();

		if(m_is_texture_empty) return;

		this->_drawLabel();
	}
}}

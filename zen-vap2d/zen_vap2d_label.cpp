
#include "zen_vap2d_label.h"
#include "zen_utf8.h"
#include "zen_ticker.h"

namespace Zen { namespace Vap2d {

	Label::Label(std::string const & name)
	: Sprite(name)
	{
		this->setTexture(SharedTexture(new Texture));
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

	void Label::setTextureBackground(float grey)
	{
		if(grey == m_background_grey) return;
		m_background_grey = grey;
		m_is_text_dirty = true;
	}
	void Label::setUnderline(int width)
	{
		if(width == m_underline) return;
		m_underline = width;
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

	void Label::clearLabelDirty()
	{
		if(m_is_text_dirty)
		{
			m_is_text_dirty = false;

			auto text32 = Zen::UTF8ToUnicode(m_text);

			auto render = FontRender::Create
			(m_font, text32, m_alignment, m_char_spacing_px,
			 m_line_spacing_px, m_max_width_px);

			ImageData image;
			auto w = render->getOutputWidth();
			auto h = render->getOutputHeight();
			if(w == 0 || h == 0)
			{
				m_is_texture_empty = true;
				this->setSize(0, 0);
				return;
			}

			render->renderToImage(image, w+2, h+2, 1, 1, m_underline);

			if(image.format != EBPP::Grey) return;

			if(m_background_grey > 0.9f/255)
			{
				for(auto & i : image.buffer)
				{
					auto v = (int)((255 - i) * m_background_grey + i);
					i = (v > 255)?0xff:(uint8_t)v;
				}
			}

			m_texture->set(image.width, image.height, image.format, image.buffer.data());
			m_is_texture_dirty = true;
			m_is_size_dirty = true;
			m_is_texture_empty = false;
		}
		if(m_is_size_dirty)
		{
			float s = m_font_size / m_font->getConfig().width;
			this->setSize(m_texture->size().w * s, m_texture->size().h * s);
			m_is_size_dirty = false;
		}
	}

	void Label::draw()
	{
		if(m_text.empty() || m_font == nullptr) return;

		this->clearLabelDirty();
		if(m_is_texture_empty) return;

		Sprite::draw();
	}
}}

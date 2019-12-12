#pragma once

#include "zen_vap2d_sprite.h"
#include "zen_freetype.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Label : public Sprite {
	protected:
		std::string m_text;
		std::shared_ptr<FontBrush> m_font;
		int m_max_width_px = 0;
		int m_char_spacing_px = 0;
		int m_line_spacing_px = 0;
		float m_alignment = 0;
		float m_background_grey = 0;
		float m_font_size = 0;
		int m_underline = 0;
		bool m_is_text_dirty = true;
		bool m_is_size_dirty = true;
		bool m_is_texture_empty = true;
	protected:
		using Sprite::setTexture;
	public:
		Label(std::string const & name = "label");

		void setFont(std::shared_ptr<FontBrush>);

		void setMaxWidthInPixels(int pixels);

		void setCharsSpacingInPixels(int pixels);

		void setLinesSpacingInPixels(int pixels);

		void setTextAlignment(float alignment);

		void setTextureBackground(float grey);

		void setUnderline(int width);

		void setText(std::string const & str);
		std::string getText();

		void setFontSize(float size);
		float getFontSize();

		virtual void draw() override;
	};
}}

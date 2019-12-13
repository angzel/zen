#pragma once

#include "zen_vap2d_sprite.h"
#include "zen_freetype.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Label : public FinalNode, public SizeView, public Colorful, public Flipable {
	public:
		Label(std::string const & name = "label");

		void setFont(std::shared_ptr<FontBrush>);

		void setTextAlignment(float alignment);

		void setMaxWidthInPixels(int pixels);

		void setCharsSpacingInPixels(int pixels);

		void setLinesSpacingInPixels(int pixels);

		void setUnderlineInPixels(int width);

		void setFontSize(float size);
		float getFontSize();

		void setText(std::string const & str);
		std::string getText();

		void clearSizeDirty();

		virtual void draw() override;

	protected:
		std::string m_text;
		std::shared_ptr<FontBrush> m_font;
		float m_alignment = 0;
		int m_max_width_px = 0;
		int m_char_spacing_px = 0;
		int m_line_spacing_px = 0;
		int m_underline_px = 0;
		bool m_is_text_dirty = true;
		bool m_is_texture_empty = true;
	protected:
		SharedTexture m_texture;
		float m_font_size = 16;
		bool m_is_size_dirty = true;
		
	protected:
		SpriteBuffer m_gpus;
		bool m_is_buffer_dirty = false;

		void _initLabel();
		void _drawLabel();
	};
}}

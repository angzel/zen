#pragma once

#include "zen_vap_sprite.h"
#include "zen_freetype.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {

	class Label : public Node2d, public View2d, public Colorful, public Flipable {
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

		void updateContent();

		void enableFixedSize(bool);
		
		bool isFixedSizeEnabled();
		
		void setFixedSize(Size2 size);
		
		Size2 getFixedSize();

		Size2 getContentSize();
		
		virtual void draw() override;

		void setBGAlpha(uint8_t a);
	protected:
		std::string m_text;
		std::shared_ptr<FontBrush> m_font;
		float m_alignment = 0;
		int m_max_width_px = 0;
		int m_char_spacing_px = 0;
		int m_line_spacing_px = 0;
		int m_underline_px = 0;
		
		TextureShared m_texture;
		float m_font_size = 16;
		Size2 m_fixed_size = { 0, 0 };
		Size2 m_texture_size = { 0, 0 };
		bool m_is_fixed_sized = false;
		
		bool m_is_text_dirty = true;
		bool m_is_texture_empty = true;

		bool m_is_size_dirty = true;
		
		uint8_t m_bg_alpha = 0;
	protected:
		RBufferTexture m_RBuf;
		
		void initR_();
		void drawR_();
	};
}}

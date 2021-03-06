/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

/**
 add external/<...>/include/freetype2 to the include path
 */
#include "ft2build.h"
#include "freetype.h"

#include <vector>
#include <map>
#include <memory>

#include "zen_exception.h"

namespace Zen {
	
	struct FontChar
	{
		uint32_t unicode = 0;
		std::vector<uint8_t> bitmap;

		int width = 0;
		int height = 0;
		
		int bitmap_left = 0;
		int bitmap_top = 0;
		
		int view_width = 0;
		int view_height = 0;
	};

	struct FontConfig
	{
		int   width  = 16;
		int   height = 16;
		float italic = 0; // [0., 1.]
		float bold_x = 0; // px
		float bold_y = 0; // px
	};
	
	class Font
	{
	protected:
		Font() = default;
		virtual ~Font() = default;
	public:
		struct Info {
			int width = 0;
			int height = 0;
			int text_height = 0;
			int ascender = 0;
			int max_advance = 0;
		};

		virtual std::shared_ptr<FontChar>
		createCharBitmap(uint32_t unicode, FontConfig const & config) = 0;

		virtual Info getInfo(FontConfig const & config) = 0;
	};
	
	class FontLibrary
	{
	public:
		static FontLibrary * Get();
		
		virtual std::shared_ptr<Font> createFont(std::vector<uint8_t> const & data) = 0;
		
	protected:
		FontLibrary() = default;
		virtual ~FontLibrary() = default;
	};

	class FontBrush
	{
	public:
		static std::shared_ptr<FontBrush> Create(std::shared_ptr<Font> font, FontConfig config);

		std::shared_ptr<FontChar> getCharBitmap(uint32_t unicode);

		std::shared_ptr<Font> getFont() const;

		FontConfig const & getConfig() const;

		Font::Info const & getInfo() const;

	public:
		virtual ~FontBrush() = default;

	protected:
		FontBrush() = default;
	protected:
		FontConfig m_config;
		std::shared_ptr<Font> m_font;
		std::map<uint32_t, std::shared_ptr<FontChar> > m_chars;
		Font::Info m_font_info;
//		int m_line_base = 0;
//		int m_text_height = 0;
	};

	class Fonts
	{
	public:
		static Fonts * Get();

		std::shared_ptr<Font> setFont(std::string const & name, std::vector<uint8_t> const & data);

		std::shared_ptr<Font> getFont(std::string const & name);
	protected:
		Fonts() = default;
		virtual ~Fonts() = default;
		
	protected:
		std::map<std::string, std::shared_ptr<Font> > m_fonts;
	};
}

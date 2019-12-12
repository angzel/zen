/*
 Copyright (c) 2013 ClearSky G.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

#include "zen_object.h"
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
		float italic = 0;
		float bold_x = 0;
		float bold_y = 0;
	};
	
	class Font : public Zen::Object
	{
	protected:
		Font() = default;
		
	public:
		struct Info {
			int width = 0;
			int height = 0;
			int base_line_height = 0;
			int line_height = 0;
			int forward = 0;
		};

		virtual std::shared_ptr<FontChar>
		createCharBitmap(uint32_t unicode, FontConfig const & config) = 0;

		virtual Info getInfo(FontConfig const & config) = 0;
	};
	
	class FontLibrary : public Zen::Object
	{
	public:
		static FontLibrary * GetDefault();
		
		virtual std::shared_ptr<Font> createFont(std::vector<uint8_t> const & data) = 0;
		
	protected:
		FontLibrary() = default;
		virtual ~FontLibrary() = default;
	};

	class FontBrush : public Zen::Object
	{
	protected:
		FontConfig m_config;
		std::shared_ptr<Font> m_font;
		std::map<uint32_t, std::shared_ptr<FontChar> > m_chars;
		Font::Info m_font_info;
		int m_line_base = 0;
		int m_line_height = 0;
	public:
		static std::shared_ptr<FontBrush> Create(std::shared_ptr<Font> font, FontConfig const & config);

		std::shared_ptr<FontChar> getCharBitmap(uint32_t unicode);

		std::shared_ptr<Font> getFont() const;

		FontConfig const & getConfig() const;

		Font::Info const & getInfo() const;
	protected:
		FontBrush() = default;
	};

	class Fonts : public Zen::Object
	{
	public:
		static Fonts * GetDefault();

		std::shared_ptr<Font> setFont(std::string const & name, std::vector<uint8_t> const & data);

		std::shared_ptr<Font> getFont(std::string const & name);
	protected:

		std::map<std::string, std::shared_ptr<Font> > m_fonts;
	};
}

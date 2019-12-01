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
	
	struct FontCharacter
	{
		std::vector<uint8_t> bitmap;
		int width, height;
		
		int bitmap_left, bitmap_top;
		
		int view_width, view_height; // 26.6
	};

	struct FontConfig
	{
		int charW = 16, charH = 16;
		float italicValue = 0, boldX = 0, boldY = 0;
		bool isVerticalWriting = false;
	};
	
	class Font : public Zen::Object
	{
	protected:
		Font() = default;
		
	public:
		virtual std::shared_ptr<FontCharacter>
		loadCharacter
		(uint32_t unicode, int width, int height,
		 bool vertical_writing = false,
		 float italic = 0, float boldx = 0, float boldy = 0) = 0;

		virtual std::shared_ptr<FontCharacter>
		loadCharacter(uint32_t unicode, FontConfig const & config) = 0;
	};
	
	class FontLibrary : public Zen::Object
	{
	public:
		static FontLibrary * GetDefault();
		
		virtual std::shared_ptr<Font>
		loadFont(std::vector<uint8_t> const & font_data) = 0;
		
	protected:
		FontLibrary() = default;
	};

	class FontCache : public Zen::Object
	{
	public:
		static FontCache * GetDefault();

		virtual std::shared_ptr<Font> setFont(std::string const & name, std::vector<uint8_t> const & font_data) = 0;

		virtual std::shared_ptr<Font> getFont(std::string const & name) = 0;
	};
}

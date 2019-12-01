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

#include "zen_ft_library.h"
#include "zen_log.h"

#include FT_OUTLINE_H
#include FT_LIST_H

namespace Zen {

	struct FontFaceInner : public Font
	{
		std::vector<uint8_t> mMemory;
		FT_Face mFT_Face = nullptr;

		FontFaceInner(FT_Library library, const std::vector<uint8_t> &font_data)
		{
			mMemory = font_data;
			FT_Face face = nullptr;
			auto eno = FT_New_Memory_Face(library, mMemory.data(), mMemory.size(), 0, &face);
			mustsn(eno == 0, "FT_New_Memory_Face ERROR", eno);

			mFT_Face = face;
		}

		~FontFaceInner()
		{
			if(mFT_Face) FT_Done_Face(mFT_Face);
		}
		
		FT_GlyphSlot loadGlyph(uint32_t unicode, int width, int height, float italic, float boldx, float boldy, bool vertical)
		{
			auto eno = FT_Set_Char_Size(mFT_Face, width << 6, height <<6, 72, 72);
			
				//			if(eno == 23)
				//				eno = FT_Set_Char_Size(mFont, 16<<6, 16<<6, 72, 72);
				//			if(eno == 23)
				//				eno = FT_Set_Char_Size(mFont, 18<<6, 18<<6, 72, 72);
				//			if(eno == 23)
				//				eno = FT_Set_Char_Size(mFont, 20<<6, 20<<6, 72, 72);
				//			if(eno == 23)
				//				eno = FT_Set_Char_Size(mFont, 24<<6, 24<<6, 72, 72);
			
			mustsn(eno == 0, "FT_Set_Char_Size error", eno);
			
			FT_Int32 type = 0;

			FT_Matrix matrix;
			if(italic != 0)
			{
				matrix.xx = 0x10000L;
				matrix.xy = (int)(italic * (float)0x10000L);
				matrix.yx = 0;
				matrix.yy = 0x10000L;
				FT_Set_Transform(mFT_Face, &matrix, 0 );
				
				type = FT_LOAD_TARGET_NORMAL;
			}
			else
			{
				type = FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_TARGET_NORMAL;
			}

			if(vertical) type |= FT_LOAD_VERTICAL_LAYOUT;
			
			auto index = FT_Get_Char_Index(mFT_Face, (FT_ULong)unicode);
			eno = FT_Load_Glyph(mFT_Face, index, type);
			
			mustsn(eno == 0, "FT_Load_Glyph error", eno);
			
			if (mFT_Face->glyph->format == FT_GLYPH_FORMAT_OUTLINE )
			{
				FT_Outline_EmboldenXY(&mFT_Face->glyph->outline, (int)(boldx * 64), (int)(boldy*64));
			}
			
			if(mFT_Face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
			{
				eno = FT_Render_Glyph(mFT_Face->glyph, FT_RENDER_MODE_LIGHT);
				mustsn(eno == 0, "FT_Render_Glyph error", eno);
			}
			return mFT_Face->glyph;
		}

		virtual std::shared_ptr<FontCharacter>
		loadCharacter(uint32_t unicode, FontConfig const & config) override
		{
			return this->loadCharacter
			(unicode, config.charW, config.charH,
			 config.isVerticalWriting,
			 config.italicValue, config.boldX, config.boldY
			 );
		}

		virtual std::shared_ptr<FontCharacter>
		loadCharacter
		(uint32_t unicode, int width, int height,
		 bool vertical_writing = false,
		 float italic = 0, float boldx = 0, float boldy = 0) override
		{
			auto glyph = loadGlyph(unicode, width, height, italic, boldx, boldy, vertical_writing);
			auto & src = glyph->bitmap;
			auto fc = new FontCharacter;
			
			fc->width = src.width;
			fc->height = src.rows;
			fc->bitmap_left = (int)glyph->bitmap_left;
			fc->bitmap_top = (int)glyph->bitmap_top;
			fc->view_width = (int)(glyph->advance.x>>6);
			fc->view_height = (int)(glyph->advance.y>>6);

			if(fc->view_width < 0 || fc->view_height < 0)
			{
				LogV("***************** < 0 font");
			}
			
			auto size = fc->width * fc->height;
			fc->bitmap.resize(size);

			if(src.num_grays == 256)
			{
				if(src.pitch == src.width)
				{
					::memcpy(fc->bitmap.data(), src.buffer, size);
				}
				else if(src.pitch > 0)
				{
					auto buffer = src.buffer;
					auto data = fc->bitmap.data();
					for(size_t i = 0; i < fc->height; ++i)
					{
						::memcpy(data, buffer, fc->width);
						data += fc->width;
						buffer += src.pitch;
					}
				}
				else
				{
					auto buffer = src.buffer - src.rows * src.pitch; // pitch < 0
					auto data = fc->bitmap.data();
					for(size_t i = 0; i < fc->height; ++i)
					{
						buffer += src.pitch;
						::memcpy(data, buffer, fc->width);
						data += fc->width;
					}
				}
			}
			else if(src.num_grays == 2)
			{
				if(src.pitch > 0)
				{
					auto buffer = src.buffer;
					auto data = fc->bitmap.data();
					for(size_t i = 0; i < fc->height; ++i)
					{
						for(size_t j = 0; j < fc->width; ++j)
						{
							data[j] = (buffer[j>>3] & (0x80>>(j&0x7)))?255:0;
						}
						buffer += src.pitch;
						data += fc->width;
					}
				}
				else
				{
					
					auto buffer = src.buffer - src.rows * src.pitch;
					auto data = fc->bitmap.data();
					for(size_t i = 0; i < fc->height; ++i)
					{
						buffer += src.pitch;
						for(size_t j = 0; j < fc->width; ++j)
						{
							data[j] = (buffer[j>>3] & (0x80>>(j&0x7)))?255:0;
						}
						data += fc->width;
					}
				}
			}
			else
			{
				Zen::LogW("unsupported freetype pixel mode:%d grays:%d", (int)src.pixel_mode, (int)src.num_grays);
				fc->width = fc->height = 0;
				fc->bitmap.clear();
			}
			return Zen::MakeShared(fc);
		}
	};
	
	class FontLibraryInner : public FontLibrary
	{
		FT_Library mFT_Library = nullptr;
	public:
		
		std::shared_ptr<Font> loadFont(const std::vector<uint8_t> &font_data) override
		{
			return Zen::MakeShared((Font*)new FontFaceInner(mFT_Library, font_data));
		}
		
		FontLibraryInner()
		{
			auto eno = FT_Init_FreeType(&mFT_Library);
			mustsn(eno == 0, "FT_Init_FreeType ERROR", eno);
		}
		~FontLibraryInner()
		{
			if(mFT_Library) FT_Done_FreeType(mFT_Library);
		}
		
	};
	
	static FontLibraryInner * library;
	
	FontLibrary * FontLibrary::GetDefault()
	{
		if(library) return library;
		auto temp = new FontLibraryInner;
		return (library = temp);
	}

	class FontCacheInner : public FontCache
	{
		std::map<std::string, std::shared_ptr<Font> > mCache;
	public:
		FontCacheInner()
		{
		}

		virtual std::shared_ptr<Font> setFont(std::string const & name, std::vector<uint8_t> const & font_data) override
		{
			auto font = FontLibrary::GetDefault()->loadFont(font_data);
			mCache[name] = font;
			return font;
		}

		virtual std::shared_ptr<Font> getFont(std::string const & name) override
		{
			auto iter = mCache.find(name);
			if(iter == mCache.end()) return nullptr;
			return iter->second;
		}
	};
	
	FontCache * FontCache::GetDefault()
	{
		static auto me = new FontCacheInner;
		return me;
	}

}

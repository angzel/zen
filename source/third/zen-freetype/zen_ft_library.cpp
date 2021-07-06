/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_ft_library.h"
#include "zen_log.h"

#include FT_OUTLINE_H
#include FT_LIST_H

namespace Zen {
	
	struct FontInner : public Font
	{
		std::vector<uint8_t> m_data;
		FT_Face mFT_Face = nullptr;
		
		FontInner(FT_Library library, const std::vector<uint8_t> & data) throw(std::exception)
		{
			m_data = data;
			FT_Face face = nullptr;
			auto eno = FT_New_Memory_Face(library, m_data.data(), m_data.size(), 0, &face);
			mustsn(eno == 0, "FT_New_Memory_Face ERROR", eno);
			
			mFT_Face = face;
		}
		
		virtual ~FontInner()
		{
			if(mFT_Face) FT_Done_Face(mFT_Face);
		}
		
		void loadGlyph(uint32_t unicode, FontConfig const & c);
		
		virtual Info getInfo(FontConfig const & config) override
		{
			loadGlyph(uint32_t(32), config);
			
			Info info;
			auto & m = mFT_Face->size->metrics;
			info.text_height = (int)m.height>>6;
			info.ascender = (int)m.ascender>>6;
			info.max_advance = (int)m.max_advance>>6;
			info.width = m.x_ppem;
			info.height = m.y_ppem;
			return info;
		}
		
		virtual std::shared_ptr<FontChar>
		createCharBitmap(uint32_t unicode, FontConfig const & config) override;
	};
	
	class FontLibraryInner : public FontLibrary
	{
		FT_Library mFT_Library = nullptr;
	public:
		
		FontLibraryInner()
		{
			auto eno = FT_Init_FreeType(&mFT_Library);
			mustsn(eno == 0, "FT_Init_FreeType ERROR", eno);
		}
		virtual ~FontLibraryInner()
		{
			if(mFT_Library) FT_Done_FreeType(mFT_Library);
		}
		std::shared_ptr<Font> createFont(const std::vector<uint8_t> &data) override
		{
			return std::shared_ptr<Font>(new FontInner(mFT_Library, data));
		}
	};
	
	FontLibrary * FontLibrary::Get()
	{
		static auto single = new FontLibraryInner;
		return single;;
	}
	
	Fonts * Fonts::Get()
	{
		static auto single = new Fonts;
		return single;;
	}
	
	std::shared_ptr<Font> Fonts::setFont(std::string const & name, std::vector<uint8_t> const & data)
	{
		auto font = FontLibrary::Get()->createFont(data);
		m_fonts[name] = font;
		return font;
	}
	
	std::shared_ptr<Font> Fonts::getFont(std::string const & name)
	{
		auto iter = m_fonts.find(name);
		if(iter == m_fonts.end()) return nullptr;
		return iter->second;
	}
	
	std::shared_ptr<FontBrush> FontBrush::Create(std::shared_ptr<Font> font, FontConfig config)
	{
		auto fb = std::shared_ptr<FontBrush>(new FontBrush);
		fb->m_font = font;
		fb->m_config = config;
		fb->m_font_info = font->getInfo(config);
		return fb;
	}
	
	std::shared_ptr<FontChar> FontBrush::getCharBitmap(uint32_t unicode)
	{
		auto iter = m_chars.find(unicode);
		if(iter != m_chars.end()) return iter->second;
		auto c = m_font->createCharBitmap(unicode, m_config);
		m_chars[unicode] = c;
		return c;
	}
	
	std::shared_ptr<Font> FontBrush::getFont() const
	{
		return m_font;
	}
	
	Font::Info const & FontBrush::getInfo() const
	{
		return m_font_info;
	}
	
	FontConfig const & FontBrush::getConfig() const
	{
		return m_config;
	}
	
	std::shared_ptr<FontChar>
	FontInner::createCharBitmap(uint32_t unicode, FontConfig const & config)
	{
		loadGlyph(unicode, config);
		
		auto glyph = mFT_Face->glyph;
		auto & src = glyph->bitmap;
		auto res = std::shared_ptr<FontChar>(new FontChar);
		auto fc = res.get();
		
		fc->unicode = unicode;
		fc->width = src.width;
		fc->height = src.rows;
		fc->bitmap_left = (int)glyph->bitmap_left;
		fc->bitmap_top = (int)glyph->bitmap_top;
		fc->view_width = (int)(glyph->advance.x>>6);
		fc->view_height = (int)(glyph->advance.y>>6);
		
		if(fc->view_width < 0 || fc->view_height < 0)
		{
			LogV("Warn: font view size (%d %d) < 0", fc->view_width, fc->view_height);
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
			Zen::LogW("unsupported font bitmap: pixel-mode=%d grays=%d",
					  (int)src.pixel_mode, (int)src.num_grays);
			fc->width = fc->height = 0;
			fc->bitmap.clear();
		}
		return res;
	}
	void FontInner::loadGlyph(uint32_t unicode, FontConfig const & c)
	{
		auto eno = FT_Set_Char_Size(mFT_Face, c.width << 6, c.height <<6, 72, 72);
		
		mustsn(eno == 0, "FT_Set_Char_Size error", eno);
		
		FT_Int32 type = 0;
		
		FT_Matrix matrix;
		if(c.italic != 0)
		{
			matrix.xx = 0x10000L;
			matrix.xy = (int)(c.italic * (float)0x10000L);
			matrix.yx = 0;
			matrix.yy = 0x10000L;
			FT_Set_Transform(mFT_Face, &matrix, 0 );
			
			type = FT_LOAD_TARGET_NORMAL;
		}
		else
		{
			type = FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_TARGET_NORMAL;
		}
		/** no vertical
		 if(c.is_vertical) type |= FT_LOAD_VERTICAL_LAYOUT;
		 */
		auto index = FT_Get_Char_Index(mFT_Face, (FT_ULong)unicode);
		eno = FT_Load_Glyph(mFT_Face, index, type);
		
		mustsn(eno == 0, "FT_Load_Glyph error", eno);
		
		if (mFT_Face->glyph->format == FT_GLYPH_FORMAT_OUTLINE )
		{
			FT_Outline_EmboldenXY(&mFT_Face->glyph->outline, (int)(c.bold_x * 64), (int)(c.bold_y*64));
		}
		
		if(mFT_Face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			eno = FT_Render_Glyph(mFT_Face->glyph, FT_RENDER_MODE_LIGHT);
			mustsn(eno == 0, "FT_Render_Glyph error", eno);
		}
	}
}

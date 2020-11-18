
#include "zen_ft_render.h"
#include "zen_log.h"

namespace Zen {
	
	/** image: grey format */
	static void WriteChar(Image * image, FontChar * a, int x, int y)
	{
		int iw = (int)image->width();
		int ih = (int)image->height();
		
		x += a->bitmap_left; // image x
		y -= a->bitmap_top;
		
		auto w = a->width; // used bitmap w
		auto h = a->height; // used bitmap h
		
		if(x + w <= 0 || x >= iw) return;
		if(y + h <= 0 || y >= ih) return;
		
		int ax = (x < 0)?-x:0; // bitmap x
		int ay = (y < 0)?-y:0; // bitmap y
		
		if(x + w >= iw) w = iw - x; // right out
		if(y + h >= ih) h = ih - y;
		w -= ax; // left out
		h -= ay;
		
		auto dst = image->data() + iw * (y + ay) + (x + ax);
		auto src = a->bitmap.data() + ay * a->width + ax;
		for(int i = 0; i < h; ++i)
		{
//			for(int i = 0; i < w; ++i)
//			{
//				if(src[i] > dst[i]) dst[i] = src[i];
//			}
			::memcpy(dst, src, w);
			dst += iw;
			src += a->width;
		}
	}
	static void WriteLine(Image * image, int x, int y, int w, int underline)
	{
		int iw = (int)image->width();
		int ih = (int)image->height();

		if(y >= ih) return;
		if(y + underline > ih) underline = ih - y;
		if(x < 0) { w = w + x; x = 0; }
		if(x + w > iw) w = iw - x;
		
		auto b = image->data() + (y * iw) + x;
		for(int i = 0; i < underline; ++i)
		{
			::memset(b, 0xff, w);
			b += iw;
		}
	}
	
	struct FontRenderLineInfo
	{
		int width = 0;
		int tail = 0;
		std::vector<std::shared_ptr<FontChar> > chars;
	};
	
	class FontRenderInner : public FontRender
	{
	protected:
		
		int m_out_width;
		int m_out_height;
		
		std::vector<FontRenderLineInfo> m_lines;
		
		std::shared_ptr<FontBrush> m_font_brush;
		
		std::u32string m_text;
		
		float m_alignment;
		int m_char_spacing;
		int m_max_line_width;
		int m_line_spacing;
		bool m_auto_break;
		
	public:
		FontRenderInner
		(std::shared_ptr<FontBrush> font,
		 std::u32string const & text,
		 float alignment, /* 0.0 left(top), to 1.0 right(bottom) */
		 int char_spacing, // px as font-size
		 int line_spacing,
		 int max_line_width)
		{
			m_font_brush = font;
			m_text = text;
			m_alignment = alignment;
			m_char_spacing = char_spacing;
			m_max_line_width = max_line_width;
			m_line_spacing = line_spacing;
			
			m_out_width = m_out_height = 0;
			if(max_line_width)
			{
				this->initLinesBreak(m_text);
			}
			else
			{
				this->initLines(m_text);
			}
			
			auto height = (m_font_brush->getInfo().line_height + m_line_spacing) * m_lines.size();
			m_out_height = (int)height;
		}

		virtual std::shared_ptr<Image> render
		(size_t width, size_t height, int x_off, int y_off, int underline)
		override
		{
			if(width == 0) width = getOutputWidth();
			if(height == 0) height = getOutputHeight();

			auto image = Image::Create(ePixel::Grey, width, height);

			_writeImageH(image.get(), x_off, y_off, underline);

			return image;
		}
		
		virtual int getOutputWidth() override
		{
			return m_out_width;
		}
		
		virtual int getOutputHeight() override
		{
			return m_out_height;
		}
		virtual size_t getLinesCount() override
		{
			return m_lines.size();
		}
		
	protected:
		
		void _newLine(FontRenderLineInfo & _line)
		{
			if(_line.width < 0) _line.width = 0;
			_line.width = std::max(_line.width, _line.tail);
			
			if(_line.width > m_out_width) m_out_width = _line.width;
			
			m_lines.push_back(_line);
			
			_line = FontRenderLineInfo();
		}
		
		void initLines(std::u32string const & text)
		{
			FontRenderLineInfo _line;
			for(auto & i : text)
			{
				if(i == '\n')
				{
					_newLine(_line);
					continue;
				}
				
				auto ch = m_font_brush->getCharBitmap(i);
				if(!ch)
				{
					Zen::LogW("errro char %d", (int)i);
					continue;
				}
				
				auto w = ch->view_width;
				if(w <= 0)
				{
					Zen::LogW("error char [U%s], w[%d] h[%d]", (int)i, (int)ch->view_width, (int)ch->view_height);
					continue;
				}
				
				
				if(_line.chars.size())
				{
					_line.tail = std::max(_line.tail, _line.width + m_char_spacing + ch->bitmap_left + ch->width);
					_line.width += m_char_spacing + w;
				}
				else
				{
					_line.tail = ch->bitmap_left + ch->width;
					_line.width = w;
				}
				_line.chars.push_back(ch);
			}
			if(_line.width < 0) _line.width = 0;
			_newLine(_line);
		}
		void initLinesBreak(std::u32string const & text)
		{
			FontRenderLineInfo _line;
			for(auto & i : text)
			{
				if(i == '\n')
				{
					_newLine(_line);
					continue;
				}
				auto ch = m_font_brush->getCharBitmap(i);
				if(!ch)
				{
					Zen::LogW("error char %d", (int)i);
					continue;
				}
				
				auto w = ch->view_width;
				if(w <= 0)
				{
					Zen::LogW("error char [U%s], w[%d] h[%d]", (int)i, (int)ch->view_width, (int)ch->view_height);
					continue;
				}
				if(_line.chars.size())
				{
					auto tail = std::max(_line.tail, _line.width + m_char_spacing + ch->bitmap_left + ch->width);
					auto width = _line.width + m_char_spacing + w;
					if(width < m_max_line_width && tail > m_max_line_width)
					{
						_line.tail = tail;
						_line.width = width;
						_line.chars.push_back(ch);
						continue;
					}
					_newLine(_line);
				}
				_line.tail = ch->bitmap_left + ch->width;
				_line.width = w;
				_line.chars.push_back(ch);
			}
			_newLine(_line);
		}
		
		
		void _writeImageH(Image * image, int x_off, int y_off)
		{
			int y = y_off + m_font_brush->getInfo().base_line_height + m_line_spacing/2;
			
			int w = (int)image->width();
			
			for(auto & line : m_lines)
			{
				int x = int(m_alignment * float(w - line.width)) + x_off;
				
				for(auto i : line.chars)
				{
					WriteChar(image, i.get(), x, y);
					x += i->view_width + m_char_spacing;
				}
				
				y += m_font_brush->getInfo().line_height + m_line_spacing;
			}
		}
		
		void _writeImageH(Image * image, int x_off, int y_off, int underline)
		{
			int y = y_off + m_font_brush->getInfo().base_line_height + m_line_spacing/2;
			
			int w = (int)image->width();
			
			for(auto & line : m_lines)
			{
				int x = int(m_alignment * float(w - line.width)) + x_off;
				int line_x = x;
				for(auto i : line.chars)
				{
					WriteChar(image, i.get(), x, y);
					x += i->view_width + m_char_spacing;
				}
				WriteLine(image, line_x, y, line.width, underline);
				
				y += m_font_brush->getInfo().line_height + m_line_spacing;
			}
		}
		
		/** // no vertical
		 void _writeImageV(ImageData & image, int x_off, int y_off)
		 {
		 int x =  x_off + m_line_spacing/2;
		 int h = (int)image.height;
		 
		 for(auto & line : m_lines)
		 {
		 int y = (int)(m_alignment * (float)(h - line.width)) + y_off;
		 
		 // align * (image.height() - char_height / 2 - line.points) + y_off + char_height / 4
		 
		 for(auto i : line.chars)
		 {
		 y += i->view_height;
		 WriteChar(image, i.get(), x, y);
		 y += m_char_spacing;
		 }
		 x += m_font_brush->getInfo().line_height + m_line_spacing;
		 }
		 }
		 */
	};
	
	std::shared_ptr<FontRender> FontRender::Create
	(std::shared_ptr<FontBrush> brush,
	 std::u32string const & text,
	 float alignment, /* 0.0 left(top), to 1.0 right(bottom) */
	 int char_spacing, // px as font-size
	 int line_spacing,
	 int line_width)
	{
		auto me = new FontRenderInner(brush, text, alignment, char_spacing, line_spacing, line_width);
		return std::shared_ptr<FontRender>(me);
	}
}

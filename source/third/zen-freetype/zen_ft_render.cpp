
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
			for(int i = 0; i < w; ++i)
			{
				if(src[i] > dst[i]) dst[i] = src[i];
			}
//			::memcpy(dst, src, w);
			dst += iw;
			src += a->width;
		}
	}
	
	static void sWriteUnderline(Image * image, int x, int y, int w, int underline)
	{
		underline = abs(underline);
		int iw = (int)image->width();
		int ih = (int)image->height();
		int top = y;
		int bot = y + underline;
		if(top > bot) std::swap(top, bot);
		if(top >= ih || bot <= 0) return; // y outside
		int left = x;
		int right = x + w;
		if(left > right) std::swap(left, right);
		if(left >= iw || right <= 0) return; // x outside
		if(left < 0) left = 0;
		if(right > iw) right = iw;
		if(top < 0) top = 0;
		if(bot >= ih) bot = ih;
		
		auto len = right - left; // >0
		auto src = image->data() + (top * iw + left);
		while(top++!=bot)
		{
			::memset(src, 0xff, len);
		}
	}
	
	class FontRenderInner : public FontRender
	{
	public:
		struct LineInfo
		{
			int left_limit = 0;
			int right_limit = 0;
			std::vector<std::shared_ptr<FontChar> > chars;
		};
	protected:
		int m_out_width = 0;
		int m_out_height = 0;
		int m_out_top = 0;
		
		std::vector<LineInfo> m_lines;
		
		std::shared_ptr<FontBrush> m_font_brush;
		
		std::u32string m_text;

		Style m_style;
		
	public:
		FontRenderInner
		(std::shared_ptr<FontBrush> font,
		 std::u32string const & text,
		 Style const & style)
		{
			m_font_brush = font;
			m_text = text;
			m_style = style;
			
			this->initLinesAutoBreak();
			
			if(m_lines.size())
			{
				int text_height = m_font_brush->getInfo().text_height;
				int top = (text_height + m_style.line_space) * (int)(m_lines.size() - 1);
				if(top < 0)
				{
					m_out_top = top;
					m_out_height = text_height - m_out_top;
				}
				else {
					m_out_top = 0;
					m_out_height = top + text_height;
				}
			}
			else m_out_height = 0;
			
			if(m_out_width < 0) m_out_width = 0;
		}

		virtual std::shared_ptr<Image> render
		(size_t width, size_t height, RenderSetting const & set)
		override
		{
			if(width == 0) width = std::max(getOutputWidth(), size_t(1));
			if(height == 0) height = std::max(getOutputHeight(), size_t(1));

			auto image = Image::CreateWidthByte(ePixel::Grey, width, height, set.fill);

			_writeImageH(image.get(), set.x_off, set.y_off, set.underline);

			return image;
		}
		
		virtual size_t getOutputWidth() override
		{
			return m_out_width;
		}
		
		virtual size_t getOutputHeight() override
		{
			return m_out_height;
		}
		virtual size_t getLinesCount() override
		{
			return m_lines.size();
		}
		
	protected:
		
		void _newLine(LineInfo & _line)
		{
			int width = abs(_line.right_limit - _line.left_limit);
			if(width > m_out_width) m_out_width = width;
			
			m_lines.push_back(_line);
			
			_line = LineInfo();
		}
		
		void initLinesAutoBreak()
		{
			LineInfo _line;
			int x = 0;
			for(auto & i : m_text)
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
				
				if(ch->view_width <= 0)
				{
					Zen::LogW("error char [U%s], w[%d] h[%d]", (int)i, (int)ch->view_width, (int)ch->view_height);
					continue;
				}
				if(_line.chars.size())
				{
					int left = x + m_style.word_space + ch->bitmap_left;
					int right = left + ch->width;
					
					int left_limit = std::min(_line.left_limit, left);
					int right_limit = std::max(_line.right_limit, right);
					
					if(m_style.width_limit && right_limit - left_limit > m_style.width_limit)
					{
						_newLine(_line);
					}
					else
					{
						_line.left_limit = left_limit;
						_line.right_limit = right_limit;
						x += m_style.word_space + ch->view_width;
						_line.chars.push_back(ch);
						continue;
					}
				}
				/// here _line.chars.size() == 0
				
				_line.left_limit = ch->bitmap_left;
				_line.right_limit = _line.left_limit + ch->width;
				x = ch->view_width;
				
				_line.chars.push_back(ch);
			}
			_newLine(_line);
		}
		
		void _writeImageH(Image * image, int x_off, int y_off, int underline)
		{
			int y = y_off + m_font_brush->getInfo().ascender - m_out_top;
			
			int w = (int)image->width();
			
			for(auto & line : m_lines)
			{
				int width = line.right_limit - line.left_limit;

				int line_x = int(m_style.alignment * float(w - width)) + x_off;
				int x = line_x - line.left_limit;
				for(auto i : line.chars)
				{
					WriteChar(image, i.get(), x, y);
					x += i->view_width + m_style.word_space;
				}
				if(underline) sWriteUnderline(image, line_x, y, width, underline);
				
				y += m_font_brush->getInfo().text_height + m_style.line_space;
			}
		}
		
	};
	
	std::shared_ptr<FontRender> FontRender::Create
	(std::shared_ptr<FontBrush> brush,
	 std::u32string const & text,
	 Style const & style)
	{
		auto me = new FontRenderInner(brush, text, style);
		return std::shared_ptr<FontRender>(me);
	}
}

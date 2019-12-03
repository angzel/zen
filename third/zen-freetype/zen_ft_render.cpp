
#include "zen_ft_render.h"
#include "zen_log.h"

namespace Zen {

	/** image: grey format */
	static void WriteChar(ImageData & image, FontCharacter * a, int x, int y)
	{
		int iw = image.width;
		int ih = image.height;

		x += a->bitmap_left; // image x
		y -= a->bitmap_top; // image y

		auto w = a->width; // used bitmap w
		auto h = a->height; // used bitmap h

		if(x + w <= 0 || x >= iw) return;
		if(y + h <= 0 || y >= ih) return;

		int ax = 0; // bitmap x
		int ay = 0; // bitmap y

		if(x < 0) ax = -x;
		if(y < 0) ay = -y;

		if(x + w >= iw) w = iw - x;
		if(y + h >= ih) h = ih - y;

		w -= ax;
		h -= ay;

		auto dst = image.buffer.data() + iw * (y + ay) + (x + ax);
		auto src = a->bitmap.data() + ay * a->width + ax;
		for(int i = 0; i < h; ++i)
		{
			for(int i = 0; i < w; ++i)
			{
				if(src[i] > dst[i]) dst[i] = src[i];
			}
			dst += iw;
			src += a->width;
		}
	}
	class FontRenderInner : public FontRender
	{
	protected:
		FontRenderLineInfo _line;

		int mOutputWidth;
		int mOutputHeight;

		std::vector<FontRenderLineInfo> mLines;

		std::shared_ptr<Font> mFont;

		FontConfig mConfig;
		std::u32string mText;

		float mAlignment;
		int mLetterSpacing;
		int mMaxLineSize;
		bool mIsAutoBreaking;

	public:
		FontRenderInner(std::shared_ptr<Font> font,
						FontConfig const & config,
						std::u32string const & text,
						float alignment, /* 0.0 left(top), to 1.0 right(bottom) */
						int letter_spacing, // px as font-size
						int max_line_size,
						bool auto_break)
		{
			mFont = font;
			mConfig = config;
			mText = text;
			mAlignment = alignment;
			mLetterSpacing = letter_spacing;
			mMaxLineSize = max_line_size;
			mIsAutoBreaking = auto_break;

			mOutputWidth = mOutputHeight = 0;
			if(max_line_size && auto_break)
			{
				this->_makeLines(mText, letter_spacing, max_line_size);
			}
			else {
				this->_makeLines(mText, letter_spacing);
			}
			if(mConfig.isVerticalWriting)
			{
				mOutputHeight = mOutputWidth + mConfig.charH / 2;
				mOutputWidth = int(mConfig.charW * mLines.size() + mConfig.charW / 2);
			}
			else
			{
				mOutputHeight = int(mConfig.charH * mLines.size() + mConfig.charH / 2);
				mOutputWidth += mConfig.charW / 2;
			}
		}

		virtual std::shared_ptr<FontCharacter> getCharacter(char32_t unicode) override
		{
			return mFont->loadCharacter(unicode, mConfig);
		}
		/*
		 image: dest image, will be modified
		 image_width: final image width, 0 for auto set
		 image_height: final image height, 0 for auto set
		 */
		virtual void renderToImage(ImageData & image,
								   int image_width,
								   int image_height,
								   int x_off, int y_off) override
		{
			if(image_width == 0) image_width = getOutputWidth();
			if(image_height == 0) image_height = getOutputHeight();
			ImageGenerate(image, EImageFormat::Grey, image_width, image_height);

			if(mConfig.isVerticalWriting)
			{
				_writeImageV(image, mConfig.charW, x_off, y_off);
			}
			else
			{
				_writeImageH(image, mConfig.charH, x_off, y_off);
			}
		}

		virtual int getOutputWidth() override
		{
			return mOutputWidth;
		}

		virtual int getOutputHeight() override
		{
			return mOutputHeight;
		}
		virtual size_t getLinesCount() override
		{
			return mLines.size();
		}

	protected:
		void _newLine()
		{
			if(_line.points < 0) _line.points = 0;
			if(_line.points > mOutputWidth) mOutputWidth = _line.points;
			mLines.push_back(_line);
			_line.points = 0;
			_line.chars.clear();
		}
		
		void _makeLines(std::u32string const & text, int letter_spacing)
		{
			for(auto & i : text)
			{
				if(i == '\n')
				{
					_newLine();
					continue;
				}

				auto ch = this->getCharacter(i);
				if(!ch)
				{
					Zen::LogW("errro char %d", (int)i);
					continue;
				}

				auto w = ch->view_width + ch->view_height;
				if(w <= 0)
				{
					Zen::LogW("error char [U%s], w[%d] h[%d]", (int)i, (int)ch->view_width, (int)ch->view_height);
					continue;
				}

				if(_line.chars.size()) _line.points += letter_spacing + w;
				else _line.points = w;
				_line.chars.push_back(ch);
			}
			if(_line.points < 0) _line.points = 0;
			_newLine();
		}
		void _makeLines(std::u32string const & text, int letter_spacing, int max_line_size)
		{
			for(auto & i : text)
			{
				if(i == '\n')
				{
					_newLine();
					continue;
				}
				auto ch = this->getCharacter(i);
				if(!ch)
				{
					Zen::LogW("error char %d", (int)i);
					continue;
				}

				auto w = ch->view_width + ch->view_height;
				if(w <= 0)
				{
					Zen::LogW("error char [U%s], w[%d] h[%d]", (int)i, (int)ch->view_width, (int)ch->view_height);
					continue;
				}
				if(_line.chars.empty())
				{
					_line.points = w;
					_line.chars.push_back(ch);
				}
				else
				{
					auto after = _line.points + letter_spacing + w;
					if(after > max_line_size)
					{
						_newLine();
						_line.points = w;
					}
					else
					{
						_line.points = after;
					}
					_line.chars.push_back(ch);
				}
			}
			_newLine();
		}


		void _writeImageH(ImageData & image, int line_height, int x_off, int y_off)
		{
			int y = line_height + y_off + mConfig.charH / 4;
			x_off += (int)mConfig.charW / 4;

			int w = (int)image.width - mConfig.charW / 2;

			for(auto & line : mLines)
			{
				int x = int(mAlignment * float(w - line.points)) + x_off;

				for(auto i : line.chars)
				{
					WriteChar(image, i.get(), x, y);
					x += i->view_width + mLetterSpacing;
				}
				y += line_height;
			}
		}

		void _writeImageV(ImageData & image, int line_width, int x_off, int y_off)
		{
			int x = mConfig.charW / 4 + x_off;
			int h = (int)image.height - mConfig.charH / 2;
			y_off += (int)mConfig.charH / 4;
			for(auto & line : mLines)
			{
				int y = (int)(mAlignment * (float)(h - line.points)) + y_off;

				// align * (image.height() - charH / 2 - line.points) + y_off + charH / 4

				for(auto i : line.chars)
				{
					y += i->view_height;
					WriteChar(image, i.get(), x, y);
					y += mLetterSpacing;
				}
				x += line_width;
			}
		}
	};

	std::shared_ptr<FontRender> FontRender::Create
	(std::shared_ptr<Font> font,
	  FontConfig const & config,
	  std::u32string const & text,
	  float alignment, /* 0.0 left(top), to 1.0 right(bottom) */
	  int ls, // px as font-size
	  int mls,
	  bool b)
	{
		auto me = new FontRenderInner(font, config, text, alignment, ls, mls, b);
		return std::shared_ptr<FontRender>(me);
	}
}

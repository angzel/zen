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

#include "zen_color.h"
#include "zen_object.h"
#include "zen_exception.h"

#include <memory>
#include <vector>

namespace Zen {

	struct ImageData
	{
		int width = 0;
		int height = 0;
		EBPP format = EBPP::None;
		std::vector<uint8_t> buffer;
	};
	
	inline void ImageGenerate(ImageData & data, EBPP format, int width, int height)
	{
		data.format = format;
		data.width = width;
		data.height = height;
		data.buffer.resize(width * height *  (int)format);
	}
	/**
		copy the alpha channel from @source to @alpha
		only @source format is GA, RGBA valid, else just return false.
	 */
	inline bool ImageSeparateAlpha(ImageData & alpha, ImageData const & source)
	{
		if(source.format == EBPP::GA)
		{
			ImageGenerate(alpha, EBPP::Grey, source.width, source.height);
			auto dest = alpha.buffer.data();
			auto src = source.buffer.data()+1;
			for(int y = 0; y < source.height; ++y)
			{
				for(int x = 0; x < source.width; ++x)
				{
					*dest++ = *src;
					src += 2;
				}
			}
			return true;
		}
		else if(source.format == EBPP::RGBA)
		{
			ImageGenerate(alpha, EBPP::RGB, source.width, source.height);
			auto dest = alpha.buffer.data();
			auto src = source.buffer.data()+3;
			for(int y = 0; y < source.height; ++y)
			{
				for(int x = 0; x < source.width; ++x)
				{
					*dest++ = *src;
					src += 4;
				}
			}
			return true;
		}
		else return false;
	}
	/**
		copy the color channel from @source to @alpha
		copy the alpha channel from @source to @alpha
		only @source format is GA, RGBA valid, else just return false.
		GreayA = Grey + Alpha
		RGBA = RGB + Alpha
	*/
	inline bool ImageSeparateColor(ImageData & color, ImageData const & source)
	{
		if(source.format == EBPP::GA)
		{
			ImageGenerate(color, EBPP::Grey, source.width, source.height);
			auto dest = color.buffer.data();
			auto src = source.buffer.data();
			for(int y = 0; y < source.height; ++y)
			{
				for(int x = 0; x < source.width; ++x)
				{
					*dest++ = *src;
					src += 2;
				}
			}
			return true;
		}
		else if(source.format == EBPP::RGBA)
		{
			ImageGenerate(color, EBPP::RGB, source.width, source.height);
			auto dest = color.buffer.data();
			auto src = source.buffer.data();
			for(int y = 0; y < source.height; ++y)
			{
				for(int x = 0; x < source.width; ++x)
				{
					dest[0] = src[0];
					dest[1] = src[1];
					dest[2] = src[2];
					src += 4;
					dest += 3;
				}
			}
			return true;
		}
		else return false;
	}

	class ImageDecoder : public virtual Zen::Object
	{
	public:
		virtual void decode(ImageData & img, std::vector<uint8_t> const & data) = 0;
	};
	class ImageEncoder : public virtual Zen::Object
	{
	public:
		virtual std::vector<uint8_t> encode(ImageData const & img) = 0;
	};

}

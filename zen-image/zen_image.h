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

	enum class EImageFormat
	{
		None,
		Grey,	// grey 1 byte
		GreyA,	// grey+alpha 2 bytes
		RGB,	// 3 bytes
		RGBA,	// 4 bytes
	};

	struct ImageData
	{
		int width = 0;
		int height = 0;
		EImageFormat format = EImageFormat::None;
		std::vector<uint8_t> buffer;
	};

	inline int GetBytesOfImageFormat(EImageFormat format)
	{
		switch (format) {
			case EImageFormat::Grey: return 1;
			case EImageFormat::GreyA:  return 2;
			case EImageFormat::RGB:  return 3;
			case EImageFormat::RGBA: return 4;
			default: return 0;
		}
	}
	
	inline void ImageGenerate(ImageData & data, EImageFormat format, int width, int height)
	{
		data.format = format;
		data.width = width;
		data.height = height;
		data.buffer.resize(width * height *  GetBytesOfImageFormat(format));
	}
	/**
		copy the alpha channel from @source to @alpha
		only @source format is GreyA, RGBA valid, else just return false.
	 */
	inline bool ImageSeparateAlpha(ImageData & alpha, ImageData const & source)
	{
		if(source.format == EImageFormat::GreyA)
		{
			ImageGenerate(alpha, EImageFormat::Grey, source.width, source.height);
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
		else if(source.format == EImageFormat::RGBA)
		{
			ImageGenerate(alpha, EImageFormat::RGB, source.width, source.height);
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
		only @source format is GreyA, RGBA valid, else just return false.
		GreayA = Grey + Alpha
		RGBA = RGB + Alpha
	*/
	inline bool ImageSeparateColor(ImageData & color, ImageData const & source)
	{
		if(source.format == EImageFormat::GreyA)
		{
			ImageGenerate(color, EImageFormat::Grey, source.width, source.height);
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
		else if(source.format == EImageFormat::RGBA)
		{
			ImageGenerate(color, EImageFormat::RGB, source.width, source.height);
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

	struct ImageCoder : public virtual Zen::Object
	{
		virtual void load(ImageData & img, std::string const & file) = 0;
		virtual void save(ImageData const & img, std::string const & file) = 0;
		virtual void decode(ImageData & img, std::vector<uint8_t> const & data) = 0;
		virtual std::vector<uint8_t> encode(ImageData const & img) = 0;
	};

}

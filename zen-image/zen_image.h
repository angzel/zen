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

#include <memory>
#include <vector>

namespace Zen {

	class Image
	{
	public:
		inline static std::shared_ptr<Image> Create(ePixel format, size_t width, size_t height);
		inline static std::shared_ptr<Image> CreateWidthData(ePixel format, size_t width, size_t height, void const * data);
		inline static std::shared_ptr<Image> CreateWidthByte(ePixel format, size_t width, size_t height, uint8_t byte);

		uint8_t * bytes() { return m_buffer.data(); }
		size_t size() const { return m_buffer.size(); }
		uint8_t const * bytes() const { return m_buffer.data(); }
		ePixel format() const { return m_format; }
		size_t width() const { return m_width; }
		size_t height() const { return m_height; }

		void fillByte(uint8_t byte);
		void fillAlpha(uint8_t byte);
		void fillGrey(uint8_t byte);
		void fillRGB(Zen::Color3b rgb);
		void fillRGBA(Zen::Color4b rgba);

		/**
		 @ data must has same width/height.

		 copy color and alpha.
		 if any self pixel changed, return true.
		 */
		bool copy(ePixel format, void const * data);
		/**
		 copy alpha only.
		 if no alpha channel, return false.
		 */
		bool copyAlpha(ePixel format, void const * data);
		/**
		 copy rgb(or grey). only.
		 */
		bool copyColor(ePixel format, void const * data);
	protected:
		Image() = default;
		Image(Image&) = default;

		std::vector<uint8_t> m_buffer;
		ePixel m_format = ePixel::None;
		size_t m_width = 0;
		size_t m_height = 0;
	};

	class ImageDecoder
	{
	public:
		virtual std::shared_ptr<Image> decode(std::vector<uint8_t> const & data) = 0;
		virtual ~ImageDecoder() = default;
	};
	
	class ImageEncoder
	{
	public:
		virtual std::vector<uint8_t> encode(Image const *) = 0;
		virtual ~ImageEncoder() = default;
	};

}

namespace Zen {

	inline std::shared_ptr<Image> Image::CreateWidthData(ePixel format, size_t width, size_t height, void const * data)
	{
		if(data == nullptr)
			return nullptr;

		auto a = std::shared_ptr<Image>(new Image);

		a->m_width = width;
		a->m_height = height;
		a->m_format = format;
		auto size = width * height *  (int)format;
		auto buf = (uint8_t const*)data;
		a->m_buffer.assign(buf, buf + size);

		return a;
	}
	inline std::shared_ptr<Image> Image::CreateWidthByte(ePixel format, size_t width, size_t height, uint8_t byte)
	{
		auto a = std::shared_ptr<Image>(new Image);

		a->m_width = width;
		a->m_height = height;
		a->m_format = format;
		auto size = width * height *  (int)format;
		a->m_buffer.assign(size, byte);

		return a;
	}
	inline std::shared_ptr<Image> Image::Create(ePixel format, size_t width, size_t height)
	{
		return CreateWidthByte(format, width, height, 0);
	}
}

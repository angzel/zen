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

#include "zen_image.h"

namespace Zen {

	void Image::fillByte(uint8_t byte)
	{
	}


	void Image::fillAlpha(uint8_t byte)
	{
		if(m_format == ePixel::Grey)
		{
			::memset(bytes(), byte, size());
		}
		else if(m_format == ePixel::GA)
		{
			auto s = bytes() + 1;
			auto e = s + size();
			for(; s < e; s += 2) *s = byte;
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = bytes() + 3;
			auto e = s + size();
			for(; s < e; s += 4) *s = byte;
		}
	}


	void Image::fillGrey(uint8_t byte)
	{
		if(m_format == ePixel::Grey || m_format == ePixel::RGB)
		{
			::memset(bytes(), byte, size());
		}
		else if(m_format == ePixel::GA)
		{
			auto s = bytes();
			auto const e = s + size();
			for(; s < e; s += 2) *s = byte;
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = bytes();
			auto const e = s + size();
			for(; s < e; s += 4) s[0] = s[1] = s[2] = byte;
		}
	}


	void Image::fillRGB(Zen::Color3b rgb)
	{
		if(m_format == ePixel::Grey || m_format == ePixel::GA)
		{
			this->fillGrey(rgb.g_byte);
		}
		else if(m_format == ePixel::RGB || m_format == ePixel::RGBA)
		{
			auto s = bytes();
			auto const e = s + size();
			for(; s < e; s += (int)m_format)
			{
				s[0] = rgb.r_byte;
				s[1] = rgb.g_byte;
				s[2] = rgb.b_byte;
			}
		}
	}


	void Image::fillRGBA(Zen::Color4b rgba)
	{
		if(m_format == ePixel::Grey)
		{
			fillAlpha(rgba.a_byte);
		}
		else if(m_format == ePixel::RGB)
		{
			fillRGB(Zen::Color3b(rgba));
		}
		else if(m_format == ePixel::GA)
		{
			auto s = bytes();
			auto const e = s + size();
			for(; s < e; s += 2)
			{
				s[0] = rgba.g_byte;
				s[1] = rgba.a_byte;
			}
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = bytes();
			auto const e = s + size();
			for(; s < e; s += 4)
			{
				s[0] = rgba.r_byte;
				s[1] = rgba.g_byte;
				s[2] = rgba.b_byte;
				s[3] = rgba.a_byte;
			}
		}
	}

	
	bool Image::copy(ePixel format, void const * data)
	{
		if(m_format == ePixel::None || format == ePixel::None)
			return false;

		if(format == m_format)
		{
			::memcpy(bytes(), data, size());
			return true;
		}
		if(m_format == ePixel::RGB || format == ePixel::RGB)
		{
			return copyColor(format, data);
		}

		if(m_format == ePixel::Grey)
		{
			if(format == ePixel::RGBA)
				return copyAlpha(format, data);
			if(format == ePixel::GA)
				return copyColor(format, data);
		}
		if(m_format == ePixel::GA)
		{
			if(format == ePixel::RGBA)
			{
				auto dst = this->bytes();
				auto src = (uint8_t const*)data;
				auto count = width() * height();
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = src[1];
					dst[1] = src[3];
					dst += 2;
					src += 4;
				}
				return true;
			}
			if(format == ePixel::Grey)
				return copyColor(format, data);
		}
		if(m_format == ePixel::RGBA)
		{
			if(format == ePixel::GA)
			{
				auto dst = this->bytes();
				auto src = (uint8_t const*)data;
				auto count = width() * height();
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = dst[1] = dst[2] = src[0];
					dst[3] = src[1];
					dst += 4;
					src += 2;
				}
				return true;
			}
			if(format == ePixel::Grey)
				return copyAlpha(format, data);
		}
		return false;
	}

	bool Image::copyAlpha(ePixel format, void const * data)
	{
		if(m_format == ePixel::None || format == ePixel::None)
			return false;

		if(format == ePixel::RGB || m_format == ePixel::RGB) return false;

		if(format == m_format && format == ePixel::Grey)
		{
			::memcpy(bytes(), data, size());
			return true;
		}
		auto dst = bytes() + ((int)m_format - 1);
		auto src = (uint8_t const *)data + ((int)format - 1);
		auto count = width() * height();
		for(size_t i = 0; i < count; ++i)
		{
			*dst = *src;
			dst += (int)m_format;
			src += (int)format;
		}
		return true;
	}
	bool Image::copyColor(ePixel format, void const * data)
	{
		if(m_format == ePixel::None || format == ePixel::None)
			return false;

		if(format == m_format)
		{
			if(format == ePixel::RGB || format == ePixel::Grey)
			{
				::memcpy(bytes(), data, size());
				return true;
			}
		}

		int a = (int)m_format;
		int b = (int)format;
		auto dst = bytes();
		auto src = (uint8_t const *)data;
		auto count = width() * height();

		if(a > 2) // rgb, rgba
		{
			if(b > 2) // rgb, rgba
			{
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = src[0];
					dst[1] = src[1];
					dst[2] = src[2];
					dst += a;
					src += b;
				}
				return true;
			}
			else // g, ga
			{
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = dst[1] = dst[2] = src[0];
					dst += a;
					src += b;
				}
				return true;
			}
		}
		else
		{
			if(b > 2) // rgb, rgba
			{
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = src[1];
					dst += a;
					src += b;
				}
				return true;
			}
			else // g, ga
			{
				for(size_t i = 0; i < count; ++i)
				{
					dst[0] = src[0];
					dst += a;
					src += b;
				}
				return true;
			}
		}
		return true;
	}
}

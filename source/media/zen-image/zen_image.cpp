/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_image.h"

namespace Zen {

	std::shared_ptr<Image> Image::CreateWidthData(ePixel format, size_t width, size_t height, void const * data)
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
	std::shared_ptr<Image> Image::CreateWidthByte(ePixel format, size_t width, size_t height, uint8_t byte)
	{
		auto a = std::shared_ptr<Image>(new Image);

		a->m_width = width;
		a->m_height = height;
		a->m_format = format;
		auto size = width * height *  (int)format;
		a->m_buffer.assign(size, byte);

		return a;
	}
	std::shared_ptr<Image> Image::Create(ePixel format, size_t width, size_t height)
	{
		return CreateWidthByte(format, width, height, 0);
	}


	void Image::fillByte(uint8_t byte)
	{
	}


	void Image::fillAlpha(uint8_t byte)
	{
		if(m_format == ePixel::Grey)
		{
			::memset(data(), byte, size());
		}
		else if(m_format == ePixel::GA)
		{
			auto s = data() + 1;
			auto e = s + size();
			for(; s < e; s += 2) *s = byte;
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = data() + 3;
			auto e = s + size();
			for(; s < e; s += 4) *s = byte;
		}
	}


	void Image::fillGrey(uint8_t byte)
	{
		if(m_format == ePixel::Grey || m_format == ePixel::RGB)
		{
			::memset(data(), byte, size());
		}
		else if(m_format == ePixel::GA)
		{
			auto s = data();
			auto const e = s + size();
			for(; s < e; s += 2) *s = byte;
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = data();
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
			auto s = data();
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
			auto s = data();
			auto const e = s + size();
			for(; s < e; s += 2)
			{
				s[0] = rgba.g_byte;
				s[1] = rgba.a_byte;
			}
		}
		else if(m_format == ePixel::RGBA)
		{
			auto s = data();
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
			::memcpy(this->data(), data, size());
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
				auto dst = this->data();
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
				auto dst = this->data();
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
			::memcpy(this->data(), data, size());
			return true;
		}
		auto dst = this->data() + ((int)m_format - 1);
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
				::memcpy(this->data(), data, size());
				return true;
			}
		}

		int a = (int)m_format;
		int b = (int)format;
		auto dst = this->data();
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

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_color.h"

#include <memory>
#include <vector>

namespace Zen {

	class Image
	{
	public:
		static std::shared_ptr<Image> Create(ePixel format, size_t width, size_t height);
		static std::shared_ptr<Image> CreateWidthData(ePixel format, size_t width, size_t height, void const * data);
		/*
		 use {byte} to fill data area.
		 **/
		static std::shared_ptr<Image> CreateWidthByte(ePixel format, size_t width, size_t height, uint8_t byte);

		uint8_t * data() { return m_buffer.data(); }
		size_t size() const { return m_buffer.size(); }
		uint8_t const * data() const { return m_buffer.data(); }
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
		virtual std::vector<uint8_t> encode(Image const &) = 0;
		virtual ~ImageEncoder() = default;
	};

}

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_image.h"

/**
	This(raw) is a simple bitmap image format.
 
 definition: header(fixed 16Bytes) + data(pixel bytes in order).
	header = sign(4B, 'jaii') + width(4B) + height(4B) + format(2B, {ePixel}) + kept(2B unused).
 */
namespace Zen
{
	class ImageRawDecoder : public ImageDecoder
	{
	public:
		virtual std::shared_ptr<Image> decode(std::vector<uint8_t> const & data) override;
	};
	
	class ImageRawEncoder : public ImageEncoder
	{
	public:
		virtual std::vector<uint8_t> encode(Image const &) override;
	};

	class ImageRawCoder : public ImageRawDecoder, public ImageRawEncoder
	{
	};
}


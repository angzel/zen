/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */


#pragma once

#include "zen_image.h"

namespace Zen
{
	class ImagePNGDecoder : public ImageDecoder
	{
	public:
		virtual std::shared_ptr<Image> decode(std::vector<uint8_t> const & data) override;
	};
	class ImagePNGEncoder : public ImageEncoder
	{
	public:
		virtual std::vector<uint8_t> encode(Image const &) override;
	};
	class ImagePNGCoder : public ImagePNGDecoder, public ImagePNGEncoder
	{
	};
}



/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_image.h"

namespace Zen
{
	/**
	@class ImageCoderJPG
	when encode the alpha channel will be thrown away.
	 */
	class ImageJPGEncoder : public ImageEncoder
	{
	public:
		virtual std::vector<uint8_t> encode(Image const &) override;

		void setQuality(int); /* 0 - 100 */
		int getQuality() const;
	protected:
		int m_quality = 100;
	};
	class ImageJPGDecoder : public ImageDecoder
	{
	public:
		virtual std::shared_ptr<Image> decode(std::vector<uint8_t> const & data) override;
	};
	class ImageJPGCoder : public ImageJPGEncoder, public ImageJPGDecoder
	{
	};
}


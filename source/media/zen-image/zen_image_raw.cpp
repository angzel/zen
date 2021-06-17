/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_image_raw.h"
#include "zen_file.h"
#include "zen_endian.h"
#include "zen_exception.h"

namespace Zen
{
	struct ImageCoderRawHead
	{
#define Version Byte4('j','a','i','i').value
		
		uint32_t sign = 0;
		uint32_t width = 0;
		uint32_t height = 0;
		uint16_t format = 0;
		uint16_t keep_ = 0;
		
		void set(uint32_t width_, uint32_t height_,  uint32_t fmt)
		{
			sign = Version;
			width = HostNet32((uint32_t)width_);
			height = HostNet32((uint32_t)height_);
			format = HostNet16((uint16_t)fmt);
			keep_ = 0;
		}
	};
	
	std::shared_ptr<Image> ImageRawDecoder::decode(std::vector<uint8_t> const & data)
	{
		ImageCoderRawHead head;
		musts(data.size() >= sizeof(head), "invalid image raw");

		::memcpy(&head, data.data(), sizeof(head));

		uint32_t width = HostNet32(head.width);
		uint32_t height = HostNet32(head.height);
		
		auto format = (Zen::ePixel)HostNet16(head.format);
		
		uint32_t bpp = (int)format;
		musts(bpp > 0, "invalid format");
		
		uint32_t sz = width * height * bpp;
		
		musts(data.size() >= sz + sizeof(head), "too few data length");

		auto image = Image::Create(format, width, height);
		must(image->size() == sz);

		::memcpy(image->data(), data.data() + sizeof(head), sz);
		return image;
	}
	std::vector<uint8_t> ImageRawEncoder::encode(Image const & image)
	{
		ImageCoderRawHead head;

		head.set((uint32_t)image.width(), (uint32_t)image.height(), (uint32_t)image.format());
		
		auto head_buf = reinterpret_cast<char const *>(&head);
		
		std::vector<uint8_t> data;
		data.reserve(sizeof(head) + image.size());
		data.assign(head_buf, head_buf + sizeof(head));
		data.insert(data.end(), image.data(), image.data() + image.size());
		
		return data;
	}
}


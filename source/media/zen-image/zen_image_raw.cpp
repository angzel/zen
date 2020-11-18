/*
 Copyright (c) 2013 MeherTJ G.
 
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

#include "zen_image_raw.h"
#include "zen_file.h"
#include "zen_endian.h"
#include "zen_exception.h"

namespace Zen
{
	struct ImageCoderRawHead
	{
#define Version Byte4('j','a','i','i').value
		
		// donnt change the order of these members.
		uint32_t sign;
		uint32_t width;
		uint32_t height;
		uint16_t format;
		uint16_t keep_;
		
		ImageCoderRawHead()
		{
		}
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
//		Zen::BufferReader reader(&data);
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


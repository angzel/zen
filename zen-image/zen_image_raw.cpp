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

#include "zen_image_raw.h"
#include "zen_file.h"
#include "zen_endian.h"
#include "zen_buffer.h"
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
	
	
	void ImageCoderRaw::load(ImageData & img, std::string const & file)
	{
		img.format = Zen::EImageFormat::None;

		auto data = Zen::LoadFileToBuffer(file);
		musts(data.size(), "read file error");

		this->decode(img, data);
	}
	void ImageCoderRaw::save(ImageData const & img, std::string const & file)
	{
		std::fstream outs;
		outs.open(file, std::ios::out | std::ios::binary);
		musts(outs.good(), "open file error");
		
		ImageCoderRawHead head;
		head.set(img.width, img.height, (uint32_t)img.format);
		auto head_buf = reinterpret_cast<char const *>(&head);

		outs.write((const char*)head_buf, sizeof(head));
		outs.write((const char*)img.buffer.data(), img.buffer.size());
		
		musts(outs.good(), "write file error");
	}
	
	void ImageCoderRaw::decode(ImageData & img, std::vector<uint8_t> const & data)
	{
		img.format = Zen::EImageFormat::None;
		
		Zen::BufferReader reader(&data);
		
		ImageCoderRawHead head;
		
		bool res = reader.read(head);
		
		musts(res, "invalid image raw head");

		uint32_t width = HostNet32(head.width);
		uint32_t height = HostNet32(head.height);
		
		auto format = (Zen::EImageFormat)HostNet16(head.format);
		
		uint32_t bpp = GetBytesOfImageFormat(format);
		musts(bpp > 0, "invalid format");
		
		uint32_t sz = width * height * bpp;
		
		musts(reader.getReadPointer() + sz <= data.size(), "too few data length");
		
		img.buffer.resize(sz);
		
		reader.read(img.buffer.data(), img.buffer.size());

		img.width = width;
		img.height = height;
		img.format = format;
	}
	std::vector<uint8_t> ImageCoderRaw::encode(ImageData const & img)
	{
		ImageCoderRawHead head;
		head.set(img.width, img.height, (uint32_t)img.format);
		
		auto head_buf = reinterpret_cast<char const *>(&head);
		
		std::vector<uint8_t> data;
		data.reserve(sizeof(head) + img.buffer.size());
		data.assign(head_buf, head_buf + sizeof(head));
		data.insert(data.end(), img.buffer.begin(), img.buffer.end());
		
		return data;
	}
}


/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

/**
	 Need png library
 */
#include "PNG/png.h"
#include "PNG/pngconf.h"

#include "zen_image_png.h"
#include "zen_file.h"
#include "zen_endian.h"
#include "zen_exception.h"
#include "zen_log.h"
#include <iostream>
using namespace std;

namespace Zen
{
	namespace MyPNG {
		
		static void OnWarning(png_structp png_ptr, png_const_charp message)
		{
			Zen::LogW("libpng warning:%s", message);
		}
		
		static void OnError(png_structp png_ptr, png_const_charp message)
		{
			throws("libpng operation error");
		}
		
		struct ReadSource
		{
			std::vector<uint8_t> const * src;
			int offset;
		};
		struct WriteSource
		{
			std::vector<uint8_t> * dst;
		};
		struct ReadLib
		{
			png_structp png_ptr;
			png_infop info_ptr;
			
			ReadLib()
			{
				png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
				info_ptr = png_create_info_struct(png_ptr);
				
				musts(png_ptr && info_ptr, "failed to load png lib");
				
				png_set_error_fn(png_ptr, nullptr, &MyPNG::OnError, &MyPNG::OnWarning);
			}
			~ReadLib()
			{
				if (png_ptr)
				{
					png_destroy_read_struct(&png_ptr, (info_ptr ? &info_ptr : nullptr), 0);
				}
			}
		};
		struct WriteLib
		{
			png_structp png_ptr;
			png_infop info_ptr;
			
			WriteLib()
			{
				png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
				info_ptr = png_create_info_struct(png_ptr);
				
				musts(png_ptr && info_ptr, "failed to load png lib");
				
				png_set_error_fn(png_ptr, nullptr, &MyPNG::OnError, &MyPNG::OnWarning);
			}
			~WriteLib()
			{
				if (png_ptr)
				{
					png_destroy_write_struct(&png_ptr, (info_ptr ? &info_ptr : nullptr));
				}
			}
		};
		
		static void ReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
		{
			MyPNG::ReadSource * p = (MyPNG::ReadSource*)png_get_io_ptr(png_ptr);
			
			if (p->offset + length <= p->src->size())
			{
				memcpy(data, p->src->data() + p->offset, length);
				p->offset += length;
			}
			else
			{
				png_error(png_ptr, "pngReaderCallback failed");
			}
		}
		static void WriteCallback(png_structp png_ptr, png_bytep data, png_size_t length)
		{
			MyPNG::WriteSource * p = (MyPNG::WriteSource*)png_get_io_ptr(png_ptr);
			
			auto src = (const char *)data;
			p->dst->insert(p->dst->end(), src, src + length);
		}
	}
}

namespace Zen
{
//	void ImagePNGCoder::decode(ImageData & img, std::vector<uint8_t> const & data)
	std::shared_ptr<Image> ImagePNGDecoder::decode(std::vector<uint8_t> const & data)
	{
		static int const HeadLen = 8;
		
		musts(data.size() >= HeadLen, "invalid png data");

		// check the data is png or not
		musts(png_sig_cmp((png_const_bytep)data.data(), 0, HeadLen) == 0, "not png file");
		
		MyPNG::ReadLib png;
		
		MyPNG::ReadSource imageSource;
		imageSource.offset = 0;
		imageSource.src = &data;
		
		::png_set_read_fn(png.png_ptr, &imageSource, &MyPNG::ReadCallback);
		
		::png_read_info(png.png_ptr, png.info_ptr);

		uint32_t width = ::png_get_image_width(png.png_ptr, png.info_ptr);
		uint32_t height = ::png_get_image_height(png.png_ptr, png.info_ptr);
		int bits = ::png_get_bit_depth(png.png_ptr, png.info_ptr);
		png_uint_32 color_type = ::png_get_color_type(png.png_ptr, png.info_ptr);
		
		// force palette images to be expanded to 24-bit RGB
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			::png_set_palette_to_rgb(png.png_ptr);
		}
		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (color_type == PNG_COLOR_TYPE_GRAY && bits < 8)
		{
			::png_set_expand_gray_1_2_4_to_8(png.png_ptr);
		}
		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(png.png_ptr, png.info_ptr, PNG_INFO_tRNS))
		{
			::png_set_tRNS_to_alpha(png.png_ptr);
		}
		// reduce images with 16-bit samples to 8 bits
		if (bits == 16)
		{
			::png_set_strip_16(png.png_ptr);
		}

		// expand ga images to RGB
//		if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
//		{
//			::png_set_gray_to_rgb(png.png_ptr);
//		}
		
		std::vector<png_bytep> row_pointers(height, 0);
		
		::png_read_update_info(png.png_ptr, png.info_ptr);
		
		size_t rowbytes = png_get_rowbytes(png.png_ptr, png.info_ptr);
		
		size_t channel = rowbytes / width;
		
		musts(rowbytes % width == 0, "unsupported row stride");

		ePixel format = (ePixel)channel;
		musts(channel > 0 && channel <= 4, "unsupported pixel format");
		
		std::vector<uint8_t> buffer;
		buffer.resize(rowbytes * height);

		for (unsigned short i = 0; i < height; ++i)
		{
			row_pointers[i] = (unsigned char *)buffer.data() + i * rowbytes;
		}
		
		::png_read_image(png.png_ptr, row_pointers.data());
		
		::png_read_end(png.png_ptr, nullptr);

		return Image::CreateWidthData(format, width, height, buffer.data());
	}
	std::vector<uint8_t> ImagePNGEncoder::encode(Image const & image)
	{
		size_t channel = (size_t)image.format();
		musts(channel > 0 && channel <= 4, "unsupported pixel format");
		
		size_t rowbytes = channel * image.width();
		
		std::vector<png_bytep> row_pointers(image.height());
		for (size_t i = 0; i < image.height(); i++)
		{
			row_pointers[i] = (png_bytep)(image.data() + rowbytes * i);
		}
		
		int color_type = 0;
		switch (image.format())
		{
			case ePixel::Grey:
				color_type = PNG_COLOR_TYPE_GRAY;
				break;
			case ePixel::GA:
				color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
				break;
			case ePixel::RGB:
				color_type = PNG_COLOR_TYPE_RGB;
				break;
			case ePixel::RGBA:
				color_type = PNG_COLOR_TYPE_RGBA;
				break;
			default:
				throws("invalid format");
		}
		
		MyPNG::WriteLib png;
		
		MyPNG::WriteSource source;
		std::vector<uint8_t> data;
		source.dst = &data;
		
		::png_set_write_fn(png.png_ptr, &source, &MyPNG::WriteCallback, nullptr);

		::png_set_IHDR(png.png_ptr, png.info_ptr, (png_uint_32)image.width(), (png_uint_32)image.height(), 8, (int)color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		::png_write_info(png.png_ptr, png.info_ptr);
		
		::png_write_image(png.png_ptr, row_pointers.data());
		::png_write_end(png.png_ptr, nullptr);
		
		return data;
	}
}



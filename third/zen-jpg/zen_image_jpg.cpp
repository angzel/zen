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

#include "zen_image_jpg.h"
#include "zen_file.h"
#include "zen_endian.h"
#include "zen_buffer.h"
#include "zen_exception.h"

/**
 Need jpeg library.
 */
#include "JPEG/jconfig.h"
#include "JPEG/jpeglib.h"


namespace Zen { namespace MyJPG {

		struct ReadInfo
		{
			jpeg_decompress_struct cinfo;
			jpeg_error_mgr error_mgr;

			static void OnError(j_common_ptr cinfo)
			{
				throws("libjpeg decompress error");
			}
			ReadInfo()
			{
				jpeg_create_decompress(&cinfo);

				cinfo.err = jpeg_std_error(&error_mgr);

				error_mgr.error_exit = &ReadInfo::OnError;
			}
			~ReadInfo()
			{
				jpeg_destroy_decompress(&cinfo);
			}
		};

		struct WriteInfo
		{
			jpeg_compress_struct cinfo;
			jpeg_error_mgr error_mgr;

			static void OnError(j_common_ptr cinfo)
			{
				throws("libjpeg compress error");
			}
			WriteInfo()
			{
				jpeg_create_compress(&cinfo);

				cinfo.err = jpeg_std_error(&error_mgr);

				error_mgr.error_exit = &WriteInfo::OnError;
			}
			~WriteInfo()
			{
				jpeg_destroy_compress(&cinfo);
			}
		};
	}
}

namespace Zen
{
	void ImageCoderJPG::load(ImageData & img, std::string const & file)
	{
		img.format = Zen::EImageFormat::None;

		auto data = Zen::LoadFileToBuffer(file);
		musts(data.size(), "read file error");

		this->decode(img, data);
	}
	void ImageCoderJPG::save(ImageData const & img, std::string const & file)
	{
		auto data = encode(img);
		musts(Zen::WriteBufferToFile(file, data), "write file error");
	}

	void ImageCoderJPG::decode(ImageData & img, std::vector<uint8_t> const & data)
	{
		img.format = Zen::EImageFormat::None;

		MyJPG::ReadInfo jpg;

		::jpeg_mem_src(&jpg.cinfo, (unsigned char *)data.data(), data.size());

		::jpeg_read_header(&jpg.cinfo, (boolean)1);

		musts(::jpeg_start_decompress(&jpg.cinfo), "failed to decompress jpeg");

		auto bpp = jpg.cinfo.output_components;

		Zen::EImageFormat format = Zen::EImageFormat::None;

		if(bpp == 1) format = Zen::EImageFormat::Grey;
		else if(bpp == 3) format = Zen::EImageFormat::RGB;
		else if(bpp == 4) format = Zen::EImageFormat::RGBA;
		else throws("unsuported pixel format");

		auto width = (int)(jpg.cinfo.output_width);
		auto height = (int)(jpg.cinfo.output_height);
		auto rowbytes = (int)jpg.cinfo.output_width * (int)bpp;

		std::vector<uint8_t> buffer;
		buffer.resize(rowbytes * height);

		auto & ri = jpg.cinfo.output_scanline;
		while (ri < height)
		{
			JSAMPROW row_pointers = (JSAMPROW)(buffer.data() + ri * rowbytes);
			::jpeg_read_scanlines(&jpg.cinfo, &row_pointers, 1);
		}

		::jpeg_finish_decompress(&jpg.cinfo);

		img.width = width;
		img.height = height;
		img.format = format;
		img.buffer = std::move(buffer);
	}
	std::vector<uint8_t> ImageCoderJPG::encode(ImageData const & img_o)
	{
		ImageData colors;
		ImageData const & img = Zen::ImageSeparateColor(colors, img_o)?colors:img_o;

		auto bpp = GetBytesOfImageFormat(img.format);

		J_COLOR_SPACE color_space = J_COLOR_SPACE::JCS_UNKNOWN;
		if(bpp == 1) color_space = JCS_GRAYSCALE;
		else if(bpp == 3) color_space = JCS_RGB;
		else throws("not supported jpeg color format");

		MyJPG::WriteInfo jpg;

		unsigned char * buffer = nullptr;
		unsigned long size = 0;

		jpeg_mem_dest(&jpg.cinfo, &buffer, &size);

		jpg.cinfo.image_width = img.width;
		jpg.cinfo.image_height = img.height;
		jpg.cinfo.input_components = bpp;
		jpg.cinfo.in_color_space = color_space;

		::jpeg_set_defaults(&jpg.cinfo);
		::jpeg_set_quality(&jpg.cinfo, m_quality, (boolean)1);
		::jpeg_start_compress(&jpg.cinfo, (boolean)1);

		auto rowbytes = (unsigned)bpp * img.width;

		auto & ri = jpg.cinfo.next_scanline;
		while (ri < img.height)
		{
			JSAMPROW row_pointers = (JSAMPROW)(img.buffer.data() + ri * rowbytes);
			::jpeg_write_scanlines(&jpg.cinfo, &row_pointers, 1);
		}

		::jpeg_finish_compress(&jpg.cinfo);

		musts(buffer && size, "empty jpg data");

		std::vector<uint8_t> data;
		data.assign(buffer, buffer + size);

		return data;
	}

	void ImageCoderJPG::setQuality(int q)
	{
		m_quality = q;
	}

	int ImageCoderJPG::getQuality() const
	{
		return m_quality;
	}
}



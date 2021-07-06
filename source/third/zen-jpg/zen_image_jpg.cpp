/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_image_jpg.h"
#include "zen_file.h"
#include "zen_endian.h"
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
	std::shared_ptr<Image> ImageJPGDecoder::decode(std::vector<uint8_t> const & data)
	{
		MyJPG::ReadInfo jpg;

		::jpeg_mem_src(&jpg.cinfo, (unsigned char *)data.data(), data.size());

		::jpeg_read_header(&jpg.cinfo, (boolean)1);

		musts(::jpeg_start_decompress(&jpg.cinfo), "failed to decompress jpeg");

		auto bpp = jpg.cinfo.output_components;

		musts(bpp == 1 || bpp == 3 || bpp == 4, "unsuported pixel format");

		ePixel format = (ePixel)bpp;

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

		return Image::CreateWidthData(format, width, height, buffer.data());
	}
	std::vector<uint8_t> ImageJPGEncoder::encode(Image const & image)
	{
		auto bpp = (int)image.format();

		J_COLOR_SPACE color_space = J_COLOR_SPACE::JCS_UNKNOWN;
		if(bpp == 1) color_space = JCS_GRAYSCALE;
		else if(bpp == 3) color_space = JCS_RGB;
		else throws("not supported jpeg color format");

		MyJPG::WriteInfo jpg;

		unsigned char * buffer = nullptr;
		unsigned long size = 0;

		jpeg_mem_dest(&jpg.cinfo, &buffer, &size);

		jpg.cinfo.image_width = (JDIMENSION)image.width();
		jpg.cinfo.image_height = (JDIMENSION)image.height();
		jpg.cinfo.input_components = bpp;
		jpg.cinfo.in_color_space = color_space;

		::jpeg_set_defaults(&jpg.cinfo);
		::jpeg_set_quality(&jpg.cinfo, m_quality, (boolean)1);
		::jpeg_start_compress(&jpg.cinfo, (boolean)1);

		auto rowbytes = (unsigned)bpp * image.width();

		auto & ri = jpg.cinfo.next_scanline;
		while (ri < image.height())
		{
			JSAMPROW row_pointers = (JSAMPROW)(image.data() + ri * rowbytes);
			::jpeg_write_scanlines(&jpg.cinfo, &row_pointers, 1);
		}

		::jpeg_finish_compress(&jpg.cinfo);

		musts(buffer && size, "empty jpg data");

		std::vector<uint8_t> data;
		data.assign(buffer, buffer + size);

		return data;
	}

	void ImageJPGEncoder::setQuality(int q)
	{
		m_quality = q;
	}

	int ImageJPGEncoder::getQuality() const
	{
		return m_quality;
	}
}



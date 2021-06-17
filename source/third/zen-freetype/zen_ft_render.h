/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_ft_library.h"
#include "zen_image.h"
#include <string>

namespace Zen {

	class FontRender
	{
	public:
		static std::shared_ptr<FontRender> Create
		(std::shared_ptr<FontBrush>,
		 std::u32string const & text,
		 float alignment, /* 0.0 left(top), to 1.0 right(bottom) */
		 int char_spacing, // px as font-size
		 int line_spacing,
		 int line_width);

		/*
		 image: dest image, will be modified
		 image_width: final image width, 0 for auto set
		 image_height: final image height, 0 for auto set
		 underline: line width in base line
		 */
		virtual std::shared_ptr<Image> render
		(size_t width, size_t height,
		 int x_off, int y_off, int underline) = 0;

		virtual int getOutputWidth() = 0;

		virtual int getOutputHeight() = 0;

		virtual size_t getLinesCount() = 0;
	};
}

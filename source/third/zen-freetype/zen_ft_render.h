/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_ft_library.h"
#include "zen_image.h"
#include <string>

namespace Zen {

	class FontRender
	{
	public:
		struct Style {
			float alignment = 0.f; // [0., 1.] 0:left, 1:right
			int word_space = 0; // [-,+] px as font-size
			int line_space = 0; // [-,+] px
			int width_limit = 0; // [0, +] 0:nolimit, >0: px
		};
		struct RenderSetting {
			int x_off = 0; /// [-,+] move text l->r
			int y_off = 0; /// [-,+] move text top->bot
			/**
			 @var underline
			 @note range: [-,+] in px,
			 if underline <0,the UNDERLINE will be ABOVE base line.
			 = 0, no line.
			 = 1, on base line.
			 > 1, on base line and BELOW it.
			 */
			int underline = 0;
			uint8_t fill = 0; /// bg_fill.
		};
	public:
		static std::shared_ptr<FontRender> Create
		(std::shared_ptr<FontBrush>, std::u32string const & text, Style const & style);

		/**
		 */
		virtual std::shared_ptr<Image> render
		(size_t width, size_t height, RenderSetting const & set) = 0;

		virtual size_t getOutputWidth() = 0;

		virtual size_t getOutputHeight() = 0;

		virtual size_t getLinesCount() = 0;
	};
}

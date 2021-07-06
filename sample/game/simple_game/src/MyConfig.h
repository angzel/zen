
#pragma once

#include "zen_vap.h"
#include "zen_cast.h"
#include "zen_image_jpg.h"
#include "zen_image_png.h"
#include "zen_image_raw.h"
#include "zen_cast.h"
#include "zen_utils.h"
#include "zen_log.h"

using namespace Zen;
using namespace Zen::Vap;

struct MyConfig
{
	std::shared_ptr<FontBrush> m_font_brush;
	Zen::Size2 size;
	Zen::Random ra;
	Zen::ImageJPGCoder jpg_coder;
	Zen::ImagePNGCoder png_coder;
	Zen::ImageRawCoder raw_coder;

};

extern MyConfig gConfig;

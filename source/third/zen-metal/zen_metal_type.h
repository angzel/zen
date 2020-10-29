#pragma once

#include <iostream>

namespace Zen { namespace Metal {

	enum class eBlendFactor
	{
		Zero = 0,
		One = 1,
		SourceColor = 2,
		OneMinusSourceColor = 3,
		SourceAlpha = 4,
		OneMinusSourceAlpha = 5,
		DestinationColor = 6,
		OneMinusDestinationColor = 7,
		DestinationAlpha = 8,
		OneMinusDestinationAlpha = 9,
		SourceAlphaSaturated = 10,
		_Count,
	};
	enum class eMode
	{
		Point = 0,
		Line = 1,
		LineStrip = 2,
		Triangle = 3,
		TriangleStrip = 4,
	};
}}

#define ZEN_METAL_CPP 1
#include "zen_metal_device.h"
#include "zen_metal_pipe.h"
#include "zen_metal_texture.h"
#include "zen_metal_render.h"
#undef ZEN_METAL_CPP

#pragma once

#include <iostream>

namespace Zen { namespace Metal {

	enum class eBlendFactor
	{
		// the same with MTLBlendFactor
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
		None = 0xffff,
	};
	enum class eVertexMode
	{
		Point = 0,
		Line = 1,
		LineStrip = 2,
		Triangle = 3,
		TriangleStrip = 4,
	};
}}

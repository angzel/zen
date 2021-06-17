/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include <cstdint>

namespace Zen {

	struct Point2 {
		float x, y;
	};
	
	struct Size2 {
		float w, h;
	};

	struct Point3 {
		float x, y, z;
	};

	inline Point2 Point2Make(Size2 size)
	{
		return { size.w, size.h };
	}
	inline Size2 Size2Make(Point2 p)
	{
		return { p.x, p.y };
	}
}

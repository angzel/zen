/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
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
	
	struct Rect4f
	{
		float x, y, w, h;
	};
	inline Rect4f RectMake(float x, float y, float w, float h)
	{
		return { x, y, w, h };
	}
	inline Rect4f RectMakeWithDoublePoints(float x0, float y0, float x1, float y1)
	{
		return { x0, y0, x1-x0, y1-y0 };
	}
	inline Point2 Point2Make(float x, float y)
	{
		return {x, y};
	}
	inline Size2 Size2Make(float w, float h)
	{
		return {w, h};
	}
	inline Point2 Point2Make(Size2 size)
	{
		return { size.w, size.h };
	}
	inline Size2 Size2Make(Point2 p)
	{
		return { p.x, p.y };
	}
}

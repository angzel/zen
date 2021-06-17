/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

namespace Zen {
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
}

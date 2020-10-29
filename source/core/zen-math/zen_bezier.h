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

#pragma once

#include "zen_vector.h"
#include "zen_exception.h"

namespace Zen {
	struct Bezier2_3
	{
		Vector2 controls[3];
	};
	struct Bezier2_4
	{
		Vector2 controls[4];
	};
	struct Bezier3_3
	{
		Vector3 controls[3];
	};
	struct Bezier3_4
	{
		Vector3 controls[4];
	};
	inline std::pair<Vector2, Vector2> BezierGetPiar(Bezier2_3 const & bez, float value)
	{
		auto v1 = Vector2Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector2Lerp(bez.controls[1], bez.controls[2], value);
		return std::make_pair(v1, v2);
	}
	inline Vector2 BezierGetPoint(Bezier2_3 const & bez, float value)
	{
		auto v1 = Vector2Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector2Lerp(bez.controls[1], bez.controls[2], value);
		return Vector2Lerp(v1, v2, value);
	}
	inline std::pair<Vector2, Vector2> BezierGetPiar(Bezier2_4 const & bez, float value)
	{
		auto v1 = Vector2Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector2Lerp(bez.controls[1], bez.controls[2], value);
		auto v3 = Vector2Lerp(bez.controls[2], bez.controls[3], value);
		v1 = Vector2Lerp(v1, v2, value);
		v2 = Vector2Lerp(v2, v3, value);
		return std::make_pair(v1, v2);
	}
	inline Vector2 BezierGetPoint(Bezier2_4 const & bez, float value)
	{
		auto v1 = Vector2Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector2Lerp(bez.controls[1], bez.controls[2], value);
		auto v3 = Vector2Lerp(bez.controls[2], bez.controls[3], value);
		v1 = Vector2Lerp(v1, v2, value);
		v2 = Vector2Lerp(v2, v3, value);
		return Vector2Lerp(v1, v2, value);
	}

	inline std::pair<Vector3, Vector3> BezierGetPiar(Bezier3_3 const & bez, float value)
	{
		auto v1 = Vector3Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector3Lerp(bez.controls[1], bez.controls[2], value);
		return std::make_pair(v1, v2);
	}
	inline Vector3 BezierGetPoint(Bezier3_3 const & bez, float value)
	{
		auto v1 = Vector3Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector3Lerp(bez.controls[1], bez.controls[2], value);
		return Vector3Lerp(v1, v2, value);
	}
	inline std::pair<Vector3, Vector3> BezierGetPiar(Bezier3_4 const & bez, float value)
	{
		auto v1 = Vector3Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector3Lerp(bez.controls[1], bez.controls[2], value);
		auto v3 = Vector3Lerp(bez.controls[2], bez.controls[3], value);
		v1 = Vector3Lerp(v1, v2, value);
		v2 = Vector3Lerp(v2, v3, value);
		return std::make_pair(v1, v2);
	}
	inline Vector3 BezierGetPoint(Bezier3_4 const & bez, float value)
	{
		auto v1 = Vector3Lerp(bez.controls[0], bez.controls[1], value);
		auto v2 = Vector3Lerp(bez.controls[1], bez.controls[2], value);
		auto v3 = Vector3Lerp(bez.controls[2], bez.controls[3], value);
		v1 = Vector3Lerp(v1, v2, value);
		v2 = Vector3Lerp(v2, v3, value);
		return Vector3Lerp(v1, v2, value);
	}
}

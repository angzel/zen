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

#include "zen_point.h"

namespace Zen
{
	template<typename _ValueType>
	class TRect
	{
	public:
		typedef _ValueType Type;
		
		Type x, y, width, height;
	public:
		TRect()
		{
		}
		TRect(Type _x, Type _y, Type _width, Type _height)
		{
			x = _x; y = _y;
			width = _width; height = _height;
		}
		TRect(TPoint<2, Type> const & pos, TPoint<2, Type> const & size)
		{
			x = pos.x;
			y = pos.y;
			width = size.x;
			height = size.y;
		}
		TRect & set(TPoint<2, Type> const & pos, TPoint<2, Type> const & size)
		{
			x = pos.x;
			y = pos.y;
			width = size.x;
			height = size.y;
			return *this;
		}
		TRect & set(Type _x, Type _y, Type _width, Type _height)
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
			return *this;
		}
		TPoint<2, Type> pos() const
		{
			return TPoint<2, Type>(x, y);
		}
		TRect & move(TPoint<2, Type> const & pos)
		{
			x = pos.x;
			y = pos.y;
			return *this;
		}
		TPoint<2, Type> size() const
		{
			return TPoint<2, Type>(width, height);
		}
		TRect & resize(TPoint<2, Type> const & size)
		{
			width = size.x;
			height = size.y;
			return *this;
		}
		bool contained(Type _x, Type _y) const
		{
			return (width > 0 ?
							(_x >= x && _x <= x + width) :
							(_x <= x && _x >= x + width))
			&& (height > 0 ?
					(_y >= y && _y <= y + height) :
					(_y <= y && _y >= y + height));
		}
		bool contained(TPoint<2, Type> const & p) const
		{
			return contained(p.x, p.y);
		}
	};
	
	typedef TRect<float> Rect;
}

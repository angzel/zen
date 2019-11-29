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
#include "zen_exception.h"

namespace Zen {
	template<unsigned _kPointDimension>
	class Bezier
	{
	public:
		typedef TPoint<_kPointDimension, float> Point;
		
		virtual void setAnchor(unsigned index, Point anchor) = 0;
		virtual Point getAnchor(unsigned index) = 0;
		virtual unsigned getAnchorCount() = 0;
		virtual std::pair<Point, Point> getTangent(float value) = 0;
		virtual Point getPoint(float value) = 0;
		
		inline Point operator()(float value)
		{
			return getPoint(value);
		}
	};
	
	template<unsigned _kPointDimension, unsigned _kCtrlCount>
	class BezierT : public Bezier<_kPointDimension>
	{
	public:
		static_assert(_kCtrlCount >= 3, "invalid template argument: _kCtrlCount");
		
		static const unsigned kCtrlCount = _kCtrlCount;
		typedef typename Bezier<_kPointDimension>::Point Point;

		BezierT()
		{
		}
		void setAnchor(unsigned index, Point anchor)
		{
			musts(index < kCtrlCount, "out of range");
			mCtrl[index] = anchor;
		}
		Point getAnchor(unsigned index)
		{
			musts(index < kCtrlCount, "out of range");
			return mCtrl[index];
		}
		unsigned getAnchorCount()
		{
			return kCtrlCount;
		}
		std::pair<Point, Point> getTangent(float value)
		{
			Point temp[kCtrlCount-1];
			for (unsigned i = 1; i < kCtrlCount; ++i) {
				temp[i-1] = Point::Scale(mCtrl[i], mCtrl[i-1], value);
			}
			for (unsigned count = kCtrlCount - 1; count > 2; --count)
			{
				for (unsigned i = 1; i < count; ++i) {
					temp[i-1] = Point::Scale(temp[i], temp[i-1], value);
				}
			}
			return std::make_pair(temp[0], temp[1]);
		}
		Point getPoint(float value)
		{
			Point temp[kCtrlCount-1];
			for (unsigned i = 1; i < kCtrlCount; ++i) {
				temp[i-1] = Point::Scale(mCtrl[i], mCtrl[i-1], value);
			}
			for (unsigned count = kCtrlCount - 1; count > 1; --count)
			{
				for (unsigned i = 1; i < count; ++i) {
					temp[i-1] = Point::Scale(temp[i], temp[i-1], value);
				}
			}
			return temp[0];
		}
		
	protected:
		Point mCtrl[kCtrlCount];
	};
	
	
	/** BezierT<2u>
	 -- bezier with 2 ctrl points, stainght line.
	 */
	template<unsigned _kPointDimension>
	class BezierT<_kPointDimension, 2u>
	: public Bezier<_kPointDimension>
	{
	public:
		typedef typename Bezier<_kPointDimension>::Point Point;
		
		static const unsigned kCtrlCount = 2;
		
		BezierT()
		{
		}
		BezierT(Point p0, Point p1)
		{
			mCtrl[0] = p0;
			mCtrl[1] = p1;
		}
		void setAnchor(unsigned index, Point anchor)
		{
			musts(index < kCtrlCount, "out of range");
			mCtrl[index] = anchor;
		}
		Point getAnchor(unsigned index)
		{
			musts(index < kCtrlCount, "out of range");
			return mCtrl[index];
		}
		unsigned getAnchorCount()
		{
			return kCtrlCount;
		}
		std::pair<Point, Point> getTangent(float value)
		{
			return std::make_pair(mCtrl[0], mCtrl[1]);
		}
		Point getPoint(float value)
		{
			return Point::Scale(mCtrl[1], mCtrl[0], value);
		}
		
	protected:
		Point mCtrl[kCtrlCount];
	};
	
	/** BezierT<3u>
	 -- bezier2d with 3 ctrl point
	 */
	template<unsigned _kPointDimension>
	class BezierT<_kPointDimension, 3u>
	: public Bezier<_kPointDimension>
	{
	public:
		typedef typename Bezier<_kPointDimension>::Point Point;
		
		static const unsigned kCtrlCount = 3;
		
		BezierT()
		{
		}
		BezierT(Point c0, Point c1, Point c2)
		{
			mCtrl[0] = c0;
			mCtrl[1] = c1;
			mCtrl[2] = c2;
		}
		void setAnchor(unsigned index, Point anchor)
		{
			musts(index < kCtrlCount, "out of range");
			mCtrl[index] = anchor;
		}
		Point getAnchor(unsigned index)
		{
			musts(index < kCtrlCount, "out of range");
			return mCtrl[index];
		}
		unsigned getAnchorCount()
		{
			return kCtrlCount;
		}
		std::pair<Point, Point> getTangent(float value)
		{
			auto t0 = Point::Scale(mCtrl[1], mCtrl[0], value);
			auto t1 = Point::Scale(mCtrl[2], mCtrl[1], value);
			return std::make_pair(t0, t1);
		}
		Point getPoint(float value)
		{
			auto t0 = Point::Scale(mCtrl[1], mCtrl[0], value);
			auto t1 = Point::Scale(mCtrl[2], mCtrl[1], value);
			return Point::Scale(t1, t0, value);
		}
	protected:
		Point mCtrl[kCtrlCount];
	};
	
	/** BezierT<1u>
	 -- just a point
	 */
	template<unsigned _kPointDimension>
	class BezierT<_kPointDimension, 1u>
	: public Bezier<_kPointDimension>
	{
	public:
		typedef typename Bezier<_kPointDimension>::Point Point;
		static const unsigned kCtrlCount = 1;

		BezierT()
		{
		}
		BezierT(Point c0)
		{
			mCtrl = c0;
		}
		void setAnchor(unsigned index, Point anchor)
		{
			mCtrl = anchor;
		}
		Point getAnchor(unsigned index)
		{
			return mCtrl;
		}
		unsigned getAnchorCount()
		{
			return kCtrlCount;
		}
		std::pair<Point, Point> getTangent(float value)
		{
			return std::make_pair(mCtrl, mCtrl);
		}
		Point getPoint(float value)
		{
			return mCtrl;
		}
	protected:
		Point mCtrl;
	};
}

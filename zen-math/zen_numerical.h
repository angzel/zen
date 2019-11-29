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

#include <cmath>

#define ZEN_F_EPSILON 1.192092896e-07
#define ZEN_F_PI      3.1415926535897932384626433832795
#define ZEN_F_2PI     6.283185307179586476925286766559
#define ZEN_F_PI2     1.5707963267948966192313216916398
#define ZEN_F_PI4     0.78539816339744830961566084581988
#define ZEN_F_SQRT_2  1.4142135623730950488016887242097
#define ZEN_F_SQRT_3  1.7320508075688772935274463415059
#define ZEN_F_SQRT_5  2.2360679774997896964091736687313

namespace Zen {
	
	template<typename _ValueType>
	inline _ValueType SquareValue(_ValueType value)
	{
		return value * value;
	}
	
	template<typename _ValueType>
	inline _ValueType CubeValue(_ValueType value)
	{
		return value * value * value;
	}

	template<typename _FloatType>
	inline bool IsFuzzyZero(_FloatType num, _FloatType var = ZEN_F_EPSILON)
	{
		return num < var && num > -var;
	}

	template<typename _FloatType>
	inline bool IsFuzzyEqual(_FloatType num0, _FloatType num1, _FloatType var = ZEN_F_EPSILON)
	{
		return IsFuzzyZero(num0 - num1, var);
	}
	
	template<typename _ValueType>
	inline _ValueType GetGCD(_ValueType n, _ValueType m)
	{
		if(m == 0 || n == 0) return 1;
		
		if(m < 0) m = -m;
		if(n < 0) n = -n;

		while(m)
		{
			auto t = n % m;
			n = m;
			m = t;
		}
		return n;
	}
}

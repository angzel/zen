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

namespace Zen { namespace Meta {

	template<bool, typename Type, Type v1, Type v2>
	struct ConditionalValue
	{
		static const Type Value = v1;
	};
	
	template<typename Type, Type v1, Type v2>
	struct ConditionalValue<false, Type, v1, v2>
	{
		static const Type Value = v2;
	};
	
	template<typename Type, Type v1, Type v2>
	struct Min : ConditionalValue<(v1 > v2), Type, v2, v1>
	{
	};
	
	template<typename Type, Type v1, Type v2>
	struct Max : ConditionalValue<(v1 > v2), Type, v1, v2>
	{
	};

	/**
	 std::end(a) - std::begin(a)
	 */
	template<typename Type, size_t n>
	inline size_t SizeA(Type (&array)[n])
	{
		return n; // n == sizeof(array)/sizeof(*array)
	}
}}

template<typename _A, typename _B>
bool operator < (_A const & a, _B const & b)
{
	return b > a;
}

template<typename _A, typename _B>
bool operator <= (_A const & a, _B const & b)
{
	return b >= a;
}


template<typename _A, typename _B>
bool operator != (_A const & a, _B const & b)
{
	return !(a == b); //b != a;
}


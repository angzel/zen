/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
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
	inline _ValueType Square(_ValueType value)
	{
		return value * value;
	}
	
	template<typename _ValueType>
	inline _ValueType Cube(_ValueType value)
	{
		return value * value * value;
	}

	template<typename _FloatType>
	inline bool IsFuzzyZero(_FloatType num, _FloatType var = ZEN_F_EPSILON)
	{
		return num < var && num > -var;
	}
	template<typename _FloatType>
	inline _FloatType Lerp(_FloatType start, _FloatType end, _FloatType v)
	{
		return start + (end - start) * v;
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

		do { auto t = n % m; n = m; m = t; } while(m);

		return n;
	}
	inline uint16_t GetMinPowerTwo(uint16_t v)
	{
		if(v == 0) return 0;
		--v;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		return v + 1;
	}
	inline uint32_t GetMinPowerTwo(uint32_t v)
	{
		if(v == 0) return 0;
		--v;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		return v + 1;
	}
	inline uint64_t GetMinPowerTwo(uint64_t v)
	{
		if(v == 0) return 0;
		--v;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		return v + 1;
	}
}

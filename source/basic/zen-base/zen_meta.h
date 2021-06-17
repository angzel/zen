/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include <cstdint>
#include <memory>

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
	inline size_t Count(Type (&array)[n])
	{
		return n; // n == sizeof(array)/sizeof(*array)
	}
	template<typename Type> inline std::shared_ptr<Type> Shared(Type * type)
	{
		return std::shared_ptr<Type>(type);
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


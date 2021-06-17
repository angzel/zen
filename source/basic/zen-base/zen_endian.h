/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

// depending
#include "zen_macro.h"

namespace Zen {
	
	/** EndianSwap32
	 -- switch between big endian and little endian
	 */
	inline std::uint32_t EndianSwap32(std::uint32_t num)
	{
		return ((num & 0xff) << 24) |((num & 0xff00) << 8) |
		((num & 0xff0000) >> 8) | ((num & 0xff000000) >> 24);
	}
	
	/** EndianSwap16
	 -- switch between big endian and little endian
	 */
	inline std::uint16_t EndianSwap16(std::uint16_t num)
	{
		return uint16_t((num & 0xff) << 8) | ((num & 0xff00) >> 8);
	}
	
	/** HostNet32
	 -- switch between host-order and net-order(be)
	 */
	inline std::uint32_t HostNet32(std::uint32_t num)
	{
#if ZEN_IS_BE //ZEN_ENDIAN == ZEN_BE_VALUE
		return num;
#else
		return EndianSwap32(num);
#endif
	}
	
	/** HostNet16
	 -- switch between host-order and net-order(be)
	 */
	inline std::uint16_t HostNet16(std::uint16_t num)
	{
#if ZEN_ENDIAN == ZEN_BE_VALUE
		return num;
#else
		return EndianSwap16(num);
#endif
	}
	
	
	/** Byte4 (union)
	 -- switch between 32b int and 4 char
	 */
	union Byte4 {
		std::uint32_t value;
		std::uint8_t  bytes[4];
		
		Byte4(std::uint32_t v = 0)
		{
			set(v);
		}
		Byte4(std::uint8_t v0, std::uint8_t v1, std::uint8_t v2, std::uint8_t v3)
		{
			set(v0, v1, v2, v3);
		}
		void set(std::uint32_t v)
		{
			value = v;
		}
		void set(std::uint8_t v0, std::uint8_t v1, std::uint8_t v2, std::uint8_t v3)
		{
			bytes[0] = v0;
			bytes[1] = v1;
			bytes[2] = v2;
			bytes[3] = v3;
		}
		
		void setWithBE(std::uint32_t v)
		{
#if ZEN_ENDIAN == ZEN_BE_VALUE
			value = v;
#else
			value = EndianSwap32(v);
#endif
		}
		void setWithLE(std::uint32_t v)
		{
#if ZEN_IS_LE // ZEN_ENDIAN == ZEN_LE_VALUE
			value = v;
#else
			value = EndianSwap32(v);
#endif
		}
		std::uint32_t getBEValue() const
		{
#if ZEN_ENDIAN == ZEN_BE_VALUE
			return value;
#else
			return EndianSwap32(value);
#endif
		}
		std::uint32_t getLEValue() const
		{
#if ZEN_ENDIAN == ZEN_LE_VALUE
			return value;
#else
			return EndianSwap32(v);
#endif
		}
		bool operator == (Byte4 const & o)
		{
			return value == o.value;
		}
		bool operator != (Byte4 const & o)
		{
			return value != o.value;
		}
	};
}

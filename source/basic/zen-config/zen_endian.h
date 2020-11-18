/*
 Copyright (c) 2013 MeherTJ G.
 
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

// depending
#include "zen_os.h"

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
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
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
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
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
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
			value = v;
#else
			value = EndianSwap32(v);
#endif
		}
		void setWithLE(std::uint32_t v)
		{
#if ZEN_BYTE_ORDER == ZEN_LITTLE_ENDIAN
			value = v;
#else
			value = EndianSwap32(v);
#endif
		}
		std::uint32_t getBEValue() const
		{
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
			return value;
#else
			return EndianSwap32(value);
#endif
		}
		std::uint32_t getLEValue() const
		{
#if ZEN_BYTE_ORDER == ZEN_LITTLE_ENDIAN
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

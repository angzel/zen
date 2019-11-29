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

#include <cstdint>

namespace Zen {
#if defined(ZEN_P_SAFETY_CODE)
	static const uint64_t SafetyCode = ZEN_P_SAFETY_CODE;
#else
	static const uint64_t SafetyCode = 0x569A37E84BCF1E69;
#endif

	
	template<typename _ValueType, _ValueType VMask>
	class Safety
	{
	protected:
		_ValueType mValue;
	public:
		Safety(_ValueType value = 0)
		{
			mValue = value ^ VMask;
		}
		operator _ValueType() const
		{
			return mValue ^ VMask;
		}
		Safety & operator = (Safety const & other)
		{
			mValue = other.mValue;
			return *this;
		}
		Safety & operator *= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) * value) ^ VMask;
			return *this;
		}
		Safety & operator /= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) / value) ^ VMask;
			return *this;
		}
		Safety & operator += (_ValueType value)
		{
			mValue = ((mValue ^ VMask) + value) ^ VMask;
			return *this;
		}
		Safety & operator -= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) - value) ^ VMask;
			return *this;
		}
		Safety & operator --()
		{
			mValue = ((mValue ^ VMask) - 1) ^ VMask;
			return *this;
		}
		Safety & operator ++()
		{
			mValue = ((mValue ^ VMask) + 1) ^ VMask;
			return *this;
		}
		_ValueType operator ++(int)
		{
			_ValueType value = *this;
			mValue = ((mValue ^ VMask) + 1) ^ VMask;
			return value;
		}
		_ValueType operator --(int)
		{
			_ValueType value = *this;
			mValue = ((mValue ^ VMask) - 1) ^ VMask;
			return value;
		}
	};
	
	typedef Safety<int64_t, (int64_t)SafetyCode> Int64s;
	
	typedef Safety<uint64_t, (uint64_t)SafetyCode> UInt64s;
	
	typedef Safety<int32_t, (int32_t)SafetyCode> Int32s;
	
	typedef Safety<uint32_t, (uint32_t)SafetyCode> UInt32s;
	
	typedef Safety<int16_t, (int16_t)SafetyCode> Int16s;
	
	typedef Safety<uint16_t, (uint16_t)SafetyCode> UInt16s;
	
	typedef Safety<int8_t, (int8_t)SafetyCode> Int8s;
	
	typedef Safety<uint8_t, (uint8_t)SafetyCode> UInt8s;
}

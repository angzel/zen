/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
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
	class EncryptedInt
	{
	protected:
		_ValueType mValue;
	public:
		EncryptedInt(_ValueType value = 0)
		{
			mValue = value ^ VMask;
		}
		operator _ValueType() const
		{
			return mValue ^ VMask;
		}
		EncryptedInt & operator = (EncryptedInt const & other)
		{
			mValue = other.mValue;
			return *this;
		}
		EncryptedInt & operator *= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) * value) ^ VMask;
			return *this;
		}
		EncryptedInt & operator /= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) / value) ^ VMask;
			return *this;
		}
		EncryptedInt & operator += (_ValueType value)
		{
			mValue = ((mValue ^ VMask) + value) ^ VMask;
			return *this;
		}
		EncryptedInt & operator -= (_ValueType value)
		{
			mValue = ((mValue ^ VMask) - value) ^ VMask;
			return *this;
		}
		EncryptedInt & operator --()
		{
			mValue = ((mValue ^ VMask) - 1) ^ VMask;
			return *this;
		}
		EncryptedInt & operator ++()
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
	
	typedef EncryptedInt<int64_t, (int64_t)SafetyCode> Int64s;
	
	typedef EncryptedInt<uint64_t, (uint64_t)SafetyCode> UInt64s;
	
	typedef EncryptedInt<int32_t, (int32_t)SafetyCode> Int32s;
	
	typedef EncryptedInt<uint32_t, (uint32_t)SafetyCode> UInt32s;
	
	typedef EncryptedInt<int16_t, (int16_t)SafetyCode> Int16s;
	
	typedef EncryptedInt<uint16_t, (uint16_t)SafetyCode> UInt16s;
	
	typedef EncryptedInt<int8_t, (int8_t)SafetyCode> Int8s;
	
	typedef EncryptedInt<uint8_t, (uint8_t)SafetyCode> UInt8s;
}

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_md5.h"
#include "zen_cast.h"
#include <memory.h>

namespace Zen {
	inline uint32_t B2I(uint8_t const * byte)
	{
		return ((uint32_t)*byte)|
		((uint32_t)(*(byte+1)<< 8))|
		((uint32_t)(*(byte+2)<< 16))|
		((uint32_t)(*(byte+3)<< 24));
	}
	inline void I2B(uint32_t v, uint8_t * byte)
	{
		byte[0] = (uint8_t)(v & 0xff);
		byte[1] = (uint8_t)((v >> 8)& 0xff);
		byte[2] = (uint8_t)((v >> 16)& 0xff);
		byte[3] = (uint8_t)((v >> 24)& 0xff);
	}
	
	void MD5Util::start()
	{
		mTotal[0] = 0;
		mTotal[1] = 0;
		mState[0] = 0x67452301;
		mState[1] = 0xEFCDAB89;
		mState[2] = 0x98BADCFE;
		mState[3] = 0x10325476;
	}
	void MD5Util::process(uint8_t const data[64])
	{
		uint32_t A, B, C, D, X[16];
		
		for (int i = 0; i < 16; ++i)
		{
			X[i] = B2I(data + (i<<2));
		}
		
#define S(x,n) ((x << n)| ((x & 0xFFFFFFFF)>> (32 - n)))
		
#define P(a,b,c,d,k,s,t){ a += F(b,c,d)+ X[k] + t; a = S(a,s)+ b; }
		
		A = mState[0];
		B = mState[1];
		C = mState[2];
		D = mState[3];
		
#define F(x,y,z)(z ^ (x & (y ^ z)))
		
		P(A, B, C, D,  0,  7, 0xD76AA478);
		P(D, A, B, C,  1, 12, 0xE8C7B756);
		P(C, D, A, B,  2, 17, 0x242070DB);
		P(B, C, D, A,  3, 22, 0xC1BDCEEE);
		P(A, B, C, D,  4,  7, 0xF57C0FAF);
		P(D, A, B, C,  5, 12, 0x4787C62A);
		P(C, D, A, B,  6, 17, 0xA8304613);
		P(B, C, D, A,  7, 22, 0xFD469501);
		P(A, B, C, D,  8,  7, 0x698098D8);
		P(D, A, B, C,  9, 12, 0x8B44F7AF);
		P(C, D, A, B, 10, 17, 0xFFFF5BB1);
		P(B, C, D, A, 11, 22, 0x895CD7BE);
		P(A, B, C, D, 12,  7, 0x6B901122);
		P(D, A, B, C, 13, 12, 0xFD987193);
		P(C, D, A, B, 14, 17, 0xA679438E);
		P(B, C, D, A, 15, 22, 0x49B40821);
		
#undef F
		
#define F(x,y,z)(y ^ (z & (x ^ y)))
		
		P(A, B, C, D,  1,  5, 0xF61E2562);
		P(D, A, B, C,  6,  9, 0xC040B340);
		P(C, D, A, B, 11, 14, 0x265E5A51);
		P(B, C, D, A,  0, 20, 0xE9B6C7AA);
		P(A, B, C, D,  5,  5, 0xD62F105D);
		P(D, A, B, C, 10,  9, 0x02441453);
		P(C, D, A, B, 15, 14, 0xD8A1E681);
		P(B, C, D, A,  4, 20, 0xE7D3FBC8);
		P(A, B, C, D,  9,  5, 0x21E1CDE6);
		P(D, A, B, C, 14,  9, 0xC33707D6);
		P(C, D, A, B,  3, 14, 0xF4D50D87);
		P(B, C, D, A,  8, 20, 0x455A14ED);
		P(A, B, C, D, 13,  5, 0xA9E3E905);
		P(D, A, B, C,  2,  9, 0xFCEFA3F8);
		P(C, D, A, B,  7, 14, 0x676F02D9);
		P(B, C, D, A, 12, 20, 0x8D2A4C8A);
		
#undef F
		
#define F(x,y,z)(x ^ y ^ z)
		
		P(A, B, C, D,  5,  4, 0xFFFA3942);
		P(D, A, B, C,  8, 11, 0x8771F681);
		P(C, D, A, B, 11, 16, 0x6D9D6122);
		P(B, C, D, A, 14, 23, 0xFDE5380C);
		P(A, B, C, D,  1,  4, 0xA4BEEA44);
		P(D, A, B, C,  4, 11, 0x4BDECFA9);
		P(C, D, A, B,  7, 16, 0xF6BB4B60);
		P(B, C, D, A, 10, 23, 0xBEBFBC70);
		P(A, B, C, D, 13,  4, 0x289B7EC6);
		P(D, A, B, C,  0, 11, 0xEAA127FA);
		P(C, D, A, B,  3, 16, 0xD4EF3085);
		P(B, C, D, A,  6, 23, 0x04881D05);
		P(A, B, C, D,  9,  4, 0xD9D4D039);
		P(D, A, B, C, 12, 11, 0xE6DB99E5);
		P(C, D, A, B, 15, 16, 0x1FA27CF8);
		P(B, C, D, A,  2, 23, 0xC4AC5665);
		
#undef F
		
#define F(x,y,z)(y ^ (x | ~z))
		
		P(A, B, C, D,  0,  6, 0xF4292244);
		P(D, A, B, C,  7, 10, 0x432AFF97);
		P(C, D, A, B, 14, 15, 0xAB9423A7);
		P(B, C, D, A,  5, 21, 0xFC93A039);
		P(A, B, C, D, 12,  6, 0x655B59C3);
		P(D, A, B, C,  3, 10, 0x8F0CCC92);
		P(C, D, A, B, 10, 15, 0xFFEFF47D);
		P(B, C, D, A,  1, 21, 0x85845DD1);
		P(A, B, C, D,  8,  6, 0x6FA87E4F);
		P(D, A, B, C, 15, 10, 0xFE2CE6E0);
		P(C, D, A, B,  6, 15, 0xA3014314);
		P(B, C, D, A, 13, 21, 0x4E0811A1);
		P(A, B, C, D,  4,  6, 0xF7537E82);
		P(D, A, B, C, 11, 10, 0xBD3AF235);
		P(C, D, A, B,  2, 15, 0x2AD7D2BB);
		P(B, C, D, A,  9, 21, 0xEB86D391);
		
#undef F
		
		mState[0] += A;
		mState[1] += B;
		mState[2] += C;
		mState[3] += D;
	}
	void MD5Util::update(void const * data, size_t length)
	{
		uint8_t * input = (uint8_t*)data;

		uint32_t left, fill;
		
		if(! length)return;
		
		left = (mTotal[0] >> 3)& 0x3F;
		fill = 64 - left;
		
		mTotal[0] += length <<  3;
		mTotal[1] += length >> 29;
		
		mTotal[0] &= 0xFFFFFFFF;
		mTotal[1] += mTotal[0] < length << 3;
		
		if(left && length >= fill)
		{
			memcpy(mBuffer + left, input, fill);
			process(mBuffer);
			length -= fill;
			input  += fill;
			left = 0;
		}
		
		while(length >= 64)
		{
			process(input);
			length -= 64;
			input  += 64;
		}
		
		if(length)
		{
			memcpy(mBuffer + left, input, length);
		}
		
	}
	std::string MD5Util::finish(bool upper_case)
	{
		uint8_t digits[16];
		uint8_t padding[64] =
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		uint32_t last, padn;
		uint8_t msglen[8];

		MD5Util o = *this;
		
		I2B(o.mTotal[0], msglen);
		I2B(o.mTotal[1], msglen + 4);
		
		last = (o.mTotal[0] >> 3) & 0x3F;
		padn = (last < 56) ? (56 - last) : (120 - last);
		
		o.update(padding, padn);
		o.update(msglen, 8);
		
		I2B(o.mState[0], digits);
		I2B(o.mState[1], digits + 4);
		I2B(o.mState[2], digits + 8);
		I2B(o.mState[3], digits + 12);
		
		std::string res;
		for (uint8_t c : digits)
		{
			res.push_back(Zen::ValueToHexDigit(c / 16, upper_case));
			res.push_back(Zen::ValueToHexDigit(c % 16, upper_case));
		}
		return res;
	}
	std::vector<uint8_t> MD5Util::finishInBytes()
	{
		std::vector<uint8_t> res;
		res.resize(16, 0);
		uint8_t * digits = res.data();
		uint8_t padding[64] =
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		uint32_t last, padn;
		uint8_t msglen[8];

		MD5Util o = *this;
		
		I2B(o.mTotal[0], msglen);
		I2B(o.mTotal[1], msglen + 4);
		
		last = (o.mTotal[0] >> 3) & 0x3F;
		padn = (last < 56) ? (56 - last) : (120 - last);
		
		o.update(padding, padn);
		o.update(msglen, 8);
		
		I2B(o.mState[0], digits);
		I2B(o.mState[1], digits + 4);
		I2B(o.mState[2], digits + 8);
		I2B(o.mState[3], digits + 12);
		return res;
	}
	uint64_t MD5Util::finishInNumber()
	{
		uint64_t res;
		uint8_t * digits = (uint8_t*)(void*)&res;
		
		uint8_t padding[64] =
		{
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};
		uint8_t msglen[8];

		MD5Util o = *this;
		
		I2B(o.mTotal[0], msglen);
		I2B(o.mTotal[1], msglen + 4);
		
		uint32_t last = (o.mTotal[0] >> 3) & 0x3F;
		uint32_t padn = (last < 56) ? (56 - last) : (120 - last);
		
		o.update(padding, padn);
		o.update(msglen, 8);
		
		/// use middle 8 bytes
		I2B(o.mState[1], digits + 0);
		I2B(o.mState[2], digits + 4);
		return res;
	}

	std::string MD5(void const *data, size_t size, bool upper_case)
	{
		MD5Util coder;
		coder.start();
		coder.update((uint8_t*)data, (uint32_t)size);
		return coder.finish(upper_case);
	}
}


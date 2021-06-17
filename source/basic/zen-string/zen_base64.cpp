/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#include "zen_base64.h"
#include "zen_cast.h"

namespace Zen {
	
	static const int8_t Demap[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};
	
	static const char Alphabet[] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/',
	};
	
	Base64 * Base64::Get()
	{
		static Base64 me;
		return &me;
	}

	std::string Base64::encode(void const * buf, size_t size)
	{
		std::string res;
		auto dest_size = (size / 3) << 2;
		auto ex = (size % 3);
		if(ex == 0) res.resize(dest_size, 0);
		else res.resize(dest_size + 4, 0);
		
		auto data = (uint8_t const *)buf;
		for (size_t i = 0; i < dest_size; )
		{
			unsigned b0 = data[0] >> 2;
			unsigned b1 = (((unsigned)data[0] & 0x3) << 4) | ((unsigned)data[1] >> 4);
			unsigned b2 = (((unsigned)data[1] & 0xf) << 2) | ((unsigned)data[2] >> 6);
			unsigned b3 = data[2] & 0x3f;
			res[i++] = Alphabet[b0];
			res[i++] = Alphabet[b1];
			res[i++] = Alphabet[b2];
			res[i++] = Alphabet[b3];
			data += 3;
		}
		if(ex == 1)
		{
			unsigned b0 = (unsigned)data[0] >> 2;
			unsigned b1 = ((unsigned)data[2] & 0x3) << 4;
			res[dest_size] = Alphabet[b0];
			res[dest_size + 1] = Alphabet[b1];
			res[dest_size + 2] = res[dest_size + 3] = '=';
		}
		else if(ex == 2)
		{
			unsigned b0 = (unsigned)data[0] >> 2;
			unsigned b1 = (((unsigned)data[0] & 0x3) << 4) | ((unsigned)data[1] >> 4);
			unsigned b2 = ((unsigned)data[1] & 0xf) << 2;
			res[dest_size] = Alphabet[b0];
			res[dest_size + 1] = Alphabet[b1];
			res[dest_size + 2] = Alphabet[b2];
			res[dest_size + 3] = '=';
		}
		return res;
	}
	
	std::vector<uint8_t> Base64::decode(void const * buf, size_t size)
	{
		std::vector<uint8_t> res;
		auto bsize = (size >> 2) * 3;
		if(!bsize) return res;
		auto data = (uint8_t const *)buf;
		
		if(data[size-1] == '=')
		{
			if(data[size - 2] == '=') bsize -= 2;
			else --bsize;
		}
		res.resize(bsize);
		for (size_t i = 0; i < bsize; )
		{
			auto v0 = (uint8_t)Demap[(int)*data++];
			auto v1 = (uint8_t)Demap[(int)*data++];
			auto v2 = (uint8_t)Demap[(int)*data++];
			auto v3 = (uint8_t)Demap[(int)*data++];
			res[i++] = (uint8_t)((v0 << 2) | (v1 >> 4));
			res[i++] = (uint8_t)((v1 << 4) | (v2 >> 2));
			res[i++] = (uint8_t)((v2 << 6) | v3);
		}
		return res;
	}
	
	std::string Base64::encode(std::string const & data)
	{
		return encode((void const *)data.data(), data.size());
	}
	
	std::vector<uint8_t> Base64::decode(std::string const & str)
	{
		return decode((void const *)str.data(), str.size());
	}
	
	bool Base64::check(std::string const & coded)
	{
		auto size = coded.size();
		if(size == 0) return true;
		if((size & 0x3) != 0) return false; // must be (4 bytes x N)
		
		for (size_t i = 0; i < size - 2; ++i)
		{
			if(Demap[(int)coded[i]] == -1) return false;
		}
		auto c1 = coded[size - 1];
		auto c2 = coded[size - 2];
		return (c1 == '=' && c2 == '=') ||
		(c1 == '=' && Demap[(int)c2] != -1) ||
		(Demap[(int)c1] != -1 && Demap[(int)c2] != -1);
	}
	int Base64::demap(uint8_t ch)
	{
		return Demap[ch];
	}
	
	uint8_t Base64::map(int v64)
	{
		return Alphabet[v64&0x3f];
	}
}

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Zen {
	class Base64
	{
	public:
		static Base64 * Get();
		
		int demap(uint8_t ch);
		
		uint8_t map(int v64);
		
		std::string encode(std::string const & source);
		
		std::vector<uint8_t> decode(std::string const & str);
		
		std::string encode(void const * buf, size_t size);

		std::vector<uint8_t> decode(void const * buf, size_t size);
		
		bool check(std::string const & coded);
	};
}

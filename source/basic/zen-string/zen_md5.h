/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace Zen {
	
	class MD5Util
	{
	public:
		MD5Util() = default;
		
		void start();
		
		void update(void const * data, size_t length);
		
		std::string finish(bool upper_case = false);
		
		std::vector<uint8_t> finishInBytes();
		
		uint64_t finishInNumber();
	protected:
		uint32_t mTotal[2];
		uint32_t mState[4];
		uint8_t mBuffer[64];
		
		void process(uint8_t const data[64]);
	};

	std::string MD5(void const* data, size_t size, bool upper_case);
}

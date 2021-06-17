/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */
#pragma once

#include <string>
#include <cstdint>

namespace Zen {
	/**
	 alpha, number and the chars (-_.*): not change.
	 space : change to '+'
	 other characters: %xx
	 */
	extern bool URLCheckCoding(std::string const & str);

	extern std::string URLEncode(void const * buf, size_t size);
	
	extern std::vector<unsigned char> URLDecode(void const * buf, size_t size);

	extern std::string URLEncode(std::string const & data);
	
	extern std::vector<unsigned char> URLDecode(std::string const & data);
	
}

/*
 Copyright (c) 2013- MeherTJ G.
 
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

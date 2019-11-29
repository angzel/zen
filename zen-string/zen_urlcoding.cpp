/*
 Copyright (c) 2013- ClearSky G.
 
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

#include <string>
#include <cstdint>
#include "zen_cast.h"

namespace Zen {
	std::vector<unsigned char> URLDecode(void const * buf, size_t size)
	{
		std::vector<unsigned char> res;
		int state = 0;
		int v = 0;
		auto data = (unsigned char *)buf;
		for (size_t i = 0; i < size; i++)
		{
			if(state == 0)
			{
				if(data[i] == '%') state = 1;
				else if(data[i] == '+') res.push_back(0x20);
				else res.push_back(data[i]);
			}
			else if(state == 1)
			{
				v = HexDigitToValue(data[i]) << 4;
				state = 2;
			}
			else if(state == 2)
			{
				v += HexDigitToValue(data[i]);
				res.push_back((unsigned char)v);
				state = 0;
			}
		}
		return res;
	}

	std::string URLEncode(void const * buf, size_t size)
	{
		std::string res;

		auto data = (unsigned char const *)buf;
		for (size_t i = 0; i < size; ++i)
		{
			auto c = data[i];
			if(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '*')
			{
				res.push_back((char)c);
			}
			else if(c == 0x20)
			{
				res.push_back('+');
			}
			else
			{
				res.push_back('%');
				res.push_back(ValueToHexDigit(c >> 4));
				res.push_back(ValueToHexDigit(c & 0xf));
			}
		}
		return res;
	}

	bool URLCheckCoding(std::string const & coded)
	{
		int state = 0;
		for (auto c : coded)
		{
			switch (state) {
				case 0:
					if(c == '%') state = 1;
					break;
				case 1:
					if(!IsHexChar(c)) return false;
					state = 2;
					break;
				case 2:
					if(!IsHexChar(c)) return false;
					state = 0;
					break;
			}
		}
		return (state == 0);
	}

	std::vector<unsigned char> URLDecode(std::string const & data)
	{
		return URLDecode(data.data(), data.size());
	}

	std::string URLEncode(std::string const & data)
	{
		return URLEncode((void const *)data.data(), data.size());
	}
}


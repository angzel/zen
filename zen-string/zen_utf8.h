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
#include <string>
#include <cstdint>

namespace Zen {

	std::u32string UTF8ToUnicode(std::string const & utf8)
	{
		if (utf8.empty())
		{
			return {};
		}

		std::u32string res;

		for (size_t i = 0; i < utf8.size(); )
		{
			auto c = (unsigned char)utf8[i];
			char32_t wideChar = 0;
			if ((c & 0x80) == 0)
			{
				wideChar = c;
				++i;
			}
			else if((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
			{
				if(i + 2 > utf8.size()) break;
				wideChar  = (char32_t(c) & 0x3F) << 6;
				wideChar |= (char32_t(utf8[i+1]) & 0x3F);
				i += 2;
			}
			else if((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
			{
				if(i + 3 > utf8.size()) break;
				wideChar  = (char32_t(c) & 0x1F) << 12;
				wideChar |= (char32_t(utf8[i+1]) & 0x3F) << 6;
				wideChar |= (char32_t(utf8[i+2]) & 0x3F);
				i += 3;
			}
			else if((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
			{
				if(i + 4 > utf8.size()) break;
				wideChar  = (char32_t(c) & 0x0F) << 18;
				wideChar |= (char32_t(utf8[i+1]) & 0x3F) << 12;
				wideChar |= (char32_t(utf8[i+2]) & 0x3F) << 6;
				wideChar |= (char32_t(utf8[i+3]) & 0x3F);
				i += 4;
			}
			else///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
			{
				if(i + 4 > utf8.size()) break;
				wideChar  = (char32_t(c) & 0x07) << 24;
				wideChar |= (char32_t(utf8[i+1]) & 0x3F) << 18;
				wideChar |= (char32_t(utf8[i+2]) & 0x3F) << 12;
				wideChar |= (char32_t(utf8[i+3]) & 0x3F) << 6;
				wideChar |= (char32_t(utf8[i+4]) & 0x3F);
				i += 4;
			}
			res.push_back(wideChar);
		}

		return res;
	}

	std::string UnicodeToUTF8(std::u32string const & strRes)
	{
		std::string utf8;
		for (char32_t c : strRes)
		{
			auto i = (uint32_t)c;
			if (i < 0x80)
			{
				utf8.push_back((char)i);
			}
			else if(i < 0x800)
			{
				utf8.push_back((char)(0xc0 | (i >> 6)));
				utf8.push_back((char)(0x80 | (i & 0x3f)));
			}
			else if(i < 0x10000 )
			{
				utf8.push_back((char)(0xe0 | (i >> 12)));
				utf8.push_back((char)(0x80 | ((i >> 6)& 0x3f)));
				utf8.push_back((char)(0x80 | (i & 0x3f)));
			}
			else if(i < 0x200000 )
			{
				utf8.push_back((char)(0xf0 | (i >> 18)));
				utf8.push_back((char)(0x80 | ((i >> 12)& 0x3f)));
				utf8.push_back((char)(0x80 | ((i >> 6)& 0x3f)));
				utf8.push_back((char)(0x80 | (i & 0x3f)));
			}
			else
			{
				utf8.push_back((char)(0xf8 | (i >> 24)));
				utf8.push_back((char)(0x80 | ((i >> 18)& 0x3f)));
				utf8.push_back((char)(0x80 | ((i >> 12)& 0x3f)));
				utf8.push_back((char)(0x80 | ((i >> 6)& 0x3f)));
				utf8.push_back((char)(0x80 | (i & 0x3f)));
			}
		}
		return utf8;
	}
}

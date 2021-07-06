/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once
#include <string>
#include <cstdint>

namespace Zen {
	
	/// decode utf8 to unicode string.
	/// @param utf8 source string.
	/// @return <u32string> unicode string.
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
	
	/// encode unicode string to utf8
	/// @param uniStr source unicode string.
	/// @return utf8 string.
	std::string UnicodeToUTF8(std::u32string const & uniStr)
	{
		std::string utf8;
		for (char32_t c : uniStr)
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

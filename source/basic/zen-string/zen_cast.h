/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <cstdio>

namespace Zen {

//	ToNumber(s to i)
	template<typename _ValueType>
	inline _ValueType ToNumber(std::string const & value)
	{
		std::stringstream sha;
		sha << value;
		_ValueType num = 0;
		sha >> num;
		return num;
	}
	
//	ToString(i to s)
	template<typename _ValueType>
	inline std::string ToString(_ValueType value)
	{
#if defined(__ANDROID__)
		std::stringstream sha;
		sha << value;
		std::string str;
		sha >> str;
		return str;
#else
		return std::move(std::to_string(value));
#endif
	}

//	ToString (data to s)
	template<typename _Char>
	inline std::string ToString(std::vector<_Char> const & data)
	{
		return std::string(data.begin(), data.end());
	}
	
	template<typename _Char>
	inline std::string ToString(std::deque<_Char> const & data)
	{
		return std::string(data.begin(), data.end());
	}
	
	inline std::string ToString(void const * data, size_t bytes)
	{
		auto buf = (char const *)data;
		return std::string(buf, buf + bytes);
	}

//	ToData (s to data)
	inline std::vector<uint8_t> ToData(std::string const & s)
	{
		return std::vector<uint8_t>(s.begin(), s.end());
	}

	inline char ValueToHexDigit(uint8_t v16, bool upper_case = false)
	{
		static char tab[2][16] = {
            { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' },
            { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' },
		};
		return tab[upper_case?1:0][v16 & 0xf];
	}

	template<typename _ValueType>
	inline char ValueToHexDigit(_ValueType const & v16, bool upper_case = false)
	{
		return ValueToHexDigit((uint8_t)(v16&0xf), upper_case);
	}

	inline int HexDigitToValue(char hex)
	{
		if(hex >= '0' && hex <= '9') return hex - '0';
		if(hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
		if(hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
		return -1;
	}
	
	inline bool IsHexChar(char c)
	{
		return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
	}

	template<typename _ValueType>
	inline std::string ValueToHexNumber(_ValueType value)
	{
		if(value == 0) return "0";
		std::string temp;
		while(value)
		{
			auto r = (uint8_t)(value & 0xf);
			temp.push_back(ValueToHexDigit(r));
			value >>= 4;
		}
		std::string res;
		for(auto i = temp.rbegin(); i != temp.rend(); ++i) res.push_back(*i);
		return res;
	}
	
	template<typename _ValueType>
	inline _ValueType HexNumberToValue(std::string const & hex)
	{
		_ValueType value = 0;
		for(auto i : hex)
		{
			auto v = HexDigitToValue(i);
			if(v == -1) break;
			value <<= 4;
			value += v;
		}
		return value;
	}
	inline std::string BufferToHexString(void const * data, size_t size)
	{
		std::string res;
		res.reserve(size << 1);
		
		auto buf = (uint8_t const *)data;
		for (size_t i = 0; i < size; ++i)
		{
			res.push_back(Zen::ValueToHexDigit(buf[i] >> 4));
			res.push_back(Zen::ValueToHexDigit(buf[i] & 0xf));
		}
		return res;
	}
	
	template<typename _ValueType>
	inline std::string BufferToHexString(std::vector<_ValueType> const & data)
	{
		return HexToString(data.data(), data.size());
	}
	
	inline std::vector<uint8_t> HexStringToBuffer(std::string const & s)
	{
		std::vector<uint8_t> data;
		data.reserve(s.size() >> 1);
		
		for(decltype(s.size()) i = 1; i < s.size(); i += 2)
		{
			int v = (Zen::HexDigitToValue(s[i-1]) << 4) | Zen::HexDigitToValue(s[i]);
			data.push_back((uint8_t)v);
		}
		return data;
	}

}


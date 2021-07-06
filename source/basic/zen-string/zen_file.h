/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

namespace Zen {
	inline std::string GetFilePathExtention(std::string const & path)
	{
		auto pos = path.find_last_of(".");
		if(pos == path.npos) return "";
		return path.substr(pos+1);
	}
	template<class tFStream>
	inline std::fstream::pos_type GetFileSize(tFStream & stream)
	{
		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		if(size < 0) return 0;
		else return size;
	}

	template<class tFStream>
	inline std::string LoadStreamToString(tFStream & stream)
	{
		std::string str;
		str.reserve((std::string::size_type)GetFileSize(stream));
		stream.seekg(0);

		int c = 0;
		while((c = stream.get()) > 0) str.push_back((char)c);
		return str;

	}

	template<class tFStream>
	inline std::vector<uint8_t> LoadStreamToBuffer(tFStream & stream)
	{
		std::vector<uint8_t> data;
		data.resize((std::string::size_type)GetFileSize(stream));
		stream.seekg(0);

		stream.read((char*)data.data(), data.size());
		if(stream.gcount() != data.size())
		{
			data.clear();
		}
		return data;
	}

	inline std::string LoadFileToString(std::string const & path)
	{
		std::fstream s(path, std::ios::in);
		if(!s) return "";
		return LoadStreamToString(s);
	}

	inline std::vector<uint8_t> LoadFileToBuffer(std::string const & path)
	{
		std::fstream s(path, std::ios::in | std::ios::binary);
		if(!s) return {};
		return LoadStreamToBuffer(s);
	}

	
	inline bool WriteBufferToFile(std::string const & path, void const * data, size_t size)
	{
		std::fstream s(path, std::ios::out | std::ios::binary);
		if(!s) return false;
		return s.write((const char*)data, size).good();
	}
    inline bool WriteBufferToFile(std::string const & path, std::vector<uint8_t> const & data)
    {
	    return WriteBufferToFile(path, data.data(), data.size());
    }

	inline bool AppendBufferToFile(std::string const & path, void const * data, size_t size)
	{
		std::fstream s(path, std::ios::out | std::ios::binary | std::ios::app );
		if(!s) return {};
		return s.write((const char*)data, size).good();
	}
    inline bool AppendBufferToFile(std::string const & path, std::vector<uint8_t> const &  data)
    {
        return AppendBufferToFile(path, data.data(), data.size());
    }

	inline bool AppendStringToFile(std::string const & path, void const * data, size_t size)
	{
		std::fstream s(path, std::ios::out | std::ios::app );
		if(!s) return false;
		return s.write((const char*)data, size).good();
	}
    inline bool AppendStringToFile(std::string const & path, std::string const & data)
    {
        std::fstream s(path, std::ios::out | std::ios::app );
        if(!s) return false;
        return s.write((char const*)data.data(), data.size()).good();
    }
    inline bool WriteStringToFile(std::string const & path, void const * data, size_t size)
    {
        std::fstream s(path, std::ios::out);
        if(!s) return false;
        return s.write((char const *)data, size).good();
    }
    inline bool WriteStringToFile(std::string const & path, std::string const & data)
    {
	    return AppendStringToFile(path, data.data(), data.size());
    }
}

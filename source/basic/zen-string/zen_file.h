/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
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
	inline std::fstream::pos_type GetFileSize(std::fstream & stream)
	{
		stream.seekg(0, std::ios::end);
		auto size = stream.tellg();
		if(size < 0) return 0;
		else return size;
	}

	inline std::string LoadFileToString(std::fstream & stream)
	{
		std::string str;
		str.reserve((std::string::size_type)GetFileSize(stream));
		stream.seekg(0);

		int c = 0;
		while((c = stream.get()) > 0) str.push_back((char)c);
		return str;

	}

	inline std::vector<uint8_t> LoadFileToBuffer(std::fstream & stream)
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


	inline std::string LoadFileToString(std::string const & path, bool binary = false)
	{
		std::fstream s(path, std::ios::in | (binary?std::ios::binary:0));
		if(!s) return "";
		return LoadFileToString(s);
	}

	inline std::vector<uint8_t> LoadFileToBuffer(std::string const & path, bool binary = true)
	{
		std::fstream s(path, std::ios::in | (binary?std::ios::binary:0));
		if(!s) return {};
		return LoadFileToBuffer(s);
	}

	inline bool WriteBufferToFile(std::string const & path, std::vector<uint8_t> const & data, bool binary = true)
	{
		std::fstream s(path, std::ios::out | (binary?std::ios::binary:0));
		if(!s) return {};
		return s.write((char*)data.data(), data.size()).good();
	}

	inline bool AppendBufferToFile(std::string const & path, std::vector<uint8_t> const &  data, bool binary = true)
	{
		std::fstream s(path, std::ios::out | (binary?std::ios::binary:0) | std::ios::app );
		if(!s) return {};
		return s.write((char*)data.data(), data.size()).good();
	}
	inline bool WriteStringToFile(std::string const & path, std::string const & data, bool binary = false)
	{
		std::fstream s(path, std::ios::out | (binary?std::ios::binary:0));
		if(!s) return {};
		return s.write((char*)data.data(), data.size()).good();
	}

	inline bool AppendStringToFile(std::string const & path, std::string const & data, bool binary = false)
	{
		std::fstream s(path, std::ios::out | (binary?std::ios::binary:0) | std::ios::app );
		if(!s) return {};
		return s.write((char*)data.data(), data.size()).good();
	}

}

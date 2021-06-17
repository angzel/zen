/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#pragma once

#include <map>
#include <vector>
#include <vector>
#include <string>
#include "zen_csv.h"

namespace Zen {

	/**
	 you might use this file to load your multi-language strings.
	 
	 a sample csv content may be:
	 ====== begin (no this line in csv) =====
	 
	 key1, text1, text2, text3
	 key2, text1, text2, text3
	 key3, text1, text2, text3
	 ...
	 
	 ====== end (no this line in csv) =====
	 
	 */
	class Localization
	{
	public:
		typedef std::vector<std::string> ItemType;
		
		typedef std::string KeyType;
		
	protected:
		
		std::map<KeyType, ItemType> mData;
		
		size_t mIndex;
		
	public:
		
		Localization()
		{
			mIndex = 0;
		}

		bool initWithCSVContent(CSVContent const & data)
		{
			for (auto & i : data.rows)
			{
				if(i.empty()) continue; // jump empty lines
				mData[i.front()].assign(i.begin() + 1, i.end());
			}
			return true;
		}

		ItemType & getItem(std::string const & key)
		{
			return mData[key];
		}
		ItemType & setText(std::string const & key, size_t lang_index, std::string const & text)
		{
			auto & item = mData[key];
			if(item.size() <= lang_index) item.resize(lang_index + 1);
			item[lang_index] = text;
			return item;
		}
		ItemType & setText(std::string const & key, std::string const & text)
		{
			return setText(key, mIndex, text);
		}


		void setLanguageIndex(size_t index)
		{
			mIndex = index;
		}

		size_t getLanguageIndex() const
		{
			return mIndex;
		}

		std::string getText(std::string const & key)
		{
			auto iter = mData.find(key);
			if(iter == mData.end() || iter->second.empty()) return "";

			if (iter->second.size() > mIndex) return iter->second[mIndex];
			return iter->second[0];
		}
	};
}

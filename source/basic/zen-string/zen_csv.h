/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#pragma once

#include <map>
#include <vector>
#include <string>

namespace Zen {

	struct CSVContent {
		typedef std::vector<std::string> RowType;
		std::vector<RowType> rows;
	};

	class CSVLoader
	{
	protected:
		CSVContent mTable;
	public:
		
		CSVLoader();
		
		void clear();
		
		bool decode(std::string content);
		
		std::string encode() const;
		
		CSVContent & getContent();
		
		CSVContent const & getContent() const;
	};
}

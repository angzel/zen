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

#include "zen_csv.h"

namespace Zen {
	static const char Quote = '"';
	static const char Comma = ',';
	static const char Endl = '\n';
	static const char Return = '\r';

	CSVLoader::CSVLoader()
	{
	}
	
	void CSVLoader::clear()
	{
		mTable.rows.clear();
	}
	
	std::string CSVLoader::encode() const
	{
		std::string res;
		int rn = 0;
		for(auto & r : mTable.rows)
		{
			if(rn)
			{
				res.push_back(Return);
				res.push_back(Endl);
			}
			++rn;

			int cn = 0;
			for(auto & c : r)
			{
				if(cn) res.push_back(Comma);
				++cn;
				
				if(c.find(Comma) != c.npos || c.find(Quote) != c.npos || c.find(Endl) != c.npos || c.find(Return) != c.npos)
				{
					res.push_back(Quote);
					for(auto i : c)
					{
						if(i == Quote)
						{
							res.push_back(Quote);
							res.push_back(Quote);
						}
						else
						{
							res.push_back(i);
						}
					}
					res.push_back(Quote);
				}
				else
				{
					res.append(c);
				}
			}
		}
		return res;
	}
	
	CSVContent & CSVLoader::getContent()
	{
		return mTable;
	}
	
	CSVContent const & CSVLoader::getContent() const
	{
		return mTable;
	}
	
	
	class _CSVLoaderHelp
	{
		CSVContent::RowType & row;
		std::string const & data;
		size_t & index;
		std::string temp;
		bool end;
	public:
		_CSVLoaderHelp(std::string const & data_, size_t & index_, CSVContent::RowType & row_)
		: data(data_), index(index_), row(row_)
		{
			end = false;
		}
		bool readRow()
		{
			return readCol();
		}
	private:
		bool readCol()
		{
			if(index < data.size() && data[index] == Quote)
			{
				++index;
				return readQuote();
			}
			return readNoQuote();
		}
		bool readQuote()
		{
			if(index >= data.size())
				return false;
			auto c = data[index++];
			
			if(c == Quote) return onQuoteEnd();
			temp.push_back(c);
			return readQuote();
		}
		bool onQuoteEnd()
		{
			if(index >= data.size()) return onNoMore();
			auto c = data[index++];
			
			if(c == Endl) return onEnd();
			if(c == Return) return onRt();
			if(c == Comma) return onSep();
			if(c == Quote)
			{
				temp.push_back(Quote);
				return readQuote();
			}
			else return false;
		}
		bool readNoQuote()
		{
			if(index >= data.size()) return onNoMore();
			auto c = data[index++];
			if(c == Endl) return onEnd();
			if(c == Return) return onRt();
			if(c == Comma) return onSep();
			
			temp.push_back(c);
			return readNoQuote();
		}
		bool onNoMore()
		{
			row.push_back(temp);
			return true;
		}
		bool onRt()
		{
			if(index >= data.size()) return onNoMore();
			if(data[index] == Endl)
			{
				++index;
				return onEnd();
			}
			return onNoMore();
		}
		bool onEnd()
		{
			row.push_back(temp);
			return true;
		}
		bool onSep()
		{
			row.push_back(temp);
			temp.clear();
			return readCol();
		}
	};
	
	bool CSVLoader::decode(std::string content)
	{

		size_t index = 0;
		while (index < content.size()) {
			CSVContent::RowType row;
			_CSVLoaderHelp run(content, index, row);
			if(!run.readRow())
			{
				return false;
			}
			mTable.rows.push_back(row);
		}
		return true;
	}
}

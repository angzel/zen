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

#include "zen_db_query.h"

namespace Zen {
	std::shared_ptr<DBQuery> DBQuery::Create(std::shared_ptr<DBConnection> c)
	{
		return std::shared_ptr<DBQuery>(new DBQuery(c));
	}

	DBQuery::DBQuery(std::shared_ptr<DBConnection> c)
	{
		mMysql = c;
	}
	DBQuery::~DBQuery()
	{
	}

	void DBQuery::setQueryString(std::string const & s)
	{
		mQueryString = s;
	}
	bool DBQuery::execute()
	{
		return mysql_real_query(mMysql->getMYSQL(), mQueryString.data(), mQueryString.size()) == 0;
	}
	int DBQuery::getAffectedRowCount()
	{
		return (int)mysql_affected_rows(mMysql->getMYSQL());
	}
	
	bool DBQuery::fetchResult(DBResult & result)
	{
		result.meta_fields.clear();
		result.rows.clear();

		MYSQL_RES * r = mysql_store_result(mMysql->getMYSQL());
		if(r == nullptr) return false;

		size_t num_rows = mysql_num_rows(r);
		size_t num_cols = mysql_num_fields(r);

		auto fields = mysql_fetch_fields(r);

		result.meta_fields.resize(num_cols);
		for(size_t i = 0; i < num_cols; ++i)
		{
			auto & field = result.meta_fields[i];
			auto f = &fields[i];
			field.name = f->name;
			field.type = f->type;
			field.data_length = (int)f->length;
		}

		result.rows.resize(num_rows);

		for(auto & row : result.rows)
		{
			row.resize(num_cols);

			auto src = mysql_fetch_row(r);
			auto lengths = mysql_fetch_lengths(r);
			for(size_t i = 0; i < num_cols; ++i)
			{
				auto col = src[i];
				row[i].assign(col, col + lengths[i]);
			}
		}
		mysql_free_result(r);
		return true;
	}
	uint64_t DBQuery::getInsertID()
	{
		return mysql_insert_id(mMysql->getMYSQL());
	}
	std::string DBQuery::error()
	{
		return mMysql->error();
	}
	int DBQuery::eno()
	{
		return mMysql->eno();
	}
}

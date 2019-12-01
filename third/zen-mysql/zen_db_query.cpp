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

	DBQuery::DBQuery(std::shared_ptr<DBConnection> c)
	{
		mMysql = c;
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

		size_t num_row = mysql_num_rows(r);
		size_t num_fields = mysql_num_fields(r);

		result.meta_fields.resize(num_fields);
		for(size_t i = 0; i < num_fields; ++i)
		{
			auto & field = result.meta_fields[i];
			auto f = mysql_fetch_field(r);
			if(!f) return false;
			field.name = f->name;
			field.type = f->type;
			field.length = (int)f->length;
		}

		result.rows.resize(num_row);
		for(size_t i = 0; i < num_row; ++i)
		{
			auto & row = result.rows[i];
			auto data = mysql_fetch_row(r);
			auto lens = mysql_fetch_lengths(r);
			if(!data) return false;
			row.resize(num_fields);
			for(size_t j = 0; j < num_fields; ++j)
			{
				auto b = data[j];
				auto e = b + lens[j];
				row[j].assign(b, e);
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

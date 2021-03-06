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

#include "zen_db_statement.h"

namespace Zen {

	std::shared_ptr<DBStatement> DBStatement::Create(std::shared_ptr<DBConnection> c)
	{
		return std::shared_ptr<DBStatement>(new DBStatement(c));
	}

	DBStatement::DBStatement(std::shared_ptr<DBConnection> c)
	{
		mMysql = c;
		mStat = mysql_stmt_init(mMysql->getMYSQL());
	}
	
	DBStatement::~DBStatement()
	{
		mysql_stmt_close(mStat);
	}

	bool DBStatement::prepare(std::string const & s)
	{
		if(mysql_stmt_prepare(mStat, s.data(), s.size()) != 0) return false;
		mParamCount = mysql_stmt_param_count(mStat);
		mMYSQLParams.resize(mParamCount);
		mStoreParams.resize(mParamCount);
		return true;
	}
	size_t DBStatement::getParamCount()
	{
		return mParamCount;
	}
	
#define ZEN_BIND_VALUE(vtype, u, type) \
		auto & b = mMYSQLParams[index]; \
		::memset(&b, 0, sizeof(b)); \
		auto & p = mStoreParams[index]; \
		p.value.vtype = v; \
		b.buffer = &p.value.vtype; \
		b.buffer_type = type; \
		b.is_unsigned = u

	void DBStatement::bindParam(int index, int64_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(i64, 0, MYSQL_TYPE_LONGLONG);
	}
	void DBStatement::bindParam(int index, int32_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(i32, 0, MYSQL_TYPE_LONG);
	}
	void DBStatement::bindParam(int index, int8_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(i8, 0, MYSQL_TYPE_TINY);
	}
	void DBStatement::bindParam(int index, int16_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(i16, 0, MYSQL_TYPE_SHORT);
	}
	void DBStatement::bindParam(int index, uint64_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(u64, 1, MYSQL_TYPE_LONGLONG);
	}
	void DBStatement::bindParam(int index, uint32_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(u32, 1, MYSQL_TYPE_LONG);
	}
	void DBStatement::bindParam(int index, uint8_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(u8, 1, MYSQL_TYPE_TINY);
	}
	void DBStatement::bindParam(int index, uint16_t v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(u16, 1, MYSQL_TYPE_SHORT);
	}
	void DBStatement::bindParam(int index, float v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(f, 0, MYSQL_TYPE_FLOAT);
	}
	void DBStatement::bindParam(int index, double v) //g enum_field_types type)
	{
		ZEN_BIND_VALUE(d, 0, MYSQL_TYPE_DOUBLE);
	}
	void DBStatement::bindParam(int index, std::string const & s)
	{
		auto & b = mMYSQLParams[index];
		::memset(&b, 0, sizeof(b));
		auto & p = mStoreParams[index];
		p.buf.resize(s.size());
		::memcpy(p.buf.data(), s.data(), s.size());
		b.buffer = p.buf.data();
		b.buffer_length = s.size();
		b.buffer_type = MYSQL_TYPE_STRING;
	}

	void DBStatement::bindParam(int index, std::vector<char> const & buf, bool is_long)
	{
		auto & b = mMYSQLParams[index];
		::memset(&b, 0, sizeof(b));
		auto & p = mStoreParams[index];
		p.buf = buf;
		b.buffer = p.buf.data();
		b.buffer_length = buf.size();
		b.buffer_type = is_long?MYSQL_TYPE_LONG_BLOB:MYSQL_TYPE_BLOB;
	}

	bool DBStatement::execute()
	{
		if(mysql_stmt_bind_param(mStat, mMYSQLParams.data()))
		{
			return false;
		}
		return mysql_stmt_execute(mStat) == 0;
	}
	bool DBStatement::fetchResult(DBResult & result)
	{
		auto meta = mysql_stmt_result_metadata(mStat);
		if(!meta) return false;

		/* Get total columns in the query */
		auto num_cols = mysql_num_fields(meta);
		result.meta_fields.resize(num_cols);

		std::vector<unsigned long> lengths(num_cols);
		std::vector<char> buffer;
		std::vector<MYSQL_BIND> binds;

		binds.resize(num_cols);

		auto fields = mysql_fetch_fields(meta);

		for(size_t i = 0; i < num_cols; ++i)
		{
			auto & field = result.meta_fields[i];
			auto f = &fields[i];

			field.name = f->name;
			field.type = f->type;
			field.data_length = (int)f->length;

			auto & b = binds[i];
			::memset(&b, 0, sizeof(b));
			b.buffer_type = MYSQL_TYPE_BLOB;
			b.buffer= 0;
			b.buffer_length = 0;
			b.length= &lengths[i];
		}
		mysql_free_result(meta);

		if(mysql_stmt_bind_result(mStat, binds.data()) != 0) return false;

		if(mysql_stmt_store_result(mStat) != 0) return false;

		result.rows.resize(mysql_stmt_affected_rows(mStat));

		MYSQL_BIND column;
		::memset(&column, 0, sizeof(column));
		column.buffer_type = MYSQL_TYPE_BLOB;

		for(auto & row : result.rows)
		{
			row.resize(num_cols);
			
			mysql_stmt_fetch(mStat);

			for(size_t i = 0; i < num_cols; ++i)
			{
				auto length = lengths[i];
				row[i].resize(length);
				column.buffer = row[i].data();
				column.buffer_length = length;
				mysql_stmt_fetch_column(mStat, &column, (unsigned int)i, 0);
			}
		}
		mysql_stmt_free_result(mStat);

		return true;
	}
	
	uint64_t DBStatement::getInsertID()
	{
		return mysql_stmt_insert_id(mStat);
	}

	int DBStatement::getAffectedRowCount()
	{
		return (int)mysql_stmt_affected_rows(mStat);
	}
	std::string DBStatement::error()
	{
		return mysql_stmt_error(mStat);
	}
	int DBStatement::eno()
	{
		return mysql_stmt_errno(mStat);;
	}
}

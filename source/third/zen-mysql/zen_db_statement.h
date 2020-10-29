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

/**
 class Zen::DBStatement

 param types in c/mysql
 --
 c type         enum_field_types     mysql type
 signed char    MYSQL_TYPE_TINY      TINYINT
 short int      MYSQL_TYPE_SHORT     SMALLINT
 int            MYSQL_TYPE_LONG      INT
 long long int  MYSQL_TYPE_LONGLONG  BIGINT
 float          MYSQL_TYPE_FLOAT     FLOAT
 double         MYSQL_TYPE_DOUBLE    DOUBLE
 MYSQL_TIME     MYSQL_TYPE_TIME      TIME
 MYSQL_TIME     MYSQL_TYPE_DATE      DATE
 MYSQL_TIME     MYSQL_TYPE_DATETIME  DATETIME
 MYSQL_TIME     MYSQL_TYPE_TIMESTAMP TIMESTAMP
 char[]         MYSQL_TYPE_STRING    TEXT, CHAR, VARCHAR
 char[]         MYSQL_TYPE_BLOB      BLOB, BINARY, VARBINARY
 null           MYSQL_TYPE_NULL      NULL
 --
 */

#pragma once

#include "zen_db_connection.h"

namespace Zen {

	struct DBBindInfo
	{
		std::vector<char> buf;
		union {
			int64_t i64;
			uint64_t u64;
			int32_t i32;
			uint32_t u32;
			int8_t i8;
			uint8_t u8;
			int16_t i16;
			uint16_t u16;
			float f;
			double d;
		} value;
	};

	class DBStatement
	{
		DBStatement(std::shared_ptr<DBConnection>);
	public:
		static std::shared_ptr<DBStatement> Create(std::shared_ptr<DBConnection>);

		~DBStatement();

		bool prepare(std::string const & s);

		size_t getParamCount();


		void bindParam(int index, int64_t); // MYSQL_TYPE_LONGLONG
		void bindParam(int index, int32_t); // MYSQL_TYPE_LONG
		void bindParam(int index, int8_t); // MYSQL_TYPE_TINY
		void bindParam(int index, int16_t); // MYSQL_TYPE_SHORT

		void bindParam(int index, uint64_t); // MYSQL_TYPE_LONGLONG
		void bindParam(int index, uint32_t); // MYSQL_TYPE_LONG
		void bindParam(int index, uint8_t); // MYSQL_TYPE_TINY
		void bindParam(int index, uint16_t); // MYSQL_TYPE_SHORT

		void bindParam(int index, float); // MYSQL_TYPE_FLOAT
		void bindParam(int index, double); // MYSQL_TYPE_DOUBLE

		void bindParam(int index, std::string const &);
		void bindParam(int index, std::vector<char> const &, bool is_long = false);
		
		bool execute();

		bool fetchResult(DBResult & result);

		uint64_t getInsertID();
		
		int getAffectedRowCount();
		
		std::string error();

		int eno();
	protected:
		MYSQL_STMT * mStat;

		size_t mParamCount = 0;
		std::shared_ptr<DBConnection> mMysql;

		std::vector<DBBindInfo> mStoreParams;
		std::vector<MYSQL_BIND> mMYSQLParams;
	};
}

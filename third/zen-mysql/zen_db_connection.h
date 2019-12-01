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

#pragma once

#include "mysql.h" // mysql library header
#include <string>
#include <cstring>
#include <vector>
#include <memory>

namespace Zen {

	struct DBMetaItem {
		std::string name;
		enum_field_types type;
		int length;
	};
	
	typedef std::vector<std::vector<char> > DBRow;

	struct DBResult {
		std::vector<DBMetaItem> meta_fields;
		std::vector<DBRow> rows;
	};

	class DBConnection
	{
	public:
		enum class State {
			None, Init, Connected, Retry,
		};
	protected:
		MYSQL * mMYSQL = nullptr;

		std::string mHost;
		std::string mUser;
		std::string mPasswd;
		std::string mDb;
		unsigned int mPort;
		std::string mUnixSocket;
		unsigned long mClientflag;

		State mState = State::None;
		int mPreviousReturn = 0;

		DBConnection();
	public:
		static std::shared_ptr<DBConnection> Create();

		~DBConnection();

		void init(std::string host, std::string user, std::string passwd, std::string db,
				  unsigned int port, std::string mUnixSocket = "", unsigned long flag = 0);

		bool setOption(enum mysql_option op, void * value);
		
		bool setTimeoutConnect(int seconds);
		bool setTimeoutRead(int seconds);
		bool setTimeoutWrite(int seconds);
		bool enableReconnect(bool retry);

		bool connect();

		void close();

		bool changeUser(std::string user, std::string passwd, std::string db);

		bool selectDatabase(std::string db_name);

		std::string error();

		unsigned int eno();

		State getState() const;

		std::string getSqlState();

		MYSQL * getMYSQL();
	};
}

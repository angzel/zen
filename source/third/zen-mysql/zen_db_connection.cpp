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

#include "zen_db_connection.h"

namespace Zen {
	
	std::shared_ptr<DBConnection> DBConnection::Create()
	{
		return std::shared_ptr<DBConnection>(new DBConnection);
	}
	
	DBConnection::DBConnection()
	{
		mMYSQL = new MYSQL;
		mysql_init(mMYSQL);
	}
	DBConnection::~DBConnection()
	{
		this->close();
	}
	void DBConnection::close()
	{
		if(mMYSQL)
		{
			mysql_close(mMYSQL);
			delete mMYSQL;
			mMYSQL = nullptr;
		}
	}

	void DBConnection::init(std::string host, std::string user, std::string passwd, std::string db,
							  unsigned int port, std::string unix_socket, unsigned long flag)
	{
		mHost = host;
		mUser = user;
		mPasswd = passwd;
		mDb = db;
		mPort = port;
		mUnixSocket = unix_socket;
		mClientflag = flag;
	}

	bool DBConnection::setTimeoutConnect(int seconds)
	{
		return this->setOption(mysql_option::MYSQL_OPT_CONNECT_TIMEOUT, (void*)&seconds);
	}
	bool DBConnection::setTimeoutRead(int seconds)
	{
		return this->setOption(mysql_option::MYSQL_OPT_READ_TIMEOUT, (void*)&seconds);
	}
	bool DBConnection::setTimeoutWrite(int seconds)
	{
		return this->setOption(mysql_option::MYSQL_OPT_WRITE_TIMEOUT, (void*)&seconds);
	}

	bool DBConnection::enableReconnect(bool retry)
	{
		bool v = retry?1:0;
		return (mysql_options(mMYSQL, MYSQL_OPT_RECONNECT, (void*)&v) == 0);
	}

	bool DBConnection::setOption(enum mysql_option op, void * arg)
	{
		return (mysql_options(mMYSQL, op, arg) == 0);
	}

	bool DBConnection::connect()
	{
		auto res = mysql_real_connect(mMYSQL, mHost.data(), mUser.data(), mPasswd.data(), mDb.data(), mPort, mUnixSocket.data(), mClientflag);
		if(res == mMYSQL)
		{
			mState = State::Connected;
			return true;
		}
		return false;
	}

	bool DBConnection::changeUser(std::string user, std::string passwd, std::string db)
	{
		bool res = true;
		if(mState == State::Connected)
		{
			res = (mysql_change_user(mMYSQL, user.data(),
									 passwd.data(),
									 db.empty()?nullptr:db.data()) != 0);
		}
		if(res)
		{
			mUser = user;
			mPasswd = passwd;
			if(db.size()) mDb = db;
		}
		return res;
	}

	bool DBConnection::selectDatabase(std::string db)
	{
		bool res = true;
		if(mState == State::Connected)
		{
			mysql_select_db(mMYSQL, db.data());
		}
		if(res)
		{
			mDb = db;
		}
		return res;
	}

	std::string DBConnection::error()
	{
		return mysql_error(mMYSQL);
	}

	unsigned int DBConnection::eno()
	{
		return mysql_errno(mMYSQL);
	}

	DBConnection::State DBConnection::getState() const
	{
		return mState;
	}

	std::string DBConnection::getSqlState()
	{
		return mysql_sqlstate(mMYSQL);
	}
	MYSQL * DBConnection::getMYSQL()
	{
		return mMYSQL;
	}
}

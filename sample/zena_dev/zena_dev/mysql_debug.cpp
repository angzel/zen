	//
	//  main.cpp
	//  zena_dev
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_mysql.h"
#include "zen_cast.h"

using namespace std;
int main(int argc, const char * argv[]) {
	cout << MYSQL_SERVER_VERSION << endl;
	cout << PROTOCOL_VERSION << endl;
	
	auto con = Zen::DBConnection::Create();
	con->init("127.0.0.1", "thelover", "meher", "test", 3306);
	con->enableReconnect(true);
	if(con->connect())
	{
		cout << "connect ok\n";
	}
	else
	{
		cout << con->error() << "\n";
	}
	auto state = Zen::DBStatement::Create(con);
	state->prepare("select * from `logs`");
	MYSQL_TIME time;
	::memset(&time, 0, sizeof(time));
	time.year = 2019;
	time.month = 12;
	time.day = 1;
	time.time_type = MYSQL_TIMESTAMP_DATETIME;
	auto count = state->getParamCount();
	cout << "param:" << count << endl;
		//	state.bindParam(0, "2019-12-3 12:0:0");
		//	state.bindParam(1, "hello");
	cout << "exe:" << state->execute() << endl;
	Zen::DBResult result;
	cout << "fetch:" << state->fetchResult(result) << endl;
	
	cout << "meta:" << result.meta_fields.size() << endl;
	for(auto i : result.meta_fields)
	{
		cout << "meta name:" << i.name << endl;
	}
	cout << "row size:" << result.rows.size() << endl;
	for(auto i : result.rows)
	{
		cout << i.size() << endl;
		
		for(auto e : i)
		{
			cout << "s-:" << Zen::ToString(e) << endl;
		}
	}
	
	auto query = Zen::DBQuery::Create(con);
	query->setQueryString("select * from `logs`");
	query->execute();
	query->fetchResult(result);
	
	cout << result.meta_fields.size() << endl;
	cout << result.rows.size() << endl;
	
	return 0;
}

	//
	//  main.cpp
	//  zena_dev
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_net.h"

class MyO {
} o;

using namespace std;

template<typename Type>
inline std::ostream & operator << (MyO const & o, Type const & v)
{
	cout << v << endl;
	return cout;
}

void listen()
{
	Zen::IPInfo ip;
	ip.setHostAddress("localhost");
	auto lis = Zen::SocketListener::Create();
	o << "open:" << lis->open();
	o << "bind:" << lis->bind(ip, 2200);
	lis->setReuseable(true);
	auto sock = lis->accept();
	char buf[1];
	while(sock->recv(buf, 1))
	{
		cout << buf;
		cout.flush();
	}
}
int main(int argc, const char * argv[]) {
	if(argc >= 2 && std::string(argv[1]) == "server")
	{
		listen();
		return 0;
	}

	auto con = Zen::SocketConnector::Create();
	Zen::IPInfo ip;
	ip.setHostAddress("www.baidu.com");
	o << con->connect(ip, 80);
	std::string header = "GET / HTTP/1.1\n\n";
	o << con->send(header.data(), header.size());
	o << con->couldReadBytes();
	char buf[1024];
	while(true)
	{
		auto rs = con->recv(buf, 1024);
		o << rs;
		if(rs)
		{
			buf[rs] = 0;
			o << buf;
		}
		else
		{
			break;
		}
	}
	return 0;
}

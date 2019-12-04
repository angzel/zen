	//
	//  main.cpp
	//  zena_dev
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "zen_net.h"
#include "zen_atom_queue.h"

class MyO {
} o;

using namespace std;

template<typename Type>
inline MyO const & operator << (MyO const & o, Type const & v)
{
	cout << v << endl;
	return o;
}

int main(int argc, const char * argv[]) {

	auto con = Zen::SocketConnector::Create();

	auto ip = Zen::SocketAddressMake("127.0.0.1", 2200, Zen::SocketFamily::inet);
	o << Zen::GetAddressString(ip);
	o << con->connect(ip);
	std::string header = "GET / HTTP/1.1\n\n";
	o << con->send(header.data(), header.size());
	o << con->couldReadBytes();
	std::string s;
	while(cin >> s)
	{
		con->send(s.data(), s.size());
	}
	o << con->isConnected();
	return 0;
}

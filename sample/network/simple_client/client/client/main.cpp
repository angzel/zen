//
//  main.cpp
//  client
//
//  Created by MeherTj on 2020/10/31.
//

#include <iostream>
#include "zen_net.h"
#include "zen_socket_config.h"

using namespace std;

int main(int argc, const char * argv[]) {
	auto con = Zen::SocketConnector::Create();
	auto ip = Zen::SocketAddressMake("www.baidu.com", 80, Zen::SocketFamily::inet);
	cout << "connect: " << con->connect(ip) << endl;
	std::string msg = "GET / HTTP/1.1\n"
	"Accept: */*\n"
	"Accept-Language: zh-cn\n\n";
	con->send(msg.data(), msg.size());
	static const size_t MAX_LEN = 32;
	char buf[MAX_LEN];
	do {
		auto n = con->recv(buf, MAX_LEN-1);
		buf[n] = 0;
		cout << buf << endl;
		if(n == 0) break;
	} while(1);

	return 0;
}

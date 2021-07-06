#include "header.h"

bool go(std::shared_ptr<Link> link)
{
	cout << ">:";
	cout.flush();
	cin.getline(buf, SIZE - 1);
	cout << buf << endl;
	
	auto size = strlen(buf);
	
	std::string str = buf;
	if(str == "close")
	{
		link->close();
		return false;
	}
	if(size)
	{
		auto ss = link->send(buf, size, MsgType::Default);
		if(ss != size)
		{
			error("send error");
			return false;
		}
		while(1)
		{
			auto s = link->recv(buf, SIZE);
			if(s > 0) {
				std::string data(buf, s);
				cout << "recv:" << data << endl;
			}
			if(s == SIZE) continue;
			break;
		}
	}
	return link->getStatus();
}

int main()
{
	auto con = TCPConnector::Create();
	while(true)
	{
		cout << "new connection..." << endl;
		
		auto link = con->connect(a);
		if(!link)
		{
			cout << "connect error" << endl;
			break;
		}
		
		link->setReuseable(true);
		link->setNonBlock(true);
		
		cout << "peer:" << AddressToString(link->makePeerAddress()) << endl;
		cout << "local:" << AddressToString(link->makeLocalAddress()) << endl;

		while(go(link));
	}
	
	return 0;
}

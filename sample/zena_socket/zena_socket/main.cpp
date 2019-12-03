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

struct Node {
	std::shared_ptr<Zen::Socket> socket;
};

std::vector<Node*> cons;
std::shared_ptr<Zen::Poll> poll;
Zen::AtomQueue<Node*> q;
Zen::AtomQueue<std::string> option_q;

void error_log(std::string s)
{
	cerr << s << endl;
}

void _poll_call(Zen::Poll::Event event, void * user_data, int extra)
{
	cout << "event:" << event << " ex:" << extra << endl;
	auto node = (Node*)user_data;
	if(Zen::Poll::Event::None == event)
	{

	}
	else if(Zen::Poll::Event::Write == event)
	{
		cout << "send event" << endl;
	}
	else if(Zen::Poll::Event::Read == event)
	{
		auto size = node->socket->couldReadBytes();
		std::vector<char> buf;
		buf.resize(size+1);

		auto sz = node->socket->recv(buf.data(), size);
		if(sz != size)
		{
			error_log("recv error ");
		}
		buf[size+1] = 0;
		cout << "r>>>" << buf.data() << endl;
	}
	if(!node->socket->isConnected())
	{
		poll->watch(node->socket->handle(), Zen::Poll::None, 0);
		error_log("close");
		node->socket->close();
		for(auto iter = cons.begin(); iter != cons.end(); ++iter)
		{
			if(*iter == node)
			{
				cons.erase(iter);
				break;
			}
		}
		delete node;
	}
}

void poll_run()
{
	while(1)
	{
		auto vs = q.pop_all();
		for(auto i : vs)
		{
			if(!poll->watch(i->socket->handle(), Zen::Poll::Event::Read, i))
			{
				i->socket->close();
				delete i;
				error_log("poll watch error!");
			}
			else
			{
				cons.push_back(i);
			}
			cout << "connect count:" << cons.size() << endl;
		}
		poll->waitEvent(100, _poll_call);
		sleep(1);
		if(option_q.size())
		{
			auto oq = option_q.pop_all();
			for(auto op : oq)
			{
				if(op == "closeall")
				{
					for(auto c : cons)
					{
						poll->watch(c->socket->handle(), Zen::Poll::Event::None, 0);
						delete c;
					}
					cons.clear();
				}
			}
		}
	}
}

void push_poll(std::shared_ptr<Zen::Socket> socket)
{
	auto node = new Node;
	node->socket = socket;
	q.push(node);
}
void listen_run()
{
	Zen::IPInfo ip;
	ip.setHostAddress("127.0.0.1");
	auto lis = Zen::SocketListener::Create();
	o << "open:" << lis->open();
	lis->setReuseable(true);
	lis->setNonBlock(false);
	o << "bind:" << lis->bind(ip, 2200);
	lis->listen(10);
	while(1)
	{
		auto sock = lis->accept();
		if(!sock) continue;
		push_poll(sock);
	}
}
void server()
{
	poll = Zen::Poll::Create();
	if(!poll)
	{
		o << "poll error";
		return;
	}
	q.setFullSize(12);
	option_q.setFullSize(12);
	new std::thread(poll_run);
	new std::thread(listen_run);
}

int main(int argc, const char * argv[])
{
	server();
	std::string s;
	while(cin >> s)
	{
		option_q.push(s);
		if(s == "c0")
		{
			cons[0]->socket->close();
		}
	}
	return 0;
}

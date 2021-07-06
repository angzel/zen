#include "header.h"
#include <map>
#include <thread>
#include "zen_poll.h"
#include "zen_cast.h"
#include "zen_atom.h"

Zen::AtomMap<int, std::shared_ptr<Link> > g_links;

std::shared_ptr<Poll> g_poll = nullptr;

void poll_link(Link * link)
{
	g_poll->add(link->handle(), link);
}
void close_link(Link * link)
{
	cout << link << ":" << link->handle() << endl;
	auto key = link->handle();

	link->close();
	g_links.erase(key); // after erase, link will be free.
}

int main(int argc, const char * argv[]) {
	auto lis = TCPListener::Create();
	
	g_poll = Poll::Create();
	
	/// simple run..
	new thread([]() {
		while(1)
			g_poll->wait([]( void * ud, int e) {
				auto link = (Link*)ud;
				
				size_t size = 0;
				if(!e && (size = link->canRecvSize()))
				{
					std::vector<char> buf(size);
					if(link->recv(buf.data(), buf.size()) != size)
					{
						cout << "close on error...." << endl;
						close_link(link);
						return;
					}
					else
					{
						cout << "size:" << size << endl;
						cout << Zen::ToString(buf) << endl;
						link->send(buf.data(), size);
					}
				}
				else
				{
					cout << "close ...." << endl;
					close_link(link);
				}
			}, -1);
	});
	
	if(!lis->bind(a))
	{
		return -2;
	}
	cout << "listen address: ";
	cout << AddressToString(a) << endl;
	
	lis->listen(0);
	lis->setNonBlock(true);
	lis->setReuseable(true);
	
	auto local = lis->getLink()->makeLocalAddress();
	cout << AddressToString(local) << endl;
	while(1)
	{
		usleep(1000);
		cout << "acc..." << endl;
		auto link = lis->accept();
		if(!link || !link->isValidHandle())
		{
			cout << "time out(test for NonBlock)" << endl;
			lis->setNonBlock(false);
			continue;
		}
		
		g_links.insert((int)link->handle(), link);
		
		cout << "new:" << link->handle() << endl;
		cout << "peer:" << AddressToString(link->makePeerAddress()) << endl;
		cout << "local:" << AddressToString(link->makeLocalAddress()) << endl;
		
		poll_link(link.get());
	}
	
	return 0;
}

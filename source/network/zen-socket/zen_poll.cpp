#include "zen_poll.h"

#include "zen_log.h"
#include "zen_os.h"
#include <unistd.h>
#include <string>
#include <functional>

#if defined(ZEN_OS_MAC)
#include <sys/event.h>
#include <errno.h>

namespace Zen {
	
#define KQ_EVENT_SIZE 32
	class __Kqueue : public Poll
	{
		int mKQ = -1;
		struct kevent mEvents[KQ_EVENT_SIZE];
	public:
		__Kqueue()
		{
			mKQ = kqueue();
		}
		~__Kqueue()
		{
			this->close();
		}
		bool isValid() const
		{
			return mKQ != -1;
		}
		void close()
		{
			if(mKQ == -1) return;
			::close(mKQ);
			mKQ = -1;
		}
		virtual int waitEvent(int ms_timeout, Callback callback) override
		{
			int res = 0;
			if(ms_timeout >= 0)
			{
				::timespec timeout;
				timeout.tv_sec = ms_timeout / 1000;
				timeout.tv_nsec = (ms_timeout % 1000) * 1E6;
				res = kevent(mKQ, NULL, 0, mEvents, KQ_EVENT_SIZE, &timeout);
			}
			else
			{
				res = kevent(mKQ, NULL, 0, mEvents, KQ_EVENT_SIZE, nullptr);
			}
			
			if (res == -1) return -1;
			
			if(callback == nullptr) return res;
			
			for(int i = 0; i < res; ++i)
			{
				auto &event = mEvents[i];
				
				auto filter = event.filter;
				auto data = event.data;
				auto user_data = (void*)event.udata;

				if(filter == EVFILT_READ)
				{
					callback(Event::Read, user_data, (int)data);
				}
				else if(filter == EVFILT_WRITE)
				{
					callback(Event::Write, user_data, (int)data);
				}
				else
				{
					callback(Event::None, user_data, (int)filter);
				}
			}
			
			return res;
		}
		virtual bool watch(SocketHandle native, Event event, void * user_data) override
		{
			struct kevent changes[1];
			if(event & Event::Read)
			{
				EV_SET(&changes[0], native, EVFILT_READ, EV_ADD, 0, 0, user_data);
				if(kevent(mKQ, changes, 1, nullptr, 0, nullptr) != 0) return false;
			}
			else
			{
				EV_SET(&changes[0], native, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				kevent(mKQ, changes, 1, nullptr, 0, nullptr);
			}
			if(event & Event::Write)
			{
				EV_SET(&changes[0], native, EVFILT_WRITE, EV_ADD, 0, 0, user_data);
				if(kevent(mKQ, changes, 1, nullptr, 0, nullptr) != 0) return false;
			}
			else
			{
				EV_SET(&changes[0], native, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
				kevent(mKQ, changes, 1, nullptr, 0, nullptr);
			}
			return true;
		}
	};
	
	std::shared_ptr<Poll> Poll::Create()
	{
		auto kq = new __Kqueue();
		if(kq->isValid()) return std::shared_ptr<Poll>(kq);
		delete kq;
		return nullptr;
	}
}

#elif defined(ZEN_OS_LINUX)

#include <sys/epoll.h>

namespace Zen {
	
#define EP_EVENT_SIZE 32

#define EPOLL_INOUT (EPOLLIN | EPOLLOUT)
	
	class __EPoll : public Poll
	{
		int mEP = -1;
		struct epoll_event mEvents[EP_EVENT_SIZE];
	public:
		__EPoll()
		{
			mEP = epoll_create(32);
		}
		bool isValid() const
		{
			return mEP != -1;
		}
		~__EPoll()
		{
			this->close();
		}
		void close()
		{
			if(mEP == -1) return;
			::close(mEP);
			mEP = -1;
		}
		virtual int waitEvent(int ms_timeout, Callback callback) override
		{
			int res = epoll_wait(mEP, mEvents, EP_EVENT_SIZE, ms_timeout);

			if (res == -1) return -1;
			
			if(callback == nullptr) return res;
			
			for(int i = 0; i < res; ++i)
			{
				auto & event = mEvents[i];

				auto user_data = event.data.ptr;
				auto filter = event.events;

				if(filter & EPOLLERR || filter & EPOLLHUP)
				{
					callback(Event::None, user_data, (int)filter);
					continue;
				}
				if(filter & EPOLLIN)
				{
					callback(Event::Read, user_data, 0);
				}
				if(filter & EPOLLOUT)
				{
					callback(Event::Write, user_data, 0);
				}
				if(filter & EPOLLPRI)
				{
				}
			}
			
			return res;
		}
		virtual bool watch(SocketHandle native, Event event, void * user_data) override
		{
			if(event == Event::None)
			{
				epoll_ctl(mEP, EPOLL_CTL_DEL, native, nullptr);
				return true;
			}
			
			struct epoll_event data;
			data.data.ptr = user_data;
			
			if(event == Event::Read)
			{
				data.events = EPOLLIN; // | EPOLLONESHOT;
			}
			else if(event == Event::Write)
			{
				data.events = EPOLLOUT; // | EPOLLONESHOT;
			}
			else if(event == Event::Both)
			{
				data.events = EPOLLOUT | EPOLLIN;
			}

			if(epoll_ctl(mEP, EPOLL_CTL_MOD, native, &data) == -1)
			{
				if(epoll_ctl(mEP, EPOLL_CTL_ADD, native, &data) == -1) return false;
			}
			return true;
		}
	};
	
	std::shared_ptr<Poll> Poll::Create()
	{
		auto p = new __EPoll();
		if(p->isValid()) return std::shared_ptr<Poll>(p);
		delete p;
		return nullptr;
	}
}
#endif

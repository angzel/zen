#include "zen_poll.h"

#include "zen_log.h"
#include "zen_macro.h"
#include <unistd.h>
#include <string>
#include <functional>

#if ZEN_OS_APPLE
#include <sys/event.h>
#include <errno.h>

namespace Zen { namespace Socket {
	
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
		virtual int wait(Callback callback, int timeout) override
		{
			int res = 0;
			if(timeout >= 0)
			{
				::timespec ts;
				ts.tv_sec = timeout / 1000;
				ts.tv_nsec = (timeout % 1000) * 1E6;
				res = kevent(mKQ, NULL, 0, mEvents, KQ_EVENT_SIZE, &ts);
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
				
				auto data = (void*)event.udata;

				if(filter == EVFILT_READ)
				{
					callback(data, 0);
				}
				else
				{
					callback(data, 1);
				}
			}
			
			return res;
		}
		virtual bool add(Handle native, void * data) override
		{
			struct kevent changes[1];
			EV_SET(&changes[0], native, EVFILT_READ, EV_ADD, 0, 0, data);
			return (kevent(mKQ, changes, 1, nullptr, 0, nullptr) == 0);
		}
		virtual void remove(Handle native) override
		{
			struct kevent changes[1];
			EV_SET(&changes[0], native, EVFILT_READ, EV_DELETE, 0, 0, NULL);
			kevent(mKQ, changes, 1, nullptr, 0, nullptr);
		}
	};
	
	std::shared_ptr<Poll> Poll::Create()
	{
		auto kq = new __Kqueue();
		if(kq->isValid()) return std::shared_ptr<Poll>(kq);
		delete kq;
		return nullptr;
	}
}}

#else

#include <sys/epoll.h>

namespace Zen { namespace Socket {
	
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
		virtual int wait(Callback callback, int timeout /* ms */) override
		{
			int res = epoll_wait(mEP, mEvents, EP_EVENT_SIZE, timeout);

			if (res == -1) return -1;
			
			if(callback == nullptr) return res;
			
			for(int i = 0; i < res; ++i)
			{
				printf("wait event...\n");
				auto & event = mEvents[i];

				auto data = event.data.ptr;
				auto filter = event.events;
				
				if(filter & EPOLLIN)
				{
					callback(data, 0);
				}
				else
				{
					callback(data, 1);
				}
			}
			
			return res;
		}
		virtual void remove(Handle native)
		{
			epoll_ctl(mEP, EPOLL_CTL_DEL, native, nullptr);
		}
		virtual bool add(Handle native, void * data) override
		{
			struct epoll_event epe;
			epe.data.ptr = data;
			
			epe.events = EPOLLIN; // | EPOLLONESHOT;

			if(epoll_ctl(mEP, EPOLL_CTL_MOD, native, &epe) == -1)
			{
				if(epoll_ctl(mEP, EPOLL_CTL_ADD, native, &epe) == -1) return false;
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
}}
#endif

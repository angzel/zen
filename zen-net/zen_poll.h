#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "zen_ip.h"

namespace Zen {
	class Poll
	{
	public:
		enum Event
		{
			None = 0,
			Read = 1<<0,
			Write = 1<<1,
			Both = (Read | Write),
		};

		static std::shared_ptr<Poll> Create();

		/**
		@event
			case Read: Read event ok, if no data to read, the connect break.
				extra
			case None: <extra> is error
		 */
		typedef std::function<void (Event event, void * user_data, int extra)> Callback;

		/**
		 @param ms_timeout
		 0:	return right now
		 -1:	wait utill event raise
		 >0: time ms
		 @return
		 0: 	timeout
		 -1:	error
		 >0: event count
		 */
		virtual int waitEvent(int ms_timeout, Callback) = 0;

		virtual bool watch(SocketHandle socket, Event event, void * user_data) = 0;

		virtual ~Poll() {}
	};
}

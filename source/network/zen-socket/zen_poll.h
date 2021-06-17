#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "zen_socket.h"

namespace Zen { namespace Socket {
	class Poll
	{
	public:
		static std::shared_ptr<Poll> Create();

		/**
		@event
			case Read: Read event ok, if no data to read, the connect break.
		 */
		typedef std::function<void (void * data, int e)> Callback;

		/**
		 @param timeout
		 =0:	return right now
		 <0:	wait utill event raise
		 >0: time milliseconds
		 @return
		 0: 	timeout
		 -1:	error
		 >0: event count
		 */
		virtual int wait(Callback, int timeout /* ms */) = 0;

		virtual bool add(Handle socket, void * data) = 0;
		
		virtual void remove(Handle socket) = 0;

		virtual ~Poll() {}
	};
}}

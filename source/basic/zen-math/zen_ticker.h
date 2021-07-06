/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include <ctime>
#include <chrono>

namespace Zen
{
	typedef std::chrono::microseconds Microseconds;

	template<typename TTimeDuration>
	double ToSeconds(TTimeDuration mic)
	{
		return (double)mic.count() / (double)TTimeDuration::period::den;
	}

	inline Microseconds ToMicroseconds(double seconds)
	{
		std::chrono::microseconds mc((long long)(seconds * std::chrono::microseconds::period::den));
		return mc;
	}

	inline static std::chrono::microseconds Now()
	{
		//std::micro
		using namespace std::chrono;
		return duration_cast<microseconds>(system_clock::now().time_since_epoch());
	}
	class Ticker {
	public:
		inline Ticker()
		{
			mStartTime = Now();
		}
		std::chrono::microseconds duration()
		{
			return Now() - mStartTime;
		}
		std::chrono::microseconds tick()
		{
			auto now = Now();
			auto interval = now - mStartTime;
			mStartTime = now;
			return interval;
		}
		void delay(std::chrono::microseconds move)
		{
			mStartTime -= move;
		}

	protected:
		std::chrono::microseconds mStartTime;
	};
}

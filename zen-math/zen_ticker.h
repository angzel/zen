/*
 Copyright (c) 2013 ClearSky G.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <ctime>
#include <chrono>

namespace Zen
{
	template<typename TTimeDuration>
	double ToSeconds(TTimeDuration mic)
	{
		return (double)mic.count() / (double)TTimeDuration::period::den;
	}

	inline static std::chrono::microseconds Now()
	{
		//std::micro
		using namespace std::chrono;
		return duration_cast<microseconds>(system_clock::now().time_since_epoch());
	}
		
	class Ticker
	{
	public:
		typedef std::chrono::microseconds Microsecond;
	public:
		inline Ticker(bool start = false);
		
		inline bool actived() const;
		
		inline void restart();
		
		inline void resume();
		
		inline void pause();
		
		inline Microsecond getRunningDuration() const;
		
		inline Microsecond getTimeDurationSinceResume() const;
		

	private:
		Microsecond mPastDuration;
		
		Microsecond mStartTime;
	};
}

namespace Zen {
	inline Ticker::Ticker(bool start)
	{
		mStartTime = Microsecond::zero();
		if(start) restart();
	}
	inline bool Ticker::actived() const
	{
		return mStartTime.count() != 0;
	}
	inline void Ticker::restart()
	{
		mPastDuration = Microsecond::zero();
		mStartTime = Now();
	}
	inline void Ticker::resume()
	{
		if(actived()) return;

		mStartTime = Now();
	}
	inline void Ticker::pause()
	{
		if(!actived()) return;

		mPastDuration += Now() - mStartTime;
		mStartTime = Microsecond::zero();
	}
	/* @ GetTime
	 */
	inline std::chrono::microseconds Ticker::getRunningDuration() const
	{
		if(actived()) return mPastDuration + (Now() - mStartTime);
		else return mPastDuration;
	}
	inline std::chrono::microseconds Ticker::getTimeDurationSinceResume() const
	{
		if(actived()) return Now() - mStartTime;
		else return Microsecond::zero();
	}

}

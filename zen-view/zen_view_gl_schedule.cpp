
#include "zen_view_gl_schedule.h"
#include "zen_gl.h"

namespace Zen { namespace Vap2d {

	GLSchedule::GLSchedule()
	{

	}

	void GLSchedule::setCallback(Callback callback)
	{
		this->callback = callback;
	}

	Schedule::Callback GLSchedule::getCallback()
	{
		return callback;
	}

	void GLSchedule::setRepeat(int repeat)
	{
		this->repeat = repeat;
	}

	int GLSchedule::getRepeat()
	{
		return repeat;
	}

	void GLSchedule::setInterval(float interval)
	{
		this->interval = interval;
	}

	float GLSchedule::getInterval()
	{
		return interval;
	}

	Node * GLSchedule::getEntity()
	{
		return entity;
	}

	void GLSchedule::remove()
	{
		repeat = 0;
	}
	void GLSchedule::run()
	{
		if(repeat > 0) --repeat;
		callback(interval, this);
	}

	std::pair<GLSchedule*, std::shared_ptr<Schedule> > GLScheduleManager::createSchedule()
	{
		auto s = new GLSchedule;
		auto sp = mCache[s] = std::shared_ptr<Schedule>(s);
		return std::make_pair(s, sp);
	}
	void GLScheduleManager::removeSchedule(GLSchedule * s)
	{
		mCache.erase(s);
	}

	std::deque<GLSchedule*> const & GLScheduleManager::getAllSchedules()
	{
		return mQue;
	}

	GLScheduleManager * GLScheduleManager::GetDefault()
	{
		static GLScheduleManager * me = new GLScheduleManager;
		return me;
	}

	void GLScheduleManager::push(GLSchedule * schedule)
	{
		for(auto iter = mQue.begin(); iter != mQue.end(); ++iter)
		{
			auto cur = *iter;
			if(schedule->time < cur->time)
			{
				mQue.insert(iter, schedule);
				return;
			}
		}
		mQue.insert(mQue.end(), schedule);
	}

	GLSchedule* GLScheduleManager::top()
	{
		if(mQue.empty()) return nullptr;
		return mQue.front();
	}

	void GLScheduleManager::pop()
	{
		mQue.pop_front();
	}
}}

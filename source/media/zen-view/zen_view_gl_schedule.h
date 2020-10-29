#pragma once

#include "zen_view_schedule.h"
#include <deque>
#include <map>
#include "zen_log.h"

namespace Zen { namespace Vap2d {
	class GLSchedule : public Schedule
	{
	public:
		GLSchedule();

		virtual void setCallback(Callback callback) override;

		virtual Callback getCallback() override;

		virtual void setRepeat(int repeat) override;

		virtual int getRepeat() override;

		virtual void setInterval(float interval) override;

		virtual float getInterval() override;

		virtual Node * getEntity() override;

		virtual void remove() override;

		void run();

		~GLSchedule()
		{
			Zen::LogW("~GLSchedule");
		}
	public:
		Node * entity = nullptr;

		Callback callback;

		int repeat = 1;

		float interval;

		double time;

		float delay = 0;
	};

	class GLScheduleManager : public virtual Object
	{
	public:
		static GLScheduleManager * GetDefault();

		std::pair<GLSchedule*, std::shared_ptr<Schedule> > createSchedule();

		void removeSchedule(GLSchedule *);

		void removeAllSchedule();

		void push(GLSchedule * schedule);

		GLSchedule* top();

		/**
		 * catch the GLSchedule* by <top> before <pop>, and <removeSchedule> or <push> again after <pop>
		 */
		void pop();

		std::deque<GLSchedule*> const & getAllSchedules();
	protected:
		std::deque<GLSchedule*> mQue;
		std::map<GLSchedule*, std::shared_ptr<Schedule> > mCache;
	};
}}

#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include <vector>
#include <mutex>
#include <functional>

namespace Zen { namespace Vap {
	
	class Action : public VapObject {
	protected:
		float m_delay = 0;
		float m_interval = 0;
	public:
		Action(float delay = 0.f, float interval = 0.f, std::string const & name = "action");
		/**
		 @function run
		 return delay > 0, if ==0 run in next frame.
		 -1 stop action
		 */
		virtual float start() { return m_delay; }

		virtual float run(float) { return -1; }

		virtual void reset() {}

		void setInterval(float interval);
		
		float getInterval() { return m_interval; }
	};
	
	class ActionDuration : public Action {
	protected:
		float m_running_duration = 0;
		float m_age = 0;
	public:
		ActionDuration(float delay = 0.f, float interval = 0.f, std::string const & name = "action on time");

		void initDuration(float duration);
		/**
		 @function run
		 return delay > 0, if ==0 run in next frame.
		 -1 stop action
		 */
		virtual float run(float) override;

		virtual void reset() override;
		
		virtual void runDuration() {}
		
		float getDuration() { return m_running_duration; }
		
		float getAge() { return m_age; }
	};

	class ActionCall : public Action
	{
	protected:
		std::function<bool(float)> m_call;
		int m_run_times = 0;
		int m_times = 0;
	public:
		static std::shared_ptr<Action> Create(std::function<bool(float)>, int times, float delay = 0.f, float interval = 0.f);
	public:
		ActionCall(float delay = 0.f, float interval = 0.f, std::string const & name = "action call");

		/**
		 @param call :action function return true if goon, false end.
		 @param times : 0 forever, +N repeat n times.
		 */
		void initCall(std::function<bool(float)> call, int times);

		virtual float run(float) override;

		virtual void reset() override;
	};
	
	class ActionRepeat : public Action
	{
	protected:
		std::shared_ptr<Action> m_action = nullptr; // inner pointer.
		int m_run_times = 0;
		int m_times = 0;
		bool m_is_inner_run = false;

	public:
		static std::shared_ptr<Action> Create(std::shared_ptr<Action>, int times, float delay = 0.f, float interval = 0.f);

	public:
		/**
		 time durations:
		 delay, (action.delay, action.interval ... ) interval (action.delay, action.interval ...) ...
		 */
		ActionRepeat(float delay = 0.f, float interval = 0.f, std::string const & name = "action repeat");

		/**
		 @ActionRepeat::init
		 @action
		 - repeated target action.
		 - pass as inner pointer.
		 @times
		 - repeat times.
		 if times == 0 repeat forever.
		 */
		void init(std::shared_ptr<Action>, int times);

		virtual float run(float) override;

		virtual void reset() override;
	};

	class ActionSequence : public Action
	{
	protected:
		std::vector<std::shared_ptr<Action> > m_actions;
		int m_index = 0;
		bool m_is_inner_run = false;

	public:
		static std::shared_ptr<Action> Create(std::vector<std::shared_ptr<Action> >, float delay = 0.f, float interval = 0.f);

	public:
		
		ActionSequence(float delay = 0.f, float interval = 0.f, std::string const & name = "action requence");

		void init(std::vector<std::shared_ptr<Action> >);

		void addAction(std::shared_ptr<Action>);

		virtual float run(float) override;

		virtual void reset() override;

	};
	
	class ActionView2d : virtual public VapObject {
	protected:
		View2d * m_view = nullptr;
	public:
		void initTarget(View2d * target);
		View2d * getTarget() { return m_view; }
	};
	
	class ActionMove2d : public ActionDuration, public ActionView2d
	{
	protected:
		Point2 m_speed = { 0, 0 };
		Point2 m_acc = { 0, 0 };
		Point2 m_pos_var = { 0, 0 };
	public:
		static std::shared_ptr<Action> Create(View2d*, float duration, Point2 speed, Point2 acc, float delay = 0.f, float interval = 0.f);

	public:
		ActionMove2d(float delay = 0.f, float interval = 0.f, std::string const & name = "action move");

		/**
		 getTarget().pos + (speed * age) + (0.5 * acc * age * age) = to
		 */
		void initMove(Point2 speed, Point2 acc);

		virtual void runDuration() override;

		virtual void reset() override;
	};

	class ActionScale2d : public ActionDuration, public ActionView2d
	{
	protected:
		Point2 m_speed;
		Point2 m_acc = { 0, 0 };
		Point2 m_scale_var = { 0, 0 };
	public:
		static std::shared_ptr<Action> Create(View2d*, float duration, Point2 speed, Point2 acc, float delay = 0.f, float interval = 0.f);

	public:
		ActionScale2d(float delay = 0.f, float interval = 0.f, std::string const & name = "action scale");

		void initScale(Point2 speed, Point2 acc);

		virtual void runDuration() override;

		virtual void reset() override;
	};

	class ActionRotate2d : public ActionDuration, public ActionView2d
	{
	protected:
		float m_angular_velocity = 0;
		float m_acc = 0;
		float m_angular_var = 0;
	public:
		static std::shared_ptr<Action> Create(View2d*, float duration,float angular_velocity, float acc = 0, float delay = 0.f, float interval = 0.f);

	public:
		ActionRotate2d(float delay = 0.f, float interval = 0.f, std::string const & name = "action rotate");

		void initRotate(float velo, float acc);

		virtual void runDuration() override;

		virtual void reset() override;
	};

}}

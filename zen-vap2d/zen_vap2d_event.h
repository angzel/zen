#pragma once

#include "zen_vap2d_config.h"
#include "zen_vap2d_node.h"
#include <vector>
#include <mutex>
#include <functional>

namespace Zen { namespace Vap2d {
	class Action : public VapObject {
	protected:
		float m_delay;
		float m_interval;
	public:
		Action(float delay = 0.f, float interval = 0.f, std::string const & name = "action");
		/**
		 @function run
		 return delay > 0, if ==0 run in next frame.
		 -1 stop action
		 */
		virtual float begin() { return m_delay; }

		virtual float run(float) { return -1; }

		virtual void reset() {}

		void setInterval(float interval);
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
		void init(std::function<bool(float)> call, int times);

		virtual float run(float) override;

		virtual void reset() override;
	};

	class ActionMove : public Action
	{
	protected:
		View * m_view = nullptr;
		Point2 m_speed = { 0, 0 };
		float m_running_time = 0;
		float m_last_time = 0;

	public:
		static std::shared_ptr<Action> Create(View*, Point2 speed, float last, float delay = 0.f, float interval = 0.f);

	public:
		ActionMove(float delay = 0.f, float interval = 0.f, std::string const & name = "action move");

		/**
		 @param view is an external pointer.
		 view.pos + speed * last = to
		 */
		void init(View * view, Point2 speed, float last);

		virtual float run(float) override;

		virtual void reset() override;
	};

	class ActionScale : public Action
	{
	protected:
		View * m_view = nullptr;
		Point2 m_speed = { 0, 0 };
		float m_running_time = 0;
		float m_last_time = 0;

	public:
		static std::shared_ptr<Action> Create(View*, Point2 speed, float last, float delay = 0.f, float interval = 0.f);

	public:
		ActionScale(float delay = 0.f, float interval = 0.f, std::string const & name = "action scale");

		/**
		 @param view is an external pointer.
		 */
		void init(View * view, Point2 speed, float last);

		virtual float run(float) override;

		virtual void reset() override;
	};

	class ActionRotate : public Action
	{
	protected:
		View * m_view = nullptr; // external pointer.
		float m_angular_velocity = 0;
		float m_running_time = 0;
		float m_last_time = 0;

	public:
		static std::shared_ptr<Action> Create(View *, float angular_velocity, float last, float delay = 0.f, float interval = 0.f);

	public:
		ActionRotate(float delay = 0.f, float interval = 0.f, std::string const & name = "action rotate");
		/**
		 @param view is an external pointer.
		 */
		void init(View * view, float angular_velocity, float last);

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
}}

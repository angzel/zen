#include "zen_vap2d_event.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {
	Action::Action(float delay, float interval, std::string const & name)
	: VapObject(name)
	, m_delay(delay)
	, m_interval(interval)
	{
	}

	ActionCall::ActionCall(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{
	}

	void ActionCall::init(std::function<bool (float)> call, int times)
	{
		m_call = call;
		m_times = times;
	}
	float ActionCall::run(float delay)
	{
		++m_run_times;
		if(!m_call || !m_call(delay)) return -1;
		if(m_times > 0 && m_run_times >= m_times) return -1;
		return m_interval;
	}
	void ActionCall::reset()
	{
		m_run_times = 0;
	}

		// action move

	ActionMove::ActionMove(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{
	}
	void ActionMove::init(View *view, Point2 speed, float last)
	{
		m_view = view;
		m_speed = speed;
		m_last_time = last;
	}
	float ActionMove::run(float interval)
	{
		if(!m_view) return -1;
		if(m_running_time > m_last_time) return -1;

		m_running_time += interval;

		if(m_running_time > m_last_time)
		{
			float rest = interval - (m_running_time - m_last_time);
			m_view->setX(m_view->getX() + m_speed.x * rest);
			m_view->setY(m_view->getY() + m_speed.y * rest);
			return -1;
		}
		else
		{
			m_view->setX(m_view->getX() + m_speed.x * interval);
			m_view->setY(m_view->getY() + m_speed.y * interval);
			return m_interval;
		}
	}
	void ActionMove::reset()
	{
		m_running_time = 0;
	}

		// action scale

	ActionScale::ActionScale(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{

	}

	/**
	 @param view is an external pointer.
	 */
	void ActionScale::init(View * view, Point2 speed, float last)
	{
		m_view = view;
		m_speed = speed;
		m_last_time = last;
	}

	float ActionScale::run(float interval)
	{
		if(!m_view) return -1;
		if(m_running_time > m_last_time) return -1;

		m_running_time += interval;

		if(m_running_time > m_last_time)
		{
			float rest = interval - (m_running_time - m_last_time);
			m_view->setScaleX(m_view->getScaleX() + m_speed.x * rest);
			m_view->setScaleY(m_view->getScaleY() + m_speed.y * rest);
			return -1;
		}
		else
		{
			m_view->setScaleX(m_view->getScaleX() + m_speed.x * interval);
			m_view->setScaleY(m_view->getScaleY() + m_speed.y * interval);
			return m_interval;
		}
	}

	void ActionScale::reset()
	{
		m_running_time = 0;
	}

		// action rotate

	ActionRotate::ActionRotate(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{

	}

	void ActionRotate::init(View * view, float angular_velocity, float last)
	{
		m_view = view;
		m_angular_velocity = angular_velocity;
		m_last_time = last;
	}
	float ActionRotate::run(float interval)
	{
		if(!m_view) return -1;

		if(m_running_time > m_last_time) return -1;

		m_running_time += interval;

		if(m_running_time > m_last_time)
		{
			float rest = interval - (m_running_time - m_last_time);
			m_view->setRotation(m_view->getRotation() + rest * m_angular_velocity);
			return -1;
		}
		else
		{
			m_view->setRotation(m_view->getRotation() + interval * m_angular_velocity);
			return m_interval;
		}
	}
	void ActionRotate::reset()
	{
		m_running_time = 0;
	}

		// action repeat

	ActionRepeat::ActionRepeat(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{

	}
	void ActionRepeat::init(std::shared_ptr<Action> action, int times)
	{
		m_action = action;
		m_times = times;
	}

	float ActionRepeat::run(float interval)
	{
		if(!m_action) return -1;

		if(!m_is_inner_run)
		{
			++m_run_times;
			m_is_inner_run = true;
			return m_action->begin();
		}

		auto res = m_action->run(interval);
		if(res >= 0) return res;

		if(m_times > 0 && m_run_times >= m_times)
		{
			return -1;
		}

		m_action->reset();
		m_is_inner_run = false;
		return m_interval;
	}

	void ActionRepeat::reset()
	{
		m_run_times = 0;
		m_is_inner_run = false;
	}

		// action sequence

	ActionSequence::ActionSequence(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{
	}

	void ActionSequence::addAction(std::shared_ptr<Action> action)
	{
		m_actions.push_back(action);
	}

	float ActionSequence::run(float interval)
	{
		if(m_index >= m_actions.size()) return -1;
		auto action = m_actions[m_index];
		float res = 0;
		if(!m_is_inner_run)
		{
			m_is_inner_run = true;
			res = action->begin();
		}
		else
		{
			res = action->run(interval);
		}
		if(res < 0)
		{
			++m_index;
			m_is_inner_run = false;
			return m_interval;
		}
		return res;
	}

	void ActionSequence::init(std::vector<std::shared_ptr<Action> > actions)
	{
		m_actions.swap(actions);
	}

	void ActionSequence::reset()
	{
		m_index = 0;
		m_is_inner_run = false;
		for(auto i : m_actions) i->reset();
	}

	// Create
	std::shared_ptr<Action> ActionCall::Create(std::function<bool (float)> call, int times, float delay, float interval)
	{
		auto act = new ActionCall(delay, interval);
		act->init(call, times);
		return std::shared_ptr<Action>(act);
	}
	std::shared_ptr<Action> ActionMove::Create(View * view, Point2 speed, float last, float delay, float interval)
	{
		auto act = new ActionMove(delay, interval);
		act->init(view, speed, last);
		return std::shared_ptr<Action>(act);
	}
	std::shared_ptr<Action> ActionScale::Create(View * view, Point2 speed, float last, float delay, float interval)
	{
		auto act = new ActionScale(delay, interval);
		act->init(view, speed, last);
		return std::shared_ptr<Action>(act);
	}

	std::shared_ptr<Action> ActionRotate::Create(View * view, float angular_velocity, float last, float delay, float interval)
	{
		auto act = new ActionRotate(delay, interval);
		act->init(view, angular_velocity, last);
		return std::shared_ptr<Action>(act);
	}

	std::shared_ptr<Action> ActionRepeat::Create(std::shared_ptr<Action> action, int times, float delay, float interval)
	{
		auto act = new ActionRepeat(delay, interval);
		act->init(action, times);
		return std::shared_ptr<Action>(act);
	}
	std::shared_ptr<Action> ActionSequence::Create(std::vector<std::shared_ptr<Action> > actions, float delay, float interval)
	{
		auto act = new ActionSequence(delay, interval);
		act->m_actions.swap(actions);
		return std::shared_ptr<Action>(act);
	}
}}

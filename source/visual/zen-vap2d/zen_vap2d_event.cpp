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

	void ActionCall::initCall(std::function<bool (float)> call, int times)
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
	
	ActionDuration::ActionDuration(float delay, float interval, std::string const & name)
	: Action(delay, interval, name)
	{
		
	}
	void ActionDuration::initDuration(float duration)
	{
		m_duration = duration;
	}
	float ActionDuration::run(float delay)
	{
		m_age += delay;
		if(m_age > m_duration)
		{
			m_age = m_duration;
			this->runDuration();
			return -1;
		}
		else
		{
			this->runDuration();
			return m_interval;
		}
	}
	void ActionDuration::reset()
	{
		m_age = 0;
	}
	
	void ActionOnView_::initTarget(View *target)
	{
		m_view = target;
	}

		// action move

	ActionMove::ActionMove(float delay, float interval, std::string const & name)
	: ActionDuration(delay, interval, name)
	{
	}
	void ActionMove::initMove(Point2 speed, Point2 acc)
	{
		m_speed = speed;
		m_acc = acc;
	}
	void ActionMove::runDuration()
	{
		if(!getTarget()) return;
		
		auto age = getAge();
		float x = m_speed.x * age + 0.5f * age * age * m_acc.x;
		float y = m_speed.y * age + 0.5f * age * age * m_acc.y;
		
		auto target = getTarget();
		target->setX(target->getX() + x - m_pos_var.x);
		target->setY(target->getY() + y - m_pos_var.y);
		m_pos_var = {x, y};
	}
	void ActionMove::reset()
	{
		ActionDuration::reset();
		m_pos_var = {0, 0};
	}

		// action scale

	ActionScale::ActionScale(float delay, float interval, std::string const & name)
	: ActionDuration(delay, interval, name)
	{

	}

	void ActionScale::initScale(Point2 speed, Point2 acc)
	{
		m_speed = speed;
		m_acc = acc;
		m_scale_var = {0, 0};
	}

	void ActionScale::runDuration()
	{
		if(!getTarget()) return;
		auto age = getAge();
		
		float x = m_speed.x * age + 0.5f * age * age * m_acc.x;
		float y = m_speed.y * age + 0.5f * age * age * m_acc.y;
		
		auto target = getTarget();
		target->setScaleX(target->getScaleX() + x - m_scale_var.x);
		target->setScaleY(target->getScaleY() + y - m_scale_var.y);
		m_scale_var = {x, y};
	}

	void ActionScale::reset()
	{
		m_scale_var = { 0, 0 };
		ActionDuration::reset();
	}

		// action rotate

	ActionRotate::ActionRotate(float delay, float interval, std::string const & name)
	: ActionDuration(delay, interval, name)
	{
	}

	void ActionRotate::initRotate(float velo, float acc)
	{
		m_angular_velocity = velo;
		m_acc = acc;
		m_angular_var = 0;
	}
	void ActionRotate::runDuration()
	{
		if(!getTarget()) return;
		auto age = getAge();
		auto r = m_angular_velocity * age + 0.5f * age * age * m_acc;
		auto target = getTarget();
		target->setRotation(target->getRotation() + r - m_angular_var);
		m_angular_var = r;
	}
	void ActionRotate::reset()
	{
		m_angular_var = 0;
		ActionDuration::reset();
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
		act->initCall(call, times);
		return std::shared_ptr<Action>(act);
	}
	std::shared_ptr<Action> ActionMove::Create(View * target, float duration, Point2 speed, Point2 acc, float delay, float interval)
	{
		auto act = new ActionMove(delay, interval);
		act->initTarget(target);
		act->initDuration(duration);
		act->initMove(speed, acc);
		return std::shared_ptr<Action>(act);
	}
	std::shared_ptr<Action> ActionScale::Create(View * target, float duration, Point2 speed, Point2 acc, float delay, float interval)
	{
		auto act = new ActionScale(delay, interval);
		act->initTarget(target);
		act->initDuration(duration);
		act->initScale(speed, acc);
		return std::shared_ptr<Action>(act);
	}

	std::shared_ptr<Action> ActionRotate::Create(View * target, float duration, float angular_velocity, float acc, float delay, float interval)
	{
		auto act = new ActionRotate(delay, interval);
		act->initTarget(target);
		act->initDuration(duration);
		act->initRotate(angular_velocity, acc);
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

#pragma once

#include "MyConfig.h"

class MyActionBreathe : public ActionRepeat
{
public:
	static std::shared_ptr<Action> Create(View2d * target, float sx_var, float sy_var, float period)
	{
		auto act1 = ActionScale2d::Create(target, period, {sx_var, sy_var}, {0, 0});
		
		auto act2 = ActionScale2d::Create(target, period, {-sx_var, -sy_var}, {0, 0});
		
		auto act0 = ActionCall::Create([target](float)->bool { target->setScale(1, 0.8); return false; }, 1);
		
		auto act = std::shared_ptr<MyActionBreathe>(new MyActionBreathe);
		
		act->init(ActionSequence::Create({act0, act1, act2, act0}), 0);
		return act;
	}
};

class MyActionJumping : public ActionDuration, public ActionView2d
{
	Point2 m_range;
	float m_times;
	Point2 m_range_var = {0, 0};
public:
	static std::shared_ptr<Action> Create(View2d * target, Point2 range, float duration, int times)
	{
		auto act = new MyActionJumping;
		act->init(target, range, duration, times);
		return std::shared_ptr<Action>(act);
	}
	void init(View2d * target, Point2 range, float duration, int times)
	{
		m_range = range;
		m_times = times;
		initDuration(duration);
		initTarget(target);
	}
	virtual void runDuration() override
	{
		auto age = getAge();
		auto duration = getDuration();
		
		auto p = age / duration;
		
		float ang = p * m_times * (M_PI_2 + p*M_PI) ;
		float s = fabs(sin(ang)) * (1-p) * (1-p);

		Point2 range2 = { m_range.x * s, m_range.y * s };
		
		auto target = getTarget();
		target->setX(target->getX() + range2.x - m_range_var.x);
		target->setY(target->getY() + range2.y - m_range_var.y);
		
		m_range_var = range2;
		std::cout << age << " " << range2.x << " " << range2.y << std::endl;
	}
};

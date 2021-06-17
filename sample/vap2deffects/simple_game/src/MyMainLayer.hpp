#pragma once

#include "MyConfig.h"
#include "MyFlover.hpp"
#include "MyAction.hpp"

class MyMainLayer : public Layer {
public:
	MyFlover * m_flover;
	std::shared_ptr<Action> m_jump;
	Point2 m_pos;
	
	MyMainLayer()
	{
		m_flover = new MyFlover;
		this->addNode(m_flover);
		m_pos = { gConfig.size.w/2, gConfig.size.h/2 };

		m_flover->setAnchor(0.5f, 0.5f);
		m_flover->setAreaSize({200, 200});
		m_flover->setPosition(m_pos);
		
//		m_flover->runAction(MyActionBreathe::Create(m_flover, 0.05, -0.05, 0.7f));
	}
	virtual Node * onTouchDown(Zen::AppTouch const & touch) override
	{
		Zen::LogV("touch sprite: %.2f %.2f", touch.x, touch.y);
		if(m_jump) m_flover->stopAction(m_jump);
		
		m_flover->setPosition(m_pos);
		m_flover->runAction(m_jump = MyActionJumping::Create(m_flover, {0, 10}, 5, 20));
		m_jump->setObjectName("jump action.");
		return this;
	}
};

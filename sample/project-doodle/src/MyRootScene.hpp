
#pragma once

#include "zen_vap2d.h"
#include "zen_numerical.h"

using namespace Zen;
using namespace Zen::Vap2d;

class MyRootScene : public Layer
{
	Zen::Random ra;
	Size2 m_size;

	Doodle * m_doodle;
public:
	MyRootScene()
	{
		m_size = Root::S()->getViewSize();

		m_doodle = new Doodle;
		m_doodle->setAreaSize({200, 200});
		m_doodle->setPosition(m_size.w/2, m_size.h/2);

		this->addNode(m_doodle);

		float a = 100;
		float b = 60;

		for(int i = 0; i < 10; ++i)
		{
			float r = ZEN_F_2PI / 10.f;
			float s = (i%2?a:b);
			float x = cosf(r) * s;
			float y = sinf(r) * s;
			m_doodle->addCoord({x, y}, Color(ra.next()));
		}
	}
};

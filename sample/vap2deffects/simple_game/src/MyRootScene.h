
#pragma once

#include "zen_vap.h"

#include "MyConfig.h"
#include "MyBGLayer.h"
#include "MyMainLayer.hpp"


using namespace Zen;
using namespace Zen::Vap;

class MyRootScene : public Layer
{
	Label * m_label;
	BGLayer * m_BG;
public:
	MyRootScene()
	{
		auto bg = new BGLayer;
		this->addNode(bg);

		m_label = new Label;
		m_label->setFont(gConfig.m_font_brush);
		m_label->setAnchor({0, 0});
		this->addNode(m_label);
		
		auto main = new MyMainLayer;
		this->addNode(main);
	}
	void setFPS(float fps)
	{
#if ZEN_DRAW_METAL
			std::string s = "metal " + Zen::ToString(fps);
#else
			std::string s = "opengl " + Zen::ToString(fps);
#endif
			m_label->setText(s);
	}

};

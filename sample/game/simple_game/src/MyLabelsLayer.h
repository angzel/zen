
#pragma once

#include "MyConfig.h"

using namespace Zen;
using namespace Zen::Vap;

class LabelsLayer : public Layer
{
	Zen::Vap::Label * m_fps_label = nullptr;
	Zen::Vap::Label * m_info_label = nullptr;

public:
	LabelsLayer()
	{
		this->loadFPSLabel();
		this->loadInfoLabel();
	}
	void setFPSText(std::string text)
	{
		m_fps_label->setText(text);
	}
	void loadFPSLabel()
	{
		auto label = new Zen::Vap::Label;

		label->setFont(gConfig.m_font_brush);
		label->setPosition(10, 0);
		label->setAnchor(0, 0);
		label->setCharsSpacingInPixels(2);
		label->setLinesSpacingInPixels(2);
		label->setText("i'm FPS");
		label->setAlpha(0.8f);
		label->setMaxWidthInPixels(0);
		label->setOrder(120);
		label->setTextAlignment(0.f);
		label->setUnderlineInPixels(2);
		label->setFontSize(20);

		m_fps_label = label;
		this->addNode(m_fps_label);

	}
	void loadInfoLabel()
	{
		auto label = new Zen::Vap::Label;

		label->setFont(gConfig.m_font_brush);
		label->setPosition(10, gConfig.size.h);
		label->setAnchor(0, 1);
		label->setCharsSpacingInPixels(2);
		label->setLinesSpacingInPixels(2);
		label->setText("Hello! This is test\n123456789\n-=<>!!\nAlignment:left");
		label->setAlpha(0.8f);
		label->setMaxWidthInPixels(0);
		label->setOrder(120);
		label->setTextAlignment(0.f);
		label->setUnderlineInPixels(2);
		label->setFontSize(20);

		m_info_label = label;
		this->addNode(m_info_label);
	}
};

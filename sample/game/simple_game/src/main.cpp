	//
	//  main.cpp
	//  gles_debug
	//
	//  Created by lover on 2019/12/4.
	//  Copyright © 2019 lover. All rights reserved.
	//
#include "zen_app_config.h"

//#if ZEN_APP_DRAW_API_GLES

#if 1
#include <stdio.h>
#include <vector>
#include "MySprite.h"
#include "MyRootScene.h"
#include "zen_cast.h"
#include "zen_image_jpg.h"
#include "zen_image_png.h"
#include "zen_cast.h"

Zen::Random ra;

Zen::ImageRawCoder raw_coder;
Zen::ImageJPGCoder jpg_coder;
Zen::ImagePNGCoder png_coder;

class ScreenColorDelegate : public Zen::Vap2d::RootDelegate {

	Zen::Vap2d::Label * m_label = nullptr;
	float fps = 0;
	float m_time = 0;
	int nth = 0;
	std::shared_ptr<FontBrush> m_font_brush;
public:
	virtual void onUpdate(float interval) override
	{
		m_time += interval;
		++nth;
		if(nth == 200)
		{
			fps = (float)nth/m_time;
#if ZEN_APP_DRAW_API_METAL
			std::string s = "metal " + Zen::ToString(fps);
#else
			std::string s = "opengl " + Zen::ToString(fps);
#endif
			if(m_label) m_label->setText(s);
			m_time = 0;
			nth = 0;
		}
	}

	virtual void onLaunch() override
	{
		Zen::App::S()->setFramesPerSecond(60);
		Zen::App::S()->setRotatable(true);

		auto size = Zen::Vap2d::Root::S()->getRealViewSize();
		LogV("ScreenSize: %.2f %.2f", size.w, size.h);
		auto w = 400.f;
		auto h = size.h / size.w * w;

		auto root = Zen::Vap2d::Root::S();
		root->setViewSize({w, h});
		LogV("ViewSize: %.2f %.2f", w, h);

		this->loadSource();

		auto layer = new MyRootScene;
		root->replaceRootNode(layer);

		this->loadFPSLabel(w/2, h/4);
		layer->addNode(m_label);
	}

	void loadSource()
	{
		Textures::S()->setImageDecoder("raw", std::shared_ptr<ImageRawCoder>(new ImageRawCoder));
		Textures::S()->setImageDecoder("png", std::shared_ptr<ImagePNGCoder>(new ImagePNGCoder));
		Textures::S()->setImageDecoder("jpg", std::shared_ptr<ImageJPGCoder>(new ImageJPGCoder));

		auto font_data = Zen::LoadResourceContent("res/ft.ttf");
		Zen::Fonts::S()->setFont("funny font", font_data);
		Zen::FontConfig config;
		config.width = 24;
		config.height = 0;
		config.italic = 0;

		m_font_brush = Zen::FontBrush::Create(Zen::Fonts::S()->getFont("funny font"), config);
	}
	void loadFPSLabel(float x, float y)
	{
		auto label = new Zen::Vap2d::Label;

		label->setFont(m_font_brush);
		label->setPosition(x, y);
		label->setCharsSpacingInPixels(2);
		label->setLinesSpacingInPixels(2);
		label->setText("Hello!This is test\n123456789\n-=<>!!");
		label->setAlpha(0.8f);
		label->setMaxWidthInPixels(0);
		label->setOrder(120);
		label->setTextAlignment(0.5f);
		label->setUnderlineInPixels(2);
		label->setFontSize(20);
		m_label = label;
	}

};

int ZenAppMain(int argc, const char ** argv)
{
	auto root = Zen::Vap2d::Root::S();
	root->run(std::shared_ptr<RootDelegate>(new ScreenColorDelegate));
	return 0;
}

#endif

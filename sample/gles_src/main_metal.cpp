	//
	//  main.cpp
	//  gles_debug
	//
	//  Created by lover on 2019/12/4.
	//  Copyright © 2019 lover. All rights reserved.
	//
#include "zen_app_config.h"

#if ZEN_APP_DRAW_API_METAL
#include <vector>
#include "MyDrawNode.h"
#include "MySprite.h"
#include "MyDrawNode.h"
//#include "MyDrawNodeMT.h"
//#include "MyRootScene.hpp"
#include "zen_cast.h"


Zen::Random ra;

Zen::ImageDecoderRaw raw_decoder;

class ScreenColorDelegate : public Zen::Vap2d::RootDelegate {

	Zen::Vap2d::Label * m_label = nullptr;
	float fps = 0;
	float m_time = 0;
	int nth = 0;
	std::shared_ptr<FontBrush> m_font_brush;
	Layer * m_sprite_layer = nullptr;
public:
	virtual void onUpdate(float interval) override
	{
		m_time += interval;
		++nth;
		if(nth == 200)
		{
			fps = (float)nth/m_time;
			m_label->setText(Zen::ToString(fps));
			m_label->setSize(m_label->getContentSize());
			m_time = 0;
			nth = 0;
		}
	}

	void loadSource()
	{
		Textures::S()->setImageDecoder("raw", &raw_decoder);

		auto font_data = Zen::LoadResourceContent("Arial.ttf");
		Zen::Fonts::S()->setFont("arial", font_data);
		Zen::FontConfig config;
		config.width = 24;
		config.height = 0;
		config.italic = 0;

		m_font_brush = Zen::FontBrush::Create(Zen::Fonts::S()->getFont("arial"), config);
	}
	virtual void onLaunch() override
	{
		Zen::AppRuntime::S()->setFramesPerSecond(60);
		Zen::AppRuntime::S()->setRotatable(true);

		auto size = Zen::Vap2d::Root::S()->getRealViewSize();
		LogV("ScreenSize: %.2f %.2f", size.w, size.h);
		auto w = 400.f;
		auto h = size.h / size.w * w;

		auto root = Zen::Vap2d::Root::S();
		root->setViewSize({w, h});
		LogV("ViewSize: %.2f %.2f", w, h);

		this->loadSource();

		auto layer = new Layer;
//
		root->replaceRootNode(layer);

		m_sprite_layer = new Layer;
		m_sprite_layer->setScale(0.5f, 0.5f);
		m_sprite_layer->setPosition(w/2, h/2);
		m_sprite_layer->setRotation(0.5);
		m_sprite_layer->runAction(ActionRotate::Create(m_sprite_layer, -0.5, 10000));

		layer->addNode(m_sprite_layer);


		for(int i = 0; i < 1; ++i)
		{
			this->addSprite();
		}
//
		this->loadLabel(w/2, h/4);
		layer->addNode(m_label);
//
//		layer->addNode(new MyDrawNodeMT);
	}
	void addSprite()
	{
		auto texture = Textures::S()->getCircleTexture();
		if(ra.next(2)) texture = Textures::S()->getTexture("a.raw");

		auto sprite = new MySprite;
		sprite->setTexture(texture);
		sprite->setTextureRect({0, 0, 1, 1});
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setOrder(3);
		auto sv = ra.nextf() * 0.5f + 0.2f;
		sprite->setScale(sv, sv);
		sprite->setRotation(ra.nextf()*ZEN_F_PI);
		sprite->setPosition(ra.nextf()*200, ra.nextf() * 400);

		float x = ra.nextf() * 100 + 100;
		float y = ra.nextf() * 100;
		auto rotate = new Zen::Vap2d::ActionRotate(0, 0);
		rotate->init(sprite, 1.f, 5);
		auto r = new Zen::Vap2d::ActionRepeat(5, 3);
		r->init(std::shared_ptr<Action>(rotate), 3);
		sprite->runAction(std::shared_ptr<Action>(r));

		float delay = ra.nextf() * 0.5f + 0.8f;
		auto move1 = new ActionMove(0, 0);
		move1->init(sprite, {x, y}, delay);
		auto move2 = new ActionMove(0, 0);
		move2->init(sprite, {-x, -y}, delay);
		auto seq = new ActionSequence(0, 0);
		seq->addAction(std::shared_ptr<Action>(move1));
		seq->addAction(std::shared_ptr<Action>(move2));
		auto rep = new ActionRepeat(0, 0.1f);
		rep->init(std::shared_ptr<Action>(seq), 100);
		sprite->runAction(std::shared_ptr<Action>(rep));

		m_sprite_layer->addNode(sprite);
	}
	void loadLabel(float x, float y)
	{
		auto label = new Zen::Vap2d::Label;

		label->setFont(m_font_brush);
		label->setPosition(x, y);
		label->setCharsSpacingInPixels(2);
		label->setLinesSpacingInPixels(2);
		label->setText("Aagllo\n123467890-+");
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
	root->run();
	root->setDelegate(std::shared_ptr<RootDelegate>(new ScreenColorDelegate));

	return 0;
}


#endif

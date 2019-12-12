	//
	//  main.cpp
	//  gles_debug
	//
	//  Created by lover on 2019/12/4.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <stdio.h>
#include <vector>
#include "MyDrawNode.h"
#include "MySprite.h"

Zen::Random ra;

Zen::ImageDecoderRaw raw_decoder;

class ScreenColorDelegate : public Zen::Vap2d::RootDelegate {
	Zen::Random r;
	float v = 0;
	bool touched = false;

	Zen::Vap2d::Layer * m_layer;
	Zen::Vap2d::Layer * m_sprite_layer;
	float m_time = 0;
	float m_view_w = 400;
	float m_view_h = 0;

	std::shared_ptr<FontBrush> m_font_brush;
public:
	int nth = 0;
	virtual void onUpdate(float interval) override
	{
		m_time += interval;
		++nth;
		if(nth == 200)
		{
			printf("fps:%.2f %.2f %.2f\n", (float)nth/m_time, m_time, Root::GetDefault()->getRunningTime());
			m_time = 0;
			nth = 0;
		}
	}

	void loadSource()
	{
		Textures::GetDefault()->setImageDecoder("raw", &raw_decoder);

		auto font_data = Zen::LoadResourceContent("Arial.ttf");
		Zen::Fonts::GetDefault()->setFont("arial", font_data);
		Zen::FontConfig config;
		config.width = 24;
		config.height = 0;
		config.italic = 0;

		m_font_brush = Zen::FontBrush::Create(Zen::Fonts::GetDefault()->getFont("arial"), config);
	}
	virtual void onLaunch() override
	{
		using namespace Zen::GL;
		Zen::AppRuntime::GetDefault()->setFramesPerSecond(60);
		Zen::AppRuntime::GetDefault()->setRotatable(true);

		this->loadSource();

		auto size = Zen::Vap2d::Root::GetDefault()->getRealViewSize();
		LogV("ScreenSize: %.2f %.2f", size.w, size.h);
		m_view_w = 400;
		m_view_h = size.h / size.w * m_view_w;

		auto root = Zen::Vap2d::Root::GetDefault();
		root->setViewSize({m_view_w, m_view_h});
		LogV("ViewSize: %.2f %.2f", m_view_w, m_view_h);


		m_layer = new Zen::Vap2d::Layer;

		root->replaceRootNode(m_layer);

		m_sprite_layer = new Layer;
		m_sprite_layer->setScale(0.5f, 0.5f);
		m_sprite_layer->setPosition(m_view_w/2, m_view_h/2);
		m_sprite_layer->setRotation(0.5);
		m_sprite_layer->runAction(ActionRotate::Create(m_sprite_layer, -0.5, 10000));
		m_layer->addNode(m_sprite_layer);

		this->addBackground();

		for(int i = 0; i < 100; ++i)
		{
			this->addSprite();
		}

		this->addLabel(0, 0);

		this->addPar1(m_view_w/2, m_view_h/2);
		this->addPar2(m_view_w/2, m_view_h*0.75f);
		this->addDoodle(200, 200);

		m_layer->addNode(new MyDrawNode);

		this->addSpriteA( 0, 0, 1, 1, EColor::Red);
		this->addSpriteA( 0, 0, 1, -1.5, EColor::Blue);
		this->addSpriteA( 0.5, 0.5, -1, 1, EColor::Green);

		this->addSpriteB({1, 1, -1, -1}, 0, 0, EColor::Red);
		this->addSpriteB({1, 1, -1, -1}, 0.5, 0.5, EColor::Blue);
		this->addSpriteB({1, 1, -1, -1}, 1, 1, EColor::Green);

		this->addSpriteC({0, 0, 1, 1}, 0, 0, EColor::Red);
		this->addSpriteC({0, 0, 1, 1}, 0.5, 0.5, EColor::Blue);
		this->addSpriteC({0, 0, 1, 1}, 1, 1, EColor::Green);

		this->addSpriteD({1, 1, -1, -1}, 0, 0, EColor::Red, 2);
		this->addSpriteD({1, 1, -1, -1}, 0.5, 0.5, EColor::Blue, 1.5);
		this->addSpriteD({1, 1, -1, -1}, 1, 1, EColor::Green, 1);
	}
	void addSpriteD(Rect rect, float ax, float ay, Color4f color, float scale)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::GetDefault()->getTexture("a.raw"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 500);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		sprite->setSize(50, 50);
		sprite->setScale(scale, scale);
		m_layer->addNode(sprite);
			//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteC(Rect rect, float ax, float ay, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::GetDefault()->getTexture("a.raw"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 400);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		m_layer->addNode(sprite);
//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteB(Rect rect, float ax, float ay, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::GetDefault()->getTexture("a.raw"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 300);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		m_layer->addNode(sprite);
//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteA(float ax, float ay, float sx, float sy, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::GetDefault()->getTexture("a.raw"));
		sprite->setPosition(100, 200);
		sprite->setAnchor(ax, ay);
		sprite->setScale(sx, sy);
		sprite->setOrder(1000);
		sprite->setColor(color);
		m_layer->addNode(sprite);
	}
	void addSprite()
	{
		auto texture = Textures::GetDefault()->getCircleTexture();
		if(ra.next(2)) texture = Textures::GetDefault()->getTexture("a.raw");
		
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
	void addPar1(float x, float y)
	{
		auto p = new Zen::Vap2d::GravityParticle;
		p->setTexture(Textures::GetDefault()->getParticleTexture());
		p->setColorLerpFunction([](float s, float e, float v) {
			return Zen::Lerp(s, e, std::fabsf(0.5f - v) * 2);
		});
		p->setPosition(x, y);
		p->setOrder(10);
		p->start();
		m_layer->addNode(p);
	}
	void addPar2(float x, float y)
	{
		auto ga = new GalaxyParticle;

		ga->setPosition(50, 250);
		ga->setUpdateInterval(0.05f);
		ga->setRadiusLerpFunction([](float s, float e, float v) {
			if(s < e) return Zen::Lerp(s, e, v*v);
			else return Zen::Lerp(e, s, Zen::Square(1-v));
		});
		ga->setParticleStartColorsVarying(Zen::EColor::White);
		ga->setParticleEndColorsVarying(Zen::EColor::White);
		ga->setColor(Zen::EColor::White);
		ga->setPosition(x, y);
		ga->setParticleTotalCount(128);
		m_layer->addNode(ga);
		ga->start();
		ga->setParticleScale(2.f);
		
		auto s1 = ActionScale::Create(ga, {1, 1}, 1);
		auto s2 = ActionScale::Create(ga, {-1, -1}, 1);
		ga->runAction(ActionRepeat::Create(ActionSequence::Create({s1, s2}), 0));
	}
	void addDoodle(float x, float y)
	{
		auto d = new Doodle;
		d->addCoord({0, 0}, Zen::Color4f(0xff00ffff));
		d->addCoord({1, 0}, Zen::Color4f(0x22ffffff));
		d->addCoord({0, 1}, Zen::Color4f(0x3333ffff));
		d->addCoord({1, 1}, Zen::Color4f(0x00ffffff));
		d->setSize(80, 80);
		d->setPosition(x, y);
		d->setAnchor(0.5f, 0.5f);
		d->setDrawMode(Zen::Vap2d::EDrawMode::Points, 25);
		d->runAction(ActionRotate::Create(d, -1, 1000));
		m_layer->addNode(d);
		auto cd = new DoodleColors;
		cd->setPosition(x, y);
		cd->setSize(40, 40);
		cd->setAnchor(0.5f, 0.5f);
		m_layer->addNode(cd);
	}
	void addLabel(float x, float y)
	{
		auto label = new Zen::Vap2d::Label;

		label->setFont(m_font_brush);
		label->setPosition(x, y);
		label->setCharsSpacingInPixels(2);
		label->setLinesSpacingInPixels(2);
		label->setColor(Zen::EColor::White);
		label->setText("Aagllo\n123467890-+");
		label->setAlpha(0.8f);
		label->setMaxWidthInPixels(0);
		label->setOrder(120);
		label->setTextAlignment(0.5f);
		label->setTextureBackground(0.3f);
		label->setUnderline(2);
		label->setFontSize(20);

		auto action = new Zen::Vap2d::ActionCall(0, 1);
		action->init([label](float delay)->bool {
			static int i = 0;
			i++;
			label->setText("Aag This is test\nNow is:" + std::to_string((int)i) + " \n00-+ 2");
			return true;
		}, 0);
		label->runAction(std::shared_ptr<Action>(action));

		m_layer->addNode(label);
	}
	void addBackground()
	{
		auto d = new Doodle;
		for(float i = 100; i < m_view_w; i += 100)
		{
			d->addCoord({i, 0}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addCoord({i, m_view_h}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		for(float i = 100; i < m_view_h; i += 100)
		{
			d->addCoord({0, i}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addCoord({m_view_w, i}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		d->setSize(1, 1);
		d->setDrawMode(Zen::GL::EDrawMode::Lines, 0);
		d->setOrder(-1);
		m_layer->addNode(d);

		auto line = new DoodleLine;
		line->setBeginCoord({0, 0});
		line->setEndCoord({m_view_w, m_view_h});
		line->setBeginColor(Zen::Color4f(0x3388ff99));
		line->setLineWidth(10);
		m_layer->addNode(line);
	}

};
int ZenAppMain(int argc, const char ** argv)
{
	auto root = Zen::Vap2d::Root::GetDefault();
	root->run();
	root->setDelegate(new ScreenColorDelegate);
	return 0;
}

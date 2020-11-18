
#pragma once

#include "zen_vap2d.h"
#include "MySprite.h"

using namespace Zen;
using namespace Zen::Vap2d;

class MyRootScene : public Layer
{
	Zen::Random ra;
	Size2 m_size;
	Layer * m_sprite_layer = nullptr;

public:
	MyRootScene()
	{
		m_size = Root::S()->getViewSize();

		this->addBackground();
//		return;

		Zen::Color4f colors[3] = {
			Zen::eColor::Red,
			Zen::eColor::Green,
			Zen::eColor::Blue,
		};
		auto & eColor_Red = colors[0];
		auto & eColor_Blue = colors[1];
		auto & eColor_Green = colors[2];
//
		this->addSpriteA( 0, 0, 1, 1, eColor_Red);
		this->addSpriteA( 0, 0, 1, -1.5, eColor_Blue);
		this->addSpriteA( 0.5, 0.5, -1, 1, eColor_Green);

		for(auto & c : colors) c.alpha = 0.9;

		this->addSpriteB({0, 0, 1, 1}, 0, 0, eColor_Red);
		this->addSpriteB({0, 0, 1, 0.5}, 0.5, 0.5, eColor_Blue);
		this->addSpriteB({0, 0, 0.5, 0.5}, 1, 1, eColor_Green);

		for(auto & c : colors) c.alpha = 0.5;

		this->addSpriteC({0, 0, 1, 1}, 0, 0, eColor_Red);
		this->addSpriteC({0, 0, 1, 1}, 0.5, 0.5, eColor_Blue);
		this->addSpriteC({0, 0, 1, 1}, 1, 1, eColor_Green);

		for(auto & c : colors) c.alpha = 0.2;

		this->addSpriteD({1, 1, -1, -1}, 0, 0, eColor_Red, 2);
		this->addSpriteD({1, 1, -1, -1}, 0.5, 0.5, eColor_Blue, 1.5);
		this->addSpriteD({1, 1, -1, -1}, 1, 1, eColor_Green, 1);

		this->addSpriteE(200, 300, true);
		this->addSpriteE(200, 400, false);


		this->addPar1(m_size.w/2, m_size.h/2);
		this->addPar2(m_size.w*0.7, m_size.h/4);
		this->addDoodle(m_size.w * 0.5, m_size.h*0.8);

		m_sprite_layer = new Layer;
		m_sprite_layer->setScale(0.5f, 0.5f);
		m_sprite_layer->setPosition(m_size.w/2, m_size.h/2);
		m_sprite_layer->setRotation(0.5);
		m_sprite_layer->runAction(ActionRotate::Create(m_sprite_layer, -0.5, 10000));

		this->addNode(m_sprite_layer);


		for(int i = 0; i < 50; ++i)
		{
			this->addSprite();
		}

		// test touch
		this->addNode(new MySprite);
	}
	void addSpriteE(float x, float y, float grey)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::S()->getTexture("res/o64.png"));
		sprite->setPosition(x, y);
		sprite->setAnchor(0.5, 0.5);
		sprite->setScale(1, 1);
		sprite->setAreaSize({50, 50});
		sprite->setGreyMode(grey);
		this->addNode(sprite);
	}
	void addSpriteD(Rect4f rect, float ax, float ay, Color4f color, float scale)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::S()->getTexture("res/o64.png"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 500);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		sprite->setAreaSize({80, 80});
		sprite->setScale(scale, scale);
		this->addNode(sprite);
			//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteC(Rect4f rect, float ax, float ay, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::S()->getTexture("res/o64.png"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 400);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		this->addNode(sprite);
			//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteB(Rect4f rect, float ax, float ay, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::S()->getTexture("res/o64.png"));
		sprite->setTextureRect(rect);
		sprite->setPosition(100, 300);
		sprite->setAnchor(ax, ay);
		sprite->setColor(color);
		this->addNode(sprite);
			//		sprite->runAction(ActionRotate::Create(sprite, 1, 100000));

	}
	void addSpriteA(float ax, float ay, float sx, float sy, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::S()->getTexture("res/o64.png"));
		sprite->setPosition(100, 200);
		sprite->setAnchor(ax, ay);
		sprite->setScale(sx, sy);
		sprite->setOrder(1000);
		sprite->setColor(color);
		this->addNode(sprite);
	}
	void addBackground()
	{
		auto d = new Doodle;
		for(float i = 100; i < m_size.w; i += 100)
		{
			d->addCoord({i, 0}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addCoord({i, m_size.h}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		for(float i = 100; i < m_size.h; i += 100)
		{
			d->addCoord({0, i}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addCoord({m_size.w, i}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		d->setAreaSize({1, 1});
		d->setLineMode(false);
		d->setOrder(-1);
		this->addNode(d);
//		return;
		auto line = new DoodleLine;
		line->setBeginCoord({0, 0});
		line->setEndCoord({m_size.w, m_size.h});
		line->setBeginColor(Zen::Color4f(0x3388ff99));
		this->addNode(line);
	}
	void addPar1(float x, float y)
	{
		auto p = new Zen::Vap2d::GravityParticle;
		p->setTexture(Textures::S()->getParticleTexture());
		p->setColorLerpFunction([](float s, float e, float v) {
			return Zen::Lerp(s, e, std::fabsf(0.5f - v) * 2);
		});
		p->setPosition(x, y);
		p->setOrder(10);
		p->start();
		this->addNode(p);
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
		ga->setParticleStartColorsVarying(Zen::eColor::White);
		ga->setParticleEndColorsVarying(Zen::eColor::White);
		ga->setColor(Zen::eColor::White);
		ga->setPosition(x, y);
		ga->setParticleTotalCount(128);
		ga->start();
		ga->setParticleScale(2.f);

		auto s1 = ActionScale::Create(ga, {1, 1}, 1);
		auto s2 = ActionScale::Create(ga, {-1, -1}, 1);
		ga->runAction(ActionRepeat::Create(ActionSequence::Create({s1, s2}), 0));

		this->addNode(ga);

	}
	void addDoodle(float x, float y)
	{
		auto d = new Doodle;
		d->addCoord({0, 0}, Zen::Color4f(0xff00ffff));
		d->addCoord({1, 0}, Zen::Color4f(0x22ffffff));
		d->addCoord({0, 1}, Zen::Color4f(0x3333ffff));
		d->addCoord({1, 1}, Zen::Color4f(0x00ffffff));
		d->setAreaSize({80, 80});
		d->setPosition(x, y);
		d->setAnchor(0.5f, 0.5f);

		d->setTriangleMode(true);
		d->runAction(ActionRotate::Create(d, -1, 1000));
		this->addNode(d);
		auto cd = new DoodleColors;
		cd->setPosition(x, y);
		cd->setAreaSize({80, 80});
		cd->setAnchor(0.5f, 0.5f);
		this->addNode(cd);
	}
	void addSprite()
	{
		auto texture = Textures::S()->getCircleTexture();
		if(ra.next(2)) texture = Textures::S()->getTexture("res/o64.png");

		auto sprite = new Sprite;
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

//		if(ra.next(2)) sprite->setBlend(eBlend::None);

		m_sprite_layer->addNode(sprite);
	}
};

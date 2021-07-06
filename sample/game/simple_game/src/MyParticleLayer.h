
#pragma once

#include "zen_vap.h"
#include "MySprite.h"
#include <iostream>
#include <iomanip>
using namespace std;

using namespace Zen;
using namespace Zen::Vap;

class MyParLayer : public Layer
{
public:
	MyParLayer()
	{
		auto size = gConfig.size;
		this->addPar1(0, 0);
		this->addPar2(0, 200);
		this->setX(size.w/2);
		this->setRotation(-0.1f);
		this->runAction(ActionRotate2d::Create(this, 10000, 0.4));
	}
	void addPar1(float x, float y)
	{
		float sz = 100;
		auto p = new Zen::Vap::ParticleGravity;
		p->setTexture(Textures::Get()->getParticleTexture());
		p->setParticleEmitSpeed(100);
		p->setParticleLife(1);
		p->setParticleStartSize(sz);
		p->setPosition(x, y);
		p->setOrder(10);
		p->start();
		this->addNode(p);
		
		auto sp = new Sprite;
		sp->setAnchor(0.5f, 0.5f);
		sp->setTexture(Textures::Get()->getTexture("res/p.png"));
		sp->setPosition(x, y - 100);
//		sp->setScale2_(float x, float y)
		sp->setFixedSize({sz, sz});
		sp->enableFixedSize(true);
		this->addNode(sp);
	}
	void addPar2(float x, float y)
	{
		auto ga = new ParticleGalaxy;

		ga->setPosition(50, 250);
		ga->setUpdateInterval(0.05f);
		ga->setRadiusLerpFunction([](float s, float e, float v) {
			if(s < e) return Zen::Lerp(s, e, v*v);
			else return Zen::Lerp(e, s, Zen::Square(1-v));
		});
		ga->setParticleStartColorsVarying(Zen::eColor::White);
		ga->setParticleEndColorsVarying(Zen::eColor::White);
		ga->setParticleEndRadius(400);
		ga->setColor(Zen::eColor::White);
		ga->setPosition(x, y);
		ga->setParticleTotalCount(128);
		ga->start();
		
		ga->setParticleStartSize(20);
		ga->setParticleEndSize(0);
//
//		auto s1 = ActionScale2d::Create(ga, 1, { 1, 1 }, { 0, 0 });
//		auto s2 = ActionScale2d::Create(ga, 1, {-1, -1 }, { 0, 0 });
//		ga->runAction(ActionRepeat::Create(ActionSequence::Create({s1, s2}), 0));

		this->addNode(ga);

	}
	virtual void draw() override
	{
		Layer::draw();
//		auto m = getMatrix();
//		auto v = Matrix4MultiplyVector4(m, Vector4Make(0, 100, 0, 0));
//		auto g = Matrix4MultiplyVector4(m, Vector4Make(100, 0, 0, 0));
		cout.width(10);

	}
};


#pragma once

#include "zen_vap2d.h"
#include "MySprite.h"

using namespace Zen;
using namespace Zen::Vap2d;

class MyParLayer : public Layer
{
public:
	MyParLayer()
	{
		this->addPar1(0, 0);
		this->addPar2(0, 200);
	}
	void addPar1(float x, float y)
	{
		auto p = new Zen::Vap2d::GravityParticle;
		p->setTexture(Textures::Get()->getParticleTexture());
//		p->setTexture(Textures::Get()->getTexture("res/p.png"));

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
};


#pragma once

#include "zen_vap.h"
#include "MySprite.h"

using namespace Zen;
using namespace Zen::Vap;

class SpritesLayer : public Layer
{
public:
	SpritesLayer()
	{
		float xs[4];
		float x = 100;
		float step = 200;
		for(auto & i : xs)
		{
			i = x;
			x += step;
		}
		float y = 0;
		y += step;
		add(xs[0], y, { 0, 0 }, { 1, 1 }, 0);
		add(xs[1], y, { 0, 1 }, { 1, 1 }, 0);
		add(xs[2], y, { 1, 0 }, { 1, 1 }, 0);
		add(xs[3], y, { 1, 1 }, { 1, 1 }, 0);

		y += step;
		add(xs[0], y, { 0.5, 0.5}, { 1, 1 }, 0);
		add(xs[1], y, { 0.5, 0.5}, { 0.5, 1 }, 0);
		add(xs[2], y, { 0.5, 0.5}, { 1, 0.5}, 0);
		add(xs[3], y, { 0.5, 0.5}, { 0.5, 0.5}, 0);


		y += step;
		add_c(xs[0], y, eColor::Red);
		add_c(xs[1], y, eColor::Green);
		add_c(xs[2], y, eColor::Blue);
		add_c(xs[3], y, Color4b(0x77777777));

		y += step;
		// rect is {orign, size}
		add_r(xs[0], y, { 0, 0, 1, 1 });
		add_r(xs[1], y, { 0, 1, 1, -1 });
		add_r(xs[2], y, { 1, 1, -1, -1 });
		add_r(xs[3], y, { 0, 0, 0.5, 0.5});
	}
	Sprite* add(float x, float y, Point2 a, Point2 s, float r)
	{
		auto sprite = new Sprite;
//		sprite->setTexture(Textures::Get()->getTexture("res/o64.png"));
		sprite->setTexture(Textures::Get()->getParticleTexture());
		sprite->setPosition(x, y);
		sprite->setAnchor(a.x, a.y);
		sprite->setScale(s.x, s.y);
		sprite->setRotation(r);
		this->addNode(sprite);
		return sprite;
	}
	Sprite* add_c(float x, float y, Color4f color)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::Get()->getTexture("res/o64.png"));
		sprite->setPosition(x, y);
		sprite->setColor(color);
		sprite->setAnchor(0.5, 0.5);
//		sprite->setScale(s.x, s.y);
//		sprite->setRotation(r);

		this->addNode(sprite);
		return sprite;
	}
	Sprite* add_r(float x, float y, Rect4f rect)
	{
		auto sprite = new Sprite;
		sprite->setTexture(Textures::Get()->getTexture("res/o64.png"));
		sprite->setTextureRect(rect);
		sprite->setPosition(x, y);

		this->addNode(sprite);
		return sprite;
	}
};

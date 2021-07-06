
#pragma once

#include "zen_vap.h"
#include "MySprite.h"

using namespace Zen;
using namespace Zen::Vap;

class BGLayer : public Layer
{
public:
	BGLayer()
	{
		auto & ra = gConfig.ra;
		auto const size = gConfig.size;

		auto d = new FreeDoodle;
		for(float i = 100; i < size.w; i += 100)
		{
			d->addPoint({i, 0}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addPoint({i, size.h}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		for(float i = 100; i < size.h; i += 100)
		{
			d->addPoint({0, i}, Zen::Color4f(ra.next() | 0x777777ff));
			d->addPoint({size.w, i}, Zen::Color4f(ra.next() | 0x777777ff));
		}
		d->setAreaSize({1, 1});
		d->setLineMode(false);
		d->setOrder(-1);
		this->addNode(d);
		//		return;
		auto line = new ColoredLine;
		line->setBeginCoord({0, 0});
		line->setEndCoord({size.w, size.h});
		line->setBeginColor(Zen::Color4f(0x3388ff99));
		this->addNode(line);
	}
};

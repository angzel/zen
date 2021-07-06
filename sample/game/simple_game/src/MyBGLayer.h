
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
		auto & r = gConfig.ra;
		auto const size = gConfig.size;

		auto d = new FreeDoodle;
		for(float i = 100; i < size.w; i += 100)
		{
			d->addPoint({i, 0 }, Zen::Color4f(r.next() | 0x777777ff));
			d->addPoint({i, size.h}, Zen::Color4f(r.next() | 0x777777ff));
		}
		for(float i = 100; i < size.h; i += 100)
		{
			d->addPoint({ 0, i}, Zen::Color4f(r.next() | 0x777777ff));
			d->addPoint({size.w, i}, Zen::Color4f(r.next() | 0x777777ff));
		}
//		d->setAreaSize({ 1, 1 });
		d->setLineMode(false);
		d->setOrder(-1);
		this->addNode(d);
		//		return;
		auto line = new ColoredLine;
		line->setBeginCoord({ 0, 0 });
		line->setEndCoord({size.w, size.h});
		line->setBeginColor(Zen::Color4f(0x3388ff99));
		this->addNode(line);
		
		auto fi = new FigurePoints;
		for(int i = 0; i < 100; ++i)
		{
			auto dot = FigurePoints::VertexData();
			dot.coord = { r.nextf() * size.w, r.nextf() * size.h };
			dot.color = eColor::Grey;
			dot.psize = 50.f * r.nextf() + 30.f;
			fi->addVertex(dot);
		}
		fi->setPosition(0, 0);
//		fi->setTexture(Textures::Get()->getParticleTexture());
		fi->runAction(ActionCall::Create([fi,this](float) {
			auto & r = gConfig.ra;
			auto n = fi->getVertexesCount();
			auto vs = Root::Get()->getRealViewSize();
			for(size_t i = 0; i < n; ++i)
			{
				auto d=  fi->getVertex(i);
				d.psize += (0.5 - r.nextf()) * 5;
				if(d.psize < -50) d.psize = 50;
				else if(d.psize > 50) d.psize = -50;
				
				d.coord.x += (0.5 - r.nextf()) * 5;
				d.coord.y += (0.5 - r.nextf()) * 5;
				if(d.coord.x > vs.w) d.coord.x = vs.w;
				else if(d.coord.x < 0) d.coord.x = 0;
				if(d.coord.y > vs.h) d.coord.y = vs.h;
				else if(d.coord.y < 0) d.coord.y = 0;
				
				d.color.red += (0.5 - r.nextf()) * 0.01f;
				d.color.green += (0.5 - r.nextf()) * 0.01f;
				d.color.blue += (0.5 - r.nextf()) * 0.01f;
				fi->updateVertex(i, d);
			}
			return true;
		}, -1));
		this->addNode(fi);
	}
};


#pragma once

#include "MyConfig.h"

using namespace Zen;
using namespace Zen::Vap2d;

class DoodlesLayer : public Layer
{
public:
	DoodlesLayer()
	{
		auto size = gConfig.size;
		auto & ra = gConfig.ra;
		auto doo = new FreeDoodle;
		int count = 15;
		for(int i = 0; i < count; ++i)
		{
			auto c = Color4b(ra.next()|0xff000000);
			doo->addPoint({ra.nextf(), ra.nextf()}, c);
		}
		doo->setAreaSize(size);
		doo->setLineMode(1);
		this->addNode(doo);
	}
};

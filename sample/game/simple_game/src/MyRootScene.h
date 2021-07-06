
#pragma once

#include "zen_vap.h"

#include "MyConfig.h"
#include "MyLabelsLayer.h"
#include "MyParticleLayer.h"
#include "MySprite.h"
#include "MySpritesLayer.h"
#include "MyBGLayer.h"
#include "MyDoodlesLayer.h"

using namespace Zen;
using namespace Zen::Vap;

class MyRootScene : public Layer
{
	Zen::Random ra;
	Size2 m_size;
	LabelsLayer * m_labels;
	BGLayer * m_BG;
public:
	MyRootScene()
	{
		auto vs = Root::Get()->getRealViewSize();
		auto bg = new BGLayer;
		this->addNode(bg);
		//		return;
		
		auto sp = new SpritesLayer;
		this->addNode(sp);
		this->addNode(new SpritesLayer);
		
		{
			auto par = new MyParLayer;
			this->addNode(par);
			par->setY(vs.h/2 + 100);
			par->setScale(0.5f, 1.5f);

			
		}
		{
			auto par = new MyParLayer;
			this->addNode(par);
			par->setY(500); //(0, 500);
			par->setScale(0.5f, 0.5f);
		}
		
		auto doo = new DoodlesLayer;
		this->addNode(doo);
		
		m_labels = new LabelsLayer;
		this->addNode(m_labels);
	}
	void setFPS(float fps)
	{
#if ZEN_DRAW_METAL
		std::string s = "metal " + Zen::ToString(fps);
#else
		std::string s = "opengl " + Zen::ToString(fps);
#endif
		if(m_labels) m_labels->setFPSText(s);
	}
	
};

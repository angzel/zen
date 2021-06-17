
#pragma once

#include "zen_vap2d.h"
#include "MyConfig.h"
#include "MyRootScene.h"

using namespace Zen;
using namespace Zen::Vap2d;

class MainDelegate : public Zen::Vap2d::RootDelegate {
	float fps = 0;
	float m_time = 0;
	int nth = 0;

	MyRootScene * rootScene = nullptr;
public:
	virtual void onLaunch() override
	{
		Zen::App::Get()->setFramesPerSecond(60);
		Zen::App::Get()->setRotatable(true);

		auto size = Zen::Vap2d::Root::Get()->getRealViewSize();
		LogV("ScreenSize: %.2f %.2f", size.w, size.h);

		auto h = size.h * (1080 / size.w);
		Root::Get()->setViewSize({ 1080, h});
		
		gConfig.size = Root::Get()->getViewSize();

		this->loadSource();

#if ZEN_DRAW_GLES
		using namespace Zen::GL;
		GetGLRender()->setDepthFunc(eDepthFunc::Greater);
		GetGLRender()->enableDepthTest(false);
#endif
		this->loadScene();
	}
	void loadScene()
	{
		auto root = Zen::Vap2d::Root::Get();
		auto scene = new MyRootScene;
		root->replaceRootNode(scene);
		rootScene = scene;
	}
	void loadSource()
	{
		Textures::Get()->setImageDecoder("raw", std::shared_ptr<ImageRawCoder>(new ImageRawCoder));
		Textures::Get()->setImageDecoder("png", std::shared_ptr<ImagePNGCoder>(new ImagePNGCoder));
		Textures::Get()->setImageDecoder("jpg", std::shared_ptr<ImageJPGCoder>(new ImageJPGCoder));

		auto font_data = Zen::Utils::Get()->loadResource("res/ft.ttf");
		Zen::Fonts::Get()->setFont("funny font", font_data);

		Zen::FontConfig config;
		config.width = 24;
		config.height = 0;
		config.italic = 0;

		gConfig.m_font_brush = Zen::FontBrush::Create(Zen::Fonts::Get()->getFont("funny font"), config);
	}
	
	virtual void onResize() override
	{
	
	}
	virtual void onUpdate(float interval) override
	{
		m_time += interval;
		++nth;
		if(nth == 200)
		{
			fps = (float)nth/m_time;

			m_time = 0;
			nth = 0;

			rootScene->setFPS(fps);
		}
	}
};

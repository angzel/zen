
#pragma once

#include "zen_vap.h"
#include "MyConfig.h"
#include "MyRootScene.h"

using namespace Zen;
using namespace Zen::Vap;

class MainDelegate : public Zen::Vap::RootDelegate {
	float fps = 0;
	float m_time = 0;
	int nth = 0;
	Random ra;
	MyRootScene * rootScene = nullptr;
public:
	virtual void onLaunch() override
	{
		Zen::App::Get()->setFramesPerSecond(60);
		Zen::App::Get()->setRotatable(true);

		auto size = Zen::Vap::Root::Get()->getRealViewSize();
		LogV("ScreenSize: %.2f %.2f", size.w, size.h);

		auto h = size.h * (1080 / size.w);
		
		gConfig.size = { 1080, h};

		this->loadSource();

#if ZEN_DRAW_GLES
		using namespace Zen::GL;
		GetGLRender()->setDepthFunc(eDepthFunc::Greater);
		GetGLRender()->enableDepthTest(false);
#endif
//		this->load2DScene();
		
		this->load3D();
	}
	void load2DScene()
	{
		auto root = Zen::Vap::Root::Get();
		
		auto scene = new Scene2d(gConfig.size.w, gConfig.size.h);
		
		root->replaceRootNode(scene);
		
		rootScene = new MyRootScene;
		scene->addNode(rootScene);
		
	}
	void load3D()
	{
		auto s3d = new Scene;
		s3d->setDepthTest(true);
		float aspect = gConfig.size.w / gConfig.size.h;
		auto m1 = Matrix4MakePerspective(60 * (M_PI / 180.0f), aspect, 20, 1000);
//		auto m2 = Matrix4MakeTranslation(0, 0, -5);
//		s3d->getCamera().setMatrix(Matrix4Multiply(m1, m2));
		s3d->getCamera().setMatrix(m1, 0);

//		s3d->getCamera().setMatrix(Matrix4MakeFrustum(-aspect, aspect, -1, 1, 1, 1000));
	
		Zen::Vap::Root::Get()->replaceRootNode(s3d);
//		s3d->addNode(SkyBox());
		s3d->addNode(GroundMake());

		s3d->addNode(MainEntity());
		for(int j = -9; j < 10; ++j)
		for(int i = -9; i < 10; ++i)
			s3d->addNode(GetEntity(Vector3{2.f*i, 2.f*j, -1}));
//		s3d->addNode(GetEntity());
//		s3d->addNode(GetEntity());
	}
	Entity * GroundMake()
	{
		auto en = new Entity;
		en->move = {0, -60, -40};
		en->size = 100;
		en->ro = 0.f;
		en->dz = 0;
		en->setMesh(MeshCube::GetDefaultCube());
		return en;
	}
	Entity * SkyBox()
	{
		auto en = new Entity;
		en->size = 1000 / sqrt(2);
		en->ro = 0.1f;
		en->setMesh(MeshCube::GetDefaultCube());
		return en;
	}
	Entity * MainEntity()
	{
		auto en = new Entity();
		en->move = { 0, 5, 0};
		en->size = 2;
		en->ro = 5;
		en->setMesh(MeshCube::GetDefaultCube());
		en->show = true;
		return en;
	}
	
	Entity * GetEntity(Vector3 move)
	{
		auto en = new Entity();
//		float dx = 20;
//		float dz = 20;
		en->move = move; //{ra.nextf() * dx - dx/2, ra.nextf() * dx - dx/2, (ra.nextf()*(-dz) -dz)};
		en->size = 1.f;//ra.nextf() * 2 + 20;
//		en->r = ra.nextf() * 10;
		en->ro =  0;//ra.nextf() * 5 + 1;
		en->setMesh(MeshCube::GetDefaultCube());
		return en;
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

			if(rootScene) rootScene->setFPS(fps);
		}
	}
};

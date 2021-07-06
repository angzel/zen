
#include "zen_vap_scene.h"

namespace Zen { namespace Vap {
	
	Scene::Scene(std::string const & name)
	: BNode(name)
	{
	}
	
	void Scene::draw()
	{
		auto & mat = m_camera.getMatrix();
		DrawStack::GuardAll _ga(&mat, m_camera.getMatrixVersion(), 1.f, eBlend::Normal);
#if ZEN_DRAW_GLES
//		auto s = GetGLRender();
//		s->clearDepthBuffer();
//		s->enableDepthTest(m_depth_enable);
//		if(m_depth_enable) s->setDepthFunc(GL::eDepthFunc::Less);
#else
#endif
		BNode::draw();
	}
	void Scene::setDepthTest(bool test)
	{
		m_depth_enable = test;
	}
	
	bool Scene::isDepthTest()
	{
		return m_depth_enable;
	}
	
	
	Camera & Scene::getCamera()
	{
		return m_camera;
	}
	
	Scene2d::Scene2d(float width, float height)
	{
		m_camera.setMatrix(Matrix4MakeOrtho(0, width, 0, height, -1, 1), 0);
	}
	
}}

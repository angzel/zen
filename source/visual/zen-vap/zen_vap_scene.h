#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_vap_camera.h"
#include "zen_types.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Scene : public BNode {
	public:
		Scene(std::string const & name = "scene");
		
		virtual void draw() override;
		
		Camera & getCamera();
		
		void setDepthTest(bool test);
		
		bool isDepthTest();
	protected:
		Camera m_camera;
		bool m_depth_enable = false;
		bool m_depth_mask = false;
	};
	
	class Scene2d : public Scene {
	public:
		Scene2d(float width, float height);
	};
}}

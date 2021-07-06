#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_vap_mesh.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Entity : public Node2d, public Colorful {
	public:
		Entity(std::string const & name = "entity");
		
		void setMesh(std::shared_ptr<Mesh>);
		std::shared_ptr<Mesh> getMesh();
		
		virtual void draw() override;
	protected:
		std::shared_ptr<Mesh> m_mesh;
		std::vector<Color4f> m_colors;
	public:
		float size = 5.f;
		float ro = 0.01f;
		float r = 0.f;
		Vector3 move = { 0, 0, 0 };
		float dz = -0.5f;
		
		bool show = false;
	};
	class Ground : public Node2d {
	public:
		
		virtual void draw() override;
	};
}}

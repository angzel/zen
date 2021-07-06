#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Camera : public Matrixing {
	public:
		Camera();
//		
//		const Zen::Matrix4 &getMatrix() const;
//		
//		void setMatrix(Matrix4 const & v);
//	protected:
//		Matrix4 m_matrix = Matrix4MakeIdentity();
	};
}}

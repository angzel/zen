#pragma once
#include "zen_view_layer.h"
#include "zen_view_gl_node.h"

namespace Zen { namespace Vap2d {
	class GLLayer : public virtual Layer, public virtual GLContainerNode, public virtual GLFigure {
	public:
		virtual void draw() override;
		
		virtual Mat4 * _getMat() override;

		virtual int _getMatVersion() override;
		
	protected:
		int mMatVersion = 0;
		int mParentMatVersion = 0;
		Mat4 mFinalMat;
	};
}}

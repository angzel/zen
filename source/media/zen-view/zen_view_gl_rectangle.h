#pragma once

#include "zen_view_rectangle.h"
#include "zen_view_gl_node.h"

namespace Zen { namespace Vap2d {
	class GLRectangle : public virtual Rectangle, public virtual GLFinalNode, public virtual GLBoundFigure
	{
	public:
		GLRectangle();

		virtual void draw() override;

		virtual void setCoordColor(Zen::Color4f color, int index) override;

		virtual Zen::Color4f getCoordColor(int index) override;

	protected:
		Mat4 mFinalMat;
		int mParentMatVersion = 0;
		Zen::Color4f mColors[4];
	};
}}

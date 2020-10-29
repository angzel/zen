#pragma once

#include "zen_view_polygon.h"
#include "zen_view_gl_node.h"

namespace Zen { namespace Vap2d {
	class GLPolygon : public virtual Polygon, public virtual GLFinalNode, public GLBoundFigure {
	public:
		GLPolygon(size_t apex_count);

		virtual void draw() override;

		virtual void setCenterColor(Zen::Color4f color) override;

		virtual Zen::Color4f getCenterColor() override;

		virtual void setCoordColor(Zen::Color4f color, int index) override;

		virtual Zen::Color4f getCoordColor(int index) override;

		virtual size_t getApexCount() override;

		virtual void setCenterCoord(Zen::Pt2 pos) override;

		virtual Zen::Pt2 getCenterCoord() override;

	protected:
		Mat4 mFinalMat;
		int mParentMatVersion = 0;
		
		std::vector<Zen::Pt4> mCoords;
		std::vector<Zen::Color4f> mColors;
		size_t mApexCount;
	};
}}


#include "zen_view_gl_polygon.h"
#include "zen_gl_shader_color.h"

namespace Zen { namespace Vap2d {

	GLPolygon::GLPolygon(size_t apex_count)
	{
		mApexCount = apex_count;
		auto size = apex_count + 2;
		mColors.resize(size);
		mCoords.resize(size);
		float ang_i = ZEN_F_2PI / (float)apex_count;
		mCoords[0].set(0.5f, 0.5f, 0, 1);
		mColors[0] = Zen::EColor::White;
		for(uint32_t i = 0; i < apex_count; ++i)
		{
			mColors[i + 1] = Zen::EColor::White;
			float ang = ang_i * (float)i;
			mCoords[i + 1].set(::cos(ang) * 0.5f + 0.5f, ::sin(ang) * 0.5f + 0.5f, 0, 1);
		}
		mCoords[apex_count + 1] = mCoords[1];
		mColors[apex_count + 1] = mColors[1];
	}

	size_t GLPolygon::getApexCount()
	{
		return mApexCount;
	}
	void GLPolygon::setCenterColor(Zen::Color4f color)
	{
		mColors[0] = color;
	}

	Zen::Color4f GLPolygon::getCenterColor()
	{
		return mColors[0];
	}
	void GLPolygon::draw()
	{
		if(!this->isVisible()) return;
		
#include "zen_view_gl_update_final_mat_part.h"
		/** begin draw
		 */
		_GLNodeDrawingHelper helper(this);

		auto p = Zen::GL::ShaderColor::GetNormal();
		p->active();
		using namespace Zen::GL;
		DrawAPI::EnableVertexAttrib(p->getAttribCoord());
		DrawAPI::SetVertexAttribData(p->getAttribCoord(), 4, EDataType::Float, false, 0, mCoords.data());

		DrawAPI::EnableVertexAttrib(p->getAttribColor());
		DrawAPI::SetVertexAttribData(p->getAttribColor(), 4, EDataType::Float, false, 0, mColors.data());


		float a = helper.getFinalOpacity();
		DrawAPI::SetUniformFloat(p->getUniformColor(), mColor.red * a, mColor.green * a, mColor.blue * a, mColor.alpha * a);

		DrawAPI::SetUniformMat(p->getUniformTransform(), mFinalMat);

		DrawAPI::DrawArray(Zen::GL::EDrawMode::TriangleFan, 0, (uint32_t)mCoords.size());
	}


	void GLPolygon::setCoordColor(Zen::Color4f color, int index)
	{
		mColors[index+1] = color;
		if(index == 0) mColors.back() = color;
	}

	Zen::Color4f GLPolygon::getCoordColor(int index)
	{
		return mColors[index+1];
	}

	void GLPolygon::setCenterCoord(Zen::Pt2 pos)
	{
		mCoords[0].set(pos.x, pos.y, 0, 1);
	}

	Zen::Pt2 GLPolygon::getCenterCoord()
	{
		return { mCoords[0].x, mCoords[0].y };
	}

	Polygon * Polygon::Create(uint32_t apex_count)
	{
		auto poly = new GLPolygon(apex_count);
		return poly;
	}

}}

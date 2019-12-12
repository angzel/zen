
#include "zen_view_gl_star.h"
#include "zen_gl_shader_color.h"

namespace Zen { namespace Vap2d {

	GLStar::GLStar(size_t apex_count, float inner_radius)
	{
		mApexCount = apex_count;
		mInnerRadius = inner_radius;
		auto size = apex_count * 2 + 2;
		mColors.resize(size);
		mCoords.resize(size);
		mCoords[0].set(0.5f, 0.5f, 0, 1);
		mColors[0] = Zen::EColor::White;

		float ang_i = ZEN_F_PI / (float)apex_count, ang = 0.f;
		for(size_t i = 0; i < apex_count; ++i)
		{
			auto index = i * 2 + 1;
			mColors[index] = Zen::EColor::White;
			mCoords[index].set(::cos(ang) * 0.5f + 0.5f, ::sin(ang) * 0.5f + 0.5f, 0, 1);
			++index;
			ang += ang_i;
			mColors[index] = Zen::EColor::White;
			float ix = ::cos(ang) * 0.5f * inner_radius + 0.5f;
			float iy = ::sin(ang) * 0.5f * inner_radius + 0.5f;
			mCoords[index].set(ix, iy, 0, 1);
			ang += ang_i;

		}

		mCoords[apex_count * 2 + 1] = mCoords[1];
		mColors[apex_count * 2 + 1] = mColors[1];
	}

	size_t GLStar::getApexCount()
	{
		return mApexCount;
	}
	float GLStar::getInnerRadius()
	{
		return mInnerRadius;
	}
	void GLStar::setCenterColor(Zen::Color4f color)
	{
		mColors[0] = color;
	}

	Zen::Color4f GLStar::getCenterColor()
	{
		return mColors[0];
	}
	void GLStar::draw()
	{
		if(!this->isVisible()) return;

#include "zen_view_gl_update_final_mat_part.h"

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

	void GLStar::setCoordColor(Zen::Color4f color, int index)
	{
		mColors[index*2+1] = color;
		if(index == 0) mColors.back() = color;
	}

	Zen::Color4f GLStar::getCoordColor(int index)
	{
		return mColors[index*2+1];
	}
	void GLStar::setInnerCoordColor(Zen::Color4f color, int index)
	{
		mColors[index*2+2] = color;
	}

	Zen::Color4f GLStar::getInnerCoordColor(int index)
	{
		return mColors[index*2+2];
	}

	void GLStar::setCenterCoord(Zen::Pt2 pos)
	{
		mCoords[0].set(pos.x, pos.y, 0, 1);
	}

	Zen::Pt2 GLStar::getCenterCoord()
	{
		return { mCoords[0].x, mCoords[0].y };
	}

	Star * Star::Create(size_t apex_count, float inner_radius)
	{
		auto star = new GLStar(apex_count, inner_radius);
		return star;
	}

}}

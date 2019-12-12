
#include "zen_view_gl_rectangle.h"
#include "zen_gl_shader_color.h"

namespace Zen { namespace Vap2d {
	static Zen::Pt4 sCoords[4] = {
		{0, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 1, 0, 1},
		{0, 1, 0, 1},
	};

	GLRectangle::GLRectangle()
	{
		mColors[0] = Zen::EColor::White;
		mColors[1] = Zen::EColor::White;
		mColors[2] = Zen::EColor::White;
		mColors[3] = Zen::EColor::White;
	}
	void GLRectangle::draw()
	{
		if(!this->isVisible()) return;

#include "zen_view_gl_update_final_mat_part.h"

		_GLNodeDrawingHelper helper(this);

		auto p = Zen::GL::ShaderColor::GetNormal();
		p->active();
		using namespace Zen::GL;
		DrawAPI::EnableVertexAttrib(p->getAttribCoord());
		DrawAPI::SetVertexAttribData(p->getAttribCoord(), 4, EDataType::Float, false, 0, &sCoords);

		DrawAPI::EnableVertexAttrib(p->getAttribColor());
		DrawAPI::SetVertexAttribData(p->getAttribColor(), 4, EDataType::Float, false, 0, &mColors);


		float a = helper.getFinalOpacity();
		DrawAPI::SetUniformFloat(p->getUniformColor(), mColor.red * a, mColor.green * a, mColor.blue * a, mColor.alpha * a);

		DrawAPI::SetUniformMat(p->getUniformTransform(), mFinalMat);

		DrawAPI::DrawArray(Zen::GL::EDrawMode::TriangleFan, 0, 4);
	}

	void GLRectangle::setCoordColor(Zen::Color4f color, int index)
	{
		mColors[index] = color;
	}

	Zen::Color4f GLRectangle::getCoordColor(int index)
	{
		return mColors[index];
	}

	Rectangle * Rectangle::Create()
	{
		auto rect = new GLRectangle;
		return rect;
	}

}}

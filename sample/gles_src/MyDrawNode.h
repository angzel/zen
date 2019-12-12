#pragma once

#include <iostream>
#include <vector>
#include "zen_gles2.h"
#include "zen_vap2d.h"
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_ticker.h"
#include "zen_gles2_shader_color.h"
#include "zen_gles2_shader_sampler.h"

using namespace Zen::Vap2d;
using namespace Zen;

/**
 this is a custom node.
 */
class MyDrawNode : public LNode {

public:
	MyDrawNode()
	{
		this->setOrder(200);
	}

	virtual void draw() override
	{
		drawColor();
		drawSampler(40, 50);
		drawSampler(140, 100);
	}

	void drawSampler(float posx = 20, float posy = 10)
	{
		float coords[] = {
			0, 0,
			1, 0,
			0, 1,
			1, 1
		};
		using namespace Zen::GL;
		auto ss = ShaderSampler::GetAlpha();
		Render::ActiveProgram(ss->program.getObject());

		Render::BindArrayBuffer(0);
		Render::EnableVertexAttrib(ss->a_coord);
		Render::SetVertexAttribData(ss->a_coord, 2, EType::Float, false, 0, coords);

		Render::EnableVertexAttrib(ss->a_sampler_coord);
		Render::SetVertexAttribData(ss->a_sampler_coord, 2, EType::Float, true, 0, coords);

		Render::SetUniformFloat(ss->u_color, 1, 1, 1, 0.5f);

		static float radians = 0.3f;
		radians += 0.01f;
		float cos = cosf(radians);
		float sin = sinf(radians);
		float w = 80;
		float h = 80;
		float ax = 0.5f;
		float ay = 0.5f;
		Zen::Point2 pos = { posx, posy };

		auto mat1 = Zen::Matrix4{
			cos * w, sin * w, 0.0f, 0.0f,
			-sin * h, cos * h, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		mat1.m[12] = pos.x -mat1.m[0] * ax - mat1.m[4] * ay;
		mat1.m[13] = pos.y -mat1.m[1] * ax - mat1.m[5] * ay;

		auto mat_w = RenderStack::GetDefault()->getTopMatrix4();
		auto mat = Zen::Matrix4Multiply(*mat_w, mat1);
			//		Zen::Matrix4MakeTranslation(0, -1, 0);
		Render::SetUniformMat(ss->u_transform, mat);
//		auto texture = Textures::GetDefault()->getTexture("a.raw");
		auto texture = Textures::GetDefault()->getCrossTexture();
		Render::BindTexImage2D(texture->image2d().getObject(), 1);
		Render::SetUniformInt(ss->u_sampler, 1);

		Render::DrawArray(EDrawMode::TriangleStrip, 0, 4);
	}
	void drawColor()
	{
		float coords[] = {
			0, 0,
			1, 0,
			0, 1,
			1, 1
		};
		Zen::Color4f colors[] = {
			Zen::EColor::Blue,
			Zen::EColor::Red,
			Zen::EColor::Green,
			Zen::Color4f(1, 0.5f, 0.8f, 0.0f)
		};
		using namespace Zen::GL;
		auto sc = ShaderColor::GetNormal();

		Render::BindArrayBuffer(0);
		Render::ActiveProgram(sc->program.getObject());

		Render::EnableVertexAttrib(sc->a_coord);
		Render::SetVertexAttribData(sc->a_coord, 2, EType::Float, 0, 0, coords);
		Render::EnableVertexAttrib(sc->a_color);
		Render::SetVertexAttribData(sc->a_color, 4, EType::Float, 0, 0, colors);
		Render::SetUniformFloat(sc->u_point_size, 5);
		Render::SetUniformFloat(sc->u_color, 1, 1, 1, 1);

		static float radians = 0.3f;
		radians -= 0.01f;
		float cos = cosf(radians);
		float sin = sinf(radians);
		float x = 30;
		float y = 30;
		float ax = 0.5f;
		float ay = 0.5f;
		Zen::Point2 pos = { 130, 30 };

		auto mat1 = Zen::Matrix4{
			cos * x, sin * x, 0.0f, 0.0f,
			-sin * y, cos * y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		mat1.m[12] = pos.x -mat1.m[0] * ax - mat1.m[4] * ay;
		mat1.m[13] = pos.y -mat1.m[1] * ax - mat1.m[5] * ay;

		auto mat_w = RenderStack::GetDefault()->getTopMatrix4();
		auto mat = Zen::Matrix4Multiply(*mat_w, mat1);
		Render::SetUniformMat(sc->u_transform, mat);

		Render::DrawArray(EDrawMode::TriangleStrip, 0, 4);
	}
};


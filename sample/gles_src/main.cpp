//
//  main.cpp
//  gles_debug
//
//  Created by lover on 2019/12/4.
//  Copyright © 2019 lover. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include "zen_gles2_render.h"
#include "zen_color.h"
#include "zen_random.h"
#include "zen_app_runtime.h"
#include "zen_gles2_frame.h"
#include "zen_matrix.h"
#include "zen_gles2_shader_sampler.h"
#include "zen_gles2_shader_color.h"
#include "zen_gles2_texture_src.h"

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

class ScreenColorDelegate : public Zen::AppRuntimeDelegate {
	Zen::ColorGradient grad;
	Zen::Random r;
	float v = 0;

	Zen::GL::Texture tex;

	Zen::Matrix4 mat_w;

	bool touched = false;

public:
	virtual void onTouchDown(Zen::AppTouch const &) override
	{
		touched = true;
	}
	virtual void onTouchUp(Zen::AppTouch const &) override
	{
		touched = false;
	}
	virtual void onLaunch(Zen::Size2 const & size) override {
		r.reset((uint32_t)::time(0));
		Zen::Color4 c1, c2;
		c1.set(r.next());
		c2.set(r.next());
		grad.set(c1, c2);

		size_t len = 120*120;
		std::vector<uint8_t> buf(len);
		for(int i = 0; i < len; i += 1)
		{
			buf[i] = (i%120) + (i/120);
		}
		tex.create();
		tex.bindData(120, 120, Zen::GL::ETextureFmt::Alpha, buf.data(), 0);

		glEnable(GL_BLEND);
		float screen_x = 100;
		float screen_y = size.h / size.w * screen_x;
		mat_w = Zen::Matrix4MakeScale(1/screen_x, 1/screen_y, 1);
		using namespace Zen::GL;
		Render::SetBlendFunc(EBlendSrc::SrcAlpha, EBlendDst::OneMinusSrcAlpha);
	}
	virtual void onDraw() override {
		v += 0.004f;
		if(v > 1)
		{
			grad.set(grad.get1(), Zen::Color4(r.next()));
			v = 0;
		}
		auto c = grad.get(v);
		glClearColor(c.red, c.green, c.blue, c.alpha);
		glClear(GL_COLOR_BUFFER_BIT);

		this->drawSampler();
		this->drawColor();
	}
	void drawColor()
	{
		using namespace Zen::GL;
		auto sc = touched?ShaderColor::GetGrey():ShaderColor::GetNormal();
		Render::ActiveProgram(sc->program);
		Render::EnableVertexAttrib(sc->a_coord);
		Render::SetVertexAttribData(sc->a_coord, 2, EType::Float, 0, 0, coords);
		Render::EnableVertexAttrib(sc->a_color);
		Render::SetVertexAttribData(sc->a_color, 4, EType::Float, 0, 0, colors);
		Render::SetUniformFloat(sc->u_point_size, 5);
		Render::SetUniformFloat(sc->u_color, 1, 1, 1, 1);


		static float radians = 0.3f;
		radians += 0.01f;
		float cos = cosf(radians);
		float sin = sinf(radians);
		float x = 30;
		float y = 30;
		float ax = 0.5f;
		float ay = 0.5f;
		Zen::Point2 pos = { 30, 30 };

		auto mat1 = Zen::Matrix4{
			cos * x, sin * x, 0.0f, 0.0f,
			-sin * y, cos * y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };

		mat1.m[12] = pos.x -mat1.m[0] * ax - mat1.m[4] * ay;
		mat1.m[13] = pos.y -mat1.m[1] * ax - mat1.m[5] * ay;

		auto mat = Zen::Matrix4Multiply(mat_w, mat1);
		Render::SetUniformMat(sc->u_transform, mat);

		Render::DrawArray(EDrawMode::TriangleStrip, 0, 4);
	}
	void drawSampler()
	{
		using namespace Zen::GL;
		auto ss = touched?ShaderSampler::GetAlpha():ShaderSampler::GetNormal();
		Render::ActiveProgram(ss->program);
		Render::EnableVertexAttrib(ss->a_coord);
		Render::SetVertexAttribData(ss->a_coord, 2, EType::Float, false, 0, coords);

		Render::EnableVertexAttrib(ss->a_sampler_coord);
		Render::SetVertexAttribData(ss->a_sampler_coord, 2, EType::Float, true, 0, coords);

		Render::SetUniformFloat(ss->u_color, 1, 0, 0, 1);

		static float radians = 0.3f;
		radians += 0.01f;
		float cos = cosf(radians);
		float sin = sinf(radians);
		float x = 60;
		float y = 80;
		float ax = 0.5f;
		float ay = 0.5f;
		Zen::Point2 pos = { -50, -30 };

		auto mat1 = Zen::Matrix4{
			cos * x, sin * x, 0.0f, 0.0f,
			-sin * y, cos * y, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
			
		mat1.m[12] = pos.x -mat1.m[0] * ax - mat1.m[4] * ay;
		mat1.m[13] = pos.y -mat1.m[1] * ax - mat1.m[5] * ay;

		auto mat = Zen::Matrix4Multiply(mat_w, mat1);
			//		Zen::Matrix4MakeTranslation(0, -1, 0);
		Render::SetUniformMat(ss->u_transform, mat);

		Render::BindTexture(tex, 1);
		Render::SetUniformInt(ss->u_sampler, 1);

		Render::DrawArray(EDrawMode::TriangleStrip, 0, 4);

	}
};
int ZenAppMain(int argc, const char ** argv)
{
	auto runtime = Zen::AppRuntime::GetDefault();
	runtime->setRuntimeDelegate(new ScreenColorDelegate);
	return 0;
}

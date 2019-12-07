/*
 Copyright (c) 2013 ClearSky G.

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include "zen_gles2.h"

namespace Zen { namespace GL {
	class ShaderSampler
	{
	public:
		GLint a_coord;
		GLint a_sampler_coord;
		GLint u_transform;
		GLint u_sampler;
		GLint u_color;
		Zen::GL::Program program;
	public:
		static ShaderSampler const * GetNormal();
		// convert texture to grey channel
		static ShaderSampler const * GetGrey();
		// texture is alpha channel
		static ShaderSampler const * GetAlpha();

		static ShaderSampler * Create(char const * vertext_shader, char const * fragment_shader);
	protected:
		ShaderSampler() = default;
		ShaderSampler(ShaderSampler&) = delete;
	};

}}

/* usage example

		auto p = Zen::GL::ShaderSampler::GetNormal();
		p->active();
		using namespace Zen::GL;

		Render::EnableVertexAttrib(p->getAttribCoord());
		Render::SetVertexAttribData(p->getAttribCoord(), 4, EType::Float, false, 0, ____COORDS_DATA);

		Render::EnableVertexAttrib(p->getAttribSampleCoord());
		Render::SetVertexAttribData(p->getAttribSampleCoord(), 2, EType::Float, true, 0, ____TEXTURE_COORDS_DATA);

		float a = helper.getFinalOpacity();
		Render::SetUniformFloat(p->getUniformColor(), ___R, ___G, ___B, ___A);

		Render::SetUniformMat(p->getUniformTransform(), ___TRANS_MAT);

		Render::SetUniformInt(p->getUniformSample(), sam_number);

		Render::DrawArray(Zen::GL::EDrawMode::TriangleFan, 0, 4);

		*/

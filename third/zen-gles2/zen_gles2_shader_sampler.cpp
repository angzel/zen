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

#include "zen_gles2_shader_sampler.h"


namespace Zen { namespace GL {

	std::shared_ptr<ShaderSampler const> ShaderSampler::Create(char const * vertext_shader, char const * fragment_shader)
	{
		auto s = new ShaderSampler();
		s->program.makeAttachAndLink(vertext_shader, fragment_shader);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_sampler_coord = s->program.getAttributeLocation("a_sampler_coord");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_sampler = s->program.getUniformLocation("u_sampler");
		return std::shared_ptr<ShaderSampler const>(s);
	}
	
	static const char * vertex_shader =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute vec2 a_sampler_coord;"
	"uniform mat4 u_transform;"
	"varying vec2 v_sample_coord;"
	"void main()"
	"{"
	" v_sample_coord = a_sampler_coord;"
	" gl_Position = u_transform * a_coord;"
	"}";
	
	std::shared_ptr<ShaderSampler const> ShaderSampler::GetNormal()
	{
		static const char * fragment =
		"precision mediump float;"
		"uniform sampler2D u_sampler;"
		"uniform vec4 u_color;"
		"varying vec2 v_sample_coord;"
		"void main()"
		"{"
		" gl_FragColor = texture2D(u_sampler, v_sample_coord) * u_color;"
		"}";
		static auto me = Create(vertex_shader, fragment);
		return me;
	}
	
	std::shared_ptr<ShaderSampler const> ShaderSampler::GetGrey()
	{
		static const char * fragment =
		"precision mediump float;"
		"uniform sampler2D u_sampler;"
		"uniform vec4 u_color;"
		"varying vec2 v_sample_coord;"
		"const mediump vec3 grey = vec3(0.2125, 0.7154, 0.0721);"
		"void main()"
		"{"
		" vec4 c = texture2D(u_sampler, v_sample_coord);"
		" float i = dot(c.rgb, grey);"
		" gl_FragColor = vec4(i, i, i, c.w) * u_color;"
		"}";
		static auto me = Create(vertex_shader, fragment);
		return me;
	}
	std::shared_ptr<ShaderSampler const> ShaderSampler::GetAlpha()
	{
		static const char * fragment =
		"precision mediump float;"
		"uniform sampler2D u_sampler;"
		"uniform vec4 u_color;"
		"varying vec2 v_sample_coord;"
		"void main()"
		"{"
		" gl_FragColor = vec4(u_color.rgb, u_color.w * texture2D(u_sampler, v_sample_coord).w);"
		"}";
		static auto me = Create(vertex_shader, fragment);
		return me;
	}
}}


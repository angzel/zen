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

#include "zen_gles2_shader_color.h"


namespace Zen { namespace GL {

	std::shared_ptr<ShaderColor const> ShaderColor::Create(char const * vertext_shader, char const * fragment_shader)
	{
		auto s = new ShaderColor();
		s->program.makeAttachAndLink(vertext_shader, fragment_shader);
		s->a_coord = s->program.getAttributeLocation("a_coord");
		s->a_color = s->program.getAttributeLocation("a_color");
		s->u_transform = s->program.getUniformLocation("u_transform");
		s->u_color = s->program.getUniformLocation("u_color");
		s->u_point_size = s->program.getUniformLocation("u_point_size");
		return std::shared_ptr<ShaderColor const>(s);
	}
	
	static const char * vertex_shader =
	"precision mediump float;"
	"attribute vec4 a_coord;"
	"attribute vec4 a_color;"
	"uniform mat4 u_transform;"
	"uniform float u_point_size;"
	"varying vec4 v_color;"
	"void main()"
	"{"
	"   v_color = a_color;"
	"	gl_PointSize = u_point_size;"
	"	gl_Position = u_transform * a_coord;"
	"}";
	
	std::shared_ptr<ShaderColor const> ShaderColor::GetNormal()
	{
		static const char * fragment =
		"precision mediump float;"
		"varying vec4 v_color;"
		"uniform vec4 u_color;"
		"void main()"
		"{"
		"	gl_FragColor = v_color * u_color;"
		"}";
		static auto me = Create(vertex_shader, fragment);
		return me;
	}
	
	std::shared_ptr<ShaderColor const> ShaderColor::GetGrey()
	{
		static const char * fragment =
		"precision mediump float;"
		"varying vec4 v_color;"
		"uniform vec4 u_color;"
		"const mediump vec3 grey = vec3(0.2125, 0.7154, 0.0721);"
		"void main()"
		"{"
		" float i = dot(v_color.rgb, grey);"
		" gl_FragColor = vec4(i, i, i, v_color.w) * u_color;"
		"}";
		static auto me = Create(vertex_shader, fragment);
		return me;
	}
}}

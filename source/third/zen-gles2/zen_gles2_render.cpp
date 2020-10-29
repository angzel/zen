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

#include "zen_gles2_render.h"

namespace Zen { namespace GL {
	Render::Render()
	{

	}
	Render * Render::S()
	{
		static auto single = new Render;
		return single;;
	}
	inline void sLoadLineRange(float & min, float max)
	{
		GLfloat width[2];
		glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, width);
		min = width[0];
		max = width[1];
	}
	float Render::getMaxLineWidth()
	{
		if(m_max_line_width != 0) return m_max_line_width;
		sLoadLineRange(m_min_line_width, m_max_line_width);
		return m_max_line_width;
	}
	float Render::getMinLineWidth()
	{
		if(m_min_line_width != 0) return m_max_line_width;
		sLoadLineRange(m_min_line_width, m_max_line_width);
		return m_min_line_width;
	}
}}

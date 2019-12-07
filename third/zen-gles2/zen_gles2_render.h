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

#include "zen_exception.h"
#include "zen_color.h"
#include "zen_matrix.h"
#include "zen_gles2_enum.h"
#include "zen_gles2_program.h"
#include "zen_gles2_texture.h"
#include <map>

namespace Zen { namespace GL { namespace Render {
	// Program
	inline void ActiveProgram(Program const & program);
	inline void ActiveProgram(GLuint program_id);
	// texture
	inline void BindTexture(Texture const & texture, int);
//	// frame
//	inline void ActiveFrame(FrameBuffer const & frame);
//	inline void ActiveFrame(GLuint frame_id);
	// vertex.
	inline void EnableVertexAttrib(GLint attrib);

	inline void DisableVertexAttrib(GLint attrib);

	inline void SetVertexAttribData(GLint attrib, int size, EType type, bool normalize, int stride, void const * data);
	
	inline void SetVertexAttribBuffer(GLint attrib, int size, EType type, bool normalize, int stride, int off);

	inline void SetVertexAttribFloat(GLint loc, GLfloat valu);

	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1);

	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2);

	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

	// draw
	inline void DrawArray(EDrawMode mode, int first, uint32_t count);
	
	inline void DrawElements(EDrawMode mode, int count, uint8_t const * indices);
	
	inline void DrawElements(EDrawMode mode, int count, uint16_t const * indices);
	
	inline void DrawElements(EDrawMode mode, int count, uint32_t const * indices);
	
	// uniform
	inline void SetUniformInt(GLint unif, GLint value);
	
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1);
	
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1, GLint v2);
	
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1, GLint v2, GLint v3);
	
	inline void SetUniformFloat(GLint unif, GLfloat value);
	
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1);
	
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1, GLfloat v2);
	
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	
	inline void SetUniformMat(GLint unif, Matrix3 const & mat);
	
	inline void SetUniformMat(GLint unif, Matrix4 const & mat);

	// clear
	// ClearWithColor = SetClearColor + Clear
	inline void ClearWithColor(Zen::Color4f);
	
	inline void SetClearColor(Zen::Color4f);
	
	inline void Clear();
	
	// depth.
	inline void ClearDepthBuffer();
	
	inline void EnableDepthTest(bool);
	
	inline bool IsDepthTestEnabled();
	
	inline void SetDepthFunc(EDepthFunc);
	
	inline void SetClearDepth(float z);
	
	inline void EnableDepthMask(bool on);

	// filter.
	inline void SetTextureFilter(EFilter min, EFilter mag);
	
	inline void SetTextureWarp(EWarp x_warp, EWarp y_warp);
	
	inline void SetTextureFilter(GLint min, GLint mag);
	
	inline void SetTextureWarp(GLint x_warp, GLint y_warp);
	
	// blend.
	inline bool IsBlendEnable();
	
	inline void EnableBlend();
	
	inline void DisableBlend();
	
	inline void SetBlendFunc(EBlendSrc sf, EBlendDst df);
	
	inline void SetBlendColor(float r, float g, float b, float a);
	
	// face.
	inline void CullFace(bool front, bool back);

	inline void SetClockwiseAsFront(bool);
	
	// line.
	inline void SetLineWidth(float width);
	
	inline float GetLineWidth();

	inline std::pair<float, float> GetLineWidthRange();
}}}

//-------------------------------------------

namespace Zen { namespace GL { namespace Render {

	inline void ActiveProgram(Program const & program)
	{
		glUseProgram(program.getObject());
	}
	inline void ActiveProgram(GLuint program_id)
	{
		glUseProgram(program_id);
	}
	inline void BindTexture(Texture const & texture, int sampler)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + sampler));
		glBindTexture(GL_TEXTURE_2D, texture.getObject());
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to active texture", eno);
#endif
	}
	inline void UnbindTexture(int sampler)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + sampler));
		glBindTexture(GL_TEXTURE_2D, 0);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to active texture", eno);
#endif
	}

	inline void ActiveFrame(GLuint frame_id);

	inline void SetVertexAttribData(GLint attrib, int size, EType type, bool normalize, int stride, void const* ptr)
	{
		glVertexAttribPointer((GLuint)attrib, size, (GLenum)type, normalize, stride, ptr);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set vertex attrib array", eno);
#endif
	}
	inline void SetVertexAttribBuffer(GLint attrib, int size, EType type, bool normalize, int stride, int off)
	{
		glVertexAttribPointer((GLuint)attrib, size, (GLenum)type, normalize, stride, (char*)NULL + off);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set vertex attrib array", eno);
#endif
	}
	inline void DisableVertexAttrib(GLint attrib)
	{
		glDisableVertexAttribArray((GLuint)attrib);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "enable vertex attrib array error", eno);
#endif
	}
	inline void EnableVertexAttrib(GLint attrib)
	{
		glEnableVertexAttribArray((GLuint)attrib);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "enable vertex attrib array error", eno);
#endif
	}
	inline void DrawArray(EDrawMode mode, int first, uint32_t count)
	{
		glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "gl draw arry error", eno);
#endif
	}
	inline void DrawElements(EDrawMode mode, int count, uint8_t const * indices)
	{
		glDrawElements((GLenum)mode, count, (GLenum)EType::UByte, indices);
	}
	inline void DrawElements(EDrawMode mode, int count, uint16_t const * indices)
	{
		glDrawElements((GLenum)mode, count, (GLenum)EType::UShort, indices);
	}
	inline void DrawElements(EDrawMode mode, int count, uint32_t const * indices)
	{
		glDrawElements((GLenum)mode, count, (GLenum)EType::UInt, indices);
	}
	inline void SetUniformInt(GLint unif, GLint value)
	{
		glUniform1i(unif, value);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1)
	{
		glUniform2i(unif, v0, v1);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1, GLint v2)
	{
		glUniform3i(unif, v0, v1, v2);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformInt(GLint unif, GLint v0, GLint v1, GLint v2, GLint v3)
	{
		glUniform4i(unif, v0, v1, v2, v3);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformFloat(GLint unif, GLfloat value)
	{
		glUniform1f(unif, value);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1)
	{
		glUniform2f(unif, v0, v1);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		glUniform3f(unif, v0, v1, v2);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformFloat(GLint unif, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glUniform4f(unif, v0, v1, v2, v3);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	
	inline void SetUniformMat(GLint unif, Matrix3 const & mat)
	{
		glUniformMatrix3fv(unif, 1, GL_FALSE, (GLfloat const *)&mat);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetUniformMat(GLint unif, Matrix4 const & mat)
	{
		glUniformMatrix4fv(unif, 1, GL_FALSE, (GLfloat const *)&mat);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void SetVertexAttribFloat(GLint loc, GLfloat value)
	{
		glVertexAttrib1f((GLuint)loc, value);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1)
	{
		glVertexAttrib2f((GLuint)loc, v0, v1);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		glVertexAttrib3f((GLuint)loc, v0, v1, v2);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void SetVertexAttribFloat(GLint loc, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glVertexAttrib4f((GLuint)loc, v0, v1, v2, v3);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
}}}


namespace Zen { namespace GL { namespace Render {
	inline void ClearWithColor(Zen::Color4f color)
	{
		glClearColor(color.red, color.green, color.blue, color.alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	inline void SetClearColor(Zen::Color4f color)
	{
		glClearColor(color.red, color.green, color.blue, color.alpha);
	}
	inline void Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	inline void ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	inline void EnableDepthTest(bool on)
	{
		if(on) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}
	inline bool IsDepthTestEnable()
	{
		return glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	}
	inline void SetDepthFunc(EDepthFunc func)
	{
		glDepthFunc((GLenum)func);
	}
	inline void SetClearDepth(float z)
	{
		glClearDepthf(z);
	}
	inline void EnableDepthMask(bool on)
	{
		glDepthMask(on?GL_TRUE:GL_FALSE);
	}
	inline void SetTextureFilter(EFilter min, EFilter mag)
	{
		SetTextureFilter(GLint(min), GLint(mag));
	}
	inline void SetTextureWarp(EWarp x_warp, EWarp y_warp)
	{
		SetTextureWarp((GLint)x_warp, (GLint)y_warp);
	}
	inline void SetTextureFilter(GLint min, GLint mag)
	{
#if ZEN_DEBUG
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); 
		auto eno0 = (int)glGetError();
		mustsn(eno0 == GL_NO_ERROR, "Failed to set min filter", eno0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
		auto eno1 = (int)glGetError();
		mustsn(eno1 == GL_NO_ERROR, "Failed to set mag filter", eno1);
#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag); 
#endif
	}
	inline void SetTextureWarp(GLint x_warp, GLint y_warp)
	{
#if ZEN_DEBUG
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, x_warp); 
		auto eno0 = (int)glGetError();
		mustsn(eno0 == GL_NO_ERROR, "Failed to set warp_s", eno0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, y_warp);
		auto eno1 = (int)glGetError();
		mustsn(eno1 == GL_NO_ERROR, "Failed to set warp_t", eno1);
#else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, x_warp); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, y_warp);
#endif
	}
	inline void EnableBlend()
	{
		glEnable(GL_BLEND);
	}
	inline void DisableBlend()
	{
		glDisable(GL_BLEND);
	}
	inline bool IsBlendEnable()
	{
		return glIsEnabled(GL_BLEND) == GL_TRUE;
	}
	inline  void SetBlendColor(float r, float g, float b, float a)
	{
		glBlendColor(r, g, b, a);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set blend color", eno);
#endif
	}
	inline  void SetBlendFunc(EBlendSrc sf, EBlendDst df)
	{
		glBlendFunc((GLenum)sf, (GLenum)df);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set blend func", eno);
#endif
	}
	inline void CullFace(bool front, bool back)
	{
		if(!front && !back)
		{
			glDisable(GL_CULL_FACE);
		}
		else
		{
			glCullFace(front?(back?GL_FRONT_AND_BACK:GL_FRONT):GL_BACK);
			glEnable(GL_CULL_FACE);
		}

	}
	inline void SetClockwiseAsFront(bool f)
	{
		glFrontFace(f?GL_CW:GL_CCW);
	}
	inline  void ShowFaceCW()
	{
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to cull face (gl)", eno);
#endif
	}
	inline  void ShowFaceCCW()
	{
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to cull face (gl)", eno);
#endif
	}
	inline  void ShowFaceBoth()
	{
		glDisable(GL_CULL_FACE);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to cull face (gl)", eno);
#endif
	}
	inline  void ShowFaceNone()
	{
		glCullFace(GL_FRONT_AND_BACK);
		glEnable(GL_CULL_FACE);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to cull face (gl)", eno);
#endif
	}
	inline float GetLineWidth()
	{
		GLfloat width;
		glGetFloatv(GL_LINE_WIDTH, &width);
		return width;
	}
	inline void SetLineWidth(float width)
	{
		glLineWidth(width);
	}
	inline std::pair<float, float> GetLineWidthRange()
	{
		GLfloat width[2];
		glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, width);
		return std::pair<float, float>(width[0], width[1]);
	}
}}}

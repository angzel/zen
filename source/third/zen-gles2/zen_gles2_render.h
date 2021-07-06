/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_exception.h"
#include "zen_color.h"
#include "zen_matrix.h"
#include "zen_gles2_enum.h"
#include "zen_gles2_shader.h"
#include "zen_gles2_image2d.h"
#include "zen_gles2_buffer.h"
#include <map>

namespace Zen { namespace GL { 
	
	class Render {
	public:
		Render() = default;
		// ShaderProgram
		inline void activeProgram(GLuint program_id);
		
		// buffer
		inline void enableBuffer(GLint buffer);
		
		// vertex.
		//setVertexAttribData
		inline void setVertexData(GLint attrib, size_t size, eType type, bool normalize, size_t stride, void const * data);
		//setVertexAttribBuffer
		inline void setVertexBuffer(GLint attrib, size_t size, eType type, bool normalize, size_t stride, size_t off);
		
		inline void setVertex(GLint loc, GLfloat x);
		
		inline void setVertex(GLint loc, GLfloat x, GLfloat y);
		
		inline void setVertex(GLint loc, GLfloat x, GLfloat y, GLfloat z);
		
		inline void setVertex(GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		
		inline void setVertex(GLint loc, Vector4 const & v);
		inline void setVertex(GLint loc, Vector3 const & v);
		inline void setVertex(GLint loc, Vector2 const & v);

		/**
		 more
		 glEnableVertexAttribArray
		 glDisableVertexAttribArray
		 glVertexAttrib1f
		 glVertexAttrib2f
		 glVertexAttrib3f
		 glVertexAttrib4f
		 ...
		 glVertexAttrib4fv
		 */
		// uniform
		inline void setSampler(GLint unif, int sampler, GLuint texture);
		
		inline void setUniform(GLint unif, Matrix3 const & mat);
		
		inline void setUniform(GLint unif, Matrix4 const & mat);
		
		inline void setUniform(GLint unif, float x, float y, float z, float w);
		inline void setUniform(GLint unif, float x, float y, float z);
		inline void setUniform(GLint unif, float x, float y);
		inline void setUniform(GLint unif, float x);
		
		inline void setUniform(GLint unif, Vector4 const & v);
		inline void setUniform(GLint unif, Vector3 const & v);
		inline void setUniform(GLint unif, Vector2 const & v);
		
		/**
		 more
		 ...
		 glUniform3f
		 glUniform3fv
		 glUniform4f
		 glUniform4fv
		 ...
		 */
		
		// draw
		inline void drawArray(eVertexMode mode, size_t first, size_t count);
		
		inline void drawElements(eVertexMode mode, GLint buffer, size_t count, uint8_t const * indices);
		
		inline void drawElements(eVertexMode mode, GLint buffer, size_t count, uint16_t const * indices);
		
		inline void drawElements(eVertexMode mode, GLint buffer, size_t count, uint32_t const * indices);
		
		// clear
		// ClearWithColor = SetClearColor + Clear
		inline void clearWithColor(Zen::Color4f);
		
		inline void setClearColor(Zen::Color4f);
		
		inline void clear();
		
		// depth.
		inline void clearDepthBuffer();
		
		inline void enableDepthTest(bool);
		
		inline bool isDepthTestEnabled();
		
		inline void setDepthFunc(eDepthFunc);
		
		inline void setClearDepth(float z);
		
		inline void enableDepthMask(bool on);
		
		// blend.
		inline bool isBlendEnable();
		
		inline void enableBlend();
		
		inline void disableBlend();
		
		inline void setBlendFunction(eSrcBlend sf, eDstBlend df);
		
		inline void setBlendFunction(eSrcBlend sc, eDstBlend dc, eSrcBlend sa, eDstBlend da);
		
		inline void setBlendColor(float r, float g, float b, float a);
		
		// face.
		inline void cullFace(bool front, bool back);
		
		inline void setClockwiseAsFront(bool);
	};
}}

//-------------------------------------------

namespace Zen { namespace GL {
	
	inline void Render::activeProgram(GLuint program_id)
	{
		glUseProgram(program_id);
	}
	
	inline void Render::enableBuffer(GLint buffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	}
	//	inline void Render::activeFrame(GLuint frame_id)
	//	{
	//		throw __FUNCTION__;
	//	}
	
	inline void Render::setVertex(GLint loc, GLfloat value)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib1f((GLuint)loc, value);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertex(GLint loc, GLfloat x, GLfloat y)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib2f((GLuint)loc, x, y);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertex(GLint loc, GLfloat x, GLfloat y, GLfloat z)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib3f((GLuint)loc, x, y, z);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertex(GLint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib4f((GLuint)loc, x, y, z, w);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	
	inline void Render::setVertex(GLint loc, Vector4 const & v)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib4f((GLuint)loc, v.x, v.y, v.z, v.w);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertex(GLint loc, Vector3 const & v)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib3f((GLuint)loc, v.x, v.y, v.z);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertex(GLint loc, Vector2 const & v)
	{
		glDisableVertexAttribArray((GLuint)loc);
		
		glVertexAttrib2f((GLuint)loc, v.x, v.y);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl vertex attrib", eno);
#endif
	}
	inline void Render::setVertexData(GLint attrib, size_t size, eType type, bool normalize, size_t stride, void const* ptr)
	{
		glEnableVertexAttribArray((GLuint)attrib);
		glVertexAttribPointer((GLuint)attrib, (GLint)size, (GLenum)type, (GLboolean)normalize, (GLint)stride, ptr);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set vertex attrib array", eno);
#endif
	}
	inline void Render::setVertexBuffer(GLint attrib, size_t size, eType type, bool normalize, size_t stride, size_t off)
	{
		glEnableVertexAttribArray((GLuint)attrib);
		glVertexAttribPointer((GLuint)attrib, (GLint)size, (GLenum)type, normalize, (GLint)stride, (char*)NULL + off);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set vertex attrib array", eno);
#endif
	}
	
	inline void Render::drawArray(eVertexMode mode, size_t first, size_t count)
	{
		glDrawArrays((GLenum)mode, (GLint)first, (GLsizei)count);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "gl draw arry error", eno);
#endif
	}
	inline void Render::drawElements(eVertexMode mode, GLint buffer, size_t count, uint8_t const * indices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glDrawElements((GLenum)mode, (GLint)count, (GLenum)eType::UByte, indices);
	}
	inline void Render::drawElements(eVertexMode mode, GLint buffer, size_t count, uint16_t const * indices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glDrawElements((GLenum)mode, (GLint)count, (GLenum)eType::UShort, indices);
	}
	inline void Render::drawElements(eVertexMode mode, GLint buffer, size_t count, uint32_t const * indices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glDrawElements((GLenum)mode, (GLint)count, (GLenum)eType::UInt, indices);
	}
	inline void Render::setSampler(GLint unif, int sampler, GLuint texture)
	{
		glActiveTexture(GLenum(GL_TEXTURE0 + sampler));
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(unif, sampler);
		
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to active texture", eno);
#endif
	}
	
	inline void Render::setUniform(GLint unif, Matrix3 const & mat)
	{
		glUniformMatrix3fv(unif, 1, GL_FALSE, (GLfloat const *)&mat);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, Matrix4 const & mat)
	{
		glUniformMatrix4fv(unif, 1, GL_FALSE, (GLfloat const *)&mat);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, float x)
	{
		glUniform1f(unif, x);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, float x, float y)
	{
		glUniform2f(unif, x, y);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, float x, float y, float z)
	{
		glUniform3f(unif, x, y, z);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, float x, float y, float z, float w)
	{
		glUniform4f(unif, x, y, z, w);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, Vector4 const & v)
	{
		glUniform4f(unif, v.x, v.y, v.z, v.w);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, Vector3 const & v)
	{
		glUniform3f(unif, v.x, v.y, v.z);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
	inline void Render::setUniform(GLint unif, Vector2 const & v)
	{
		glUniform2f(unif, v.x, v.y);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set gl uniform", eno);
#endif
	}
}}


namespace Zen { namespace GL {
	inline void Render::clearWithColor(Zen::Color4f color)
	{
		glClearColor(color.red, color.green, color.blue, color.alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	inline void Render::setClearColor(Zen::Color4f color)
	{
		glClearColor(color.red, color.green, color.blue, color.alpha);
	}
	inline void Render::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	inline void Render::clearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	inline void Render::enableDepthTest(bool on)
	{
		if(on) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
	}
	inline bool Render::isDepthTestEnabled()
	{
		return glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	}
	inline void Render::setDepthFunc(eDepthFunc func)
	{
		glDepthFunc((GLenum)func);
	}
	inline void Render::setClearDepth(float z)
	{
		glClearDepthf(z);
	}
	inline void Render::enableDepthMask(bool on)
	{
		glDepthMask(on?GL_TRUE:GL_FALSE);
	}
	inline void Render::enableBlend()
	{
		glEnable(GL_BLEND);
	}
	inline void Render::disableBlend()
	{
		glDisable(GL_BLEND);
	}
	inline bool Render::isBlendEnable()
	{
		return glIsEnabled(GL_BLEND) == GL_TRUE;
	}
	inline  void Render::setBlendColor(float r, float g, float b, float a)
	{
		glBlendColor(r, g, b, a);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set blend color", eno);
#endif
	}
	inline void Render::setBlendFunction(eSrcBlend sf, eDstBlend df)
	{
		glBlendFunc((GLenum)sf, (GLenum)df);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set blend func", eno);
#endif
	}
	inline void Render::setBlendFunction(eSrcBlend sc, eDstBlend dc, eSrcBlend sa, eDstBlend da)
	{
		glBlendFuncSeparate((GLenum)sc, (GLenum)dc, (GLenum)sa, (GLenum)da);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to set blend func separate", eno);
#endif
	}
	
	inline void Render::cullFace(bool front, bool back)
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
	inline void Render::setClockwiseAsFront(bool f)
	{
		glFrontFace(f?GL_CW:GL_CCW);
	}
	
}}

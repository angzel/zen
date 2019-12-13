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

#include "zen_gles2_enum.h"
#include <memory>
#include <vector>

namespace Zen { namespace GL {
	enum class ETextureFmt
	{
		Alpha  = GL_ALPHA,
		RGB    = GL_RGB,
		RGBA   = GL_RGBA,
	};

	class TexImage2D
	{
	private:
		TexImage2D(TexImage2D const &);
		
		void operator=(TexImage2D const &);
		
	public:
		inline TexImage2D();
		
		inline ~TexImage2D();
		
		inline void create();
		
		inline void release();
		
		inline void bindData(uint32_t width, uint32_t height, EBPP bytes, void const * data, int level);

		inline GLuint getObject() const;
		
	protected:
		GLuint mID;
	};
}}

	/// class TexImage2D
namespace Zen { namespace GL {
	inline TexImage2D::TexImage2D()
	{
		mID = 0;
	}
	inline TexImage2D::~TexImage2D()
	{
		this->release();
	}
	inline GLuint TexImage2D::getObject() const
	{
		return mID;
	}
	inline void TexImage2D::release()
	{
		if(mID == 0) return;
		glDeleteTextures(1, &mID);
		mID = 0;
	}
	inline void TexImage2D::create()
	{
		if(mID != 0) return;
		glGenTextures (1, &mID);
#if ZEN_DEBUG
		mustsn(mID != 0, "failed to generate gl texture", (int)glGetError());
#endif
	}
	inline void TexImage2D::bindData(uint32_t width, uint32_t height, EBPP bpp, void const * data, int level)
	{
		int i = (int)bpp;
		GLint format = (i==1?GL_ALPHA:(i==3?GL_RGB:(i==4?GL_RGBA:0)));
		musts(format, "invalid format");

		glPixelStorei(GL_UNPACK_ALIGNMENT, (int)bpp==4?4:1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, level, (GLint)format, (int)width, (int)height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to bind data to gl texture", eno);
#endif
	}
} }

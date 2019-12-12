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
#include "zen_gles2_image2d.h"

namespace Zen {
	namespace GL {
		class FrameBuffer
		{
			GLuint mID;
			GLuint mTexture;
			GLuint mDepth;
		public:
			FrameBuffer();
			
			~FrameBuffer();
			
			void create(uint32_t width, uint32_t height, ETextureFmt fmt, int lvl);
			
			void bindTexture(GLuint texture);
			
			void release();
			
			void active();
			
			static void ActiveNone();
			
			Vector2 getSize();
			
			bool isValid();
		};
	}
}



namespace Zen { namespace GL {
	
	inline FrameBuffer::FrameBuffer()
	{
		mID = 0;
		mTexture = 0;
		mDepth = 0;
	}
	
	inline FrameBuffer::~FrameBuffer()
	{
		release();
	}
	
	inline void FrameBuffer::create(uint32_t width, uint32_t height, ETextureFmt fmt, int lvl)
	{
		release();
		//
		glGenRenderbuffers(1, &mDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, (GLsizei)width, (GLsizei)height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		glGenFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepth);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to create frame buffer", eno);
#endif
	}
	inline void FrameBuffer::bindTexture(GLuint texture)
	{
		mTexture = texture;
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	inline void FrameBuffer::release()
	{
		if(mID) glDeleteFramebuffers(1, &mID);
		if(mDepth) glDeleteRenderbuffers(1, &mDepth);
		mID = 0;
		mTexture = 0;
		mDepth = 0;
	}
	inline void FrameBuffer::active()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}
	inline void FrameBuffer::ActiveNone()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline Vector2 FrameBuffer::getSize()
	{
		GLint width, height;
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to get size of frame buffer", eno);
#endif
		return Vector2Make((float)width, (float)height);
	}
	inline bool FrameBuffer::isValid()
	{
		if(mID != 0 && mDepth != 0)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mID);
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return (status == GL_FRAMEBUFFER_COMPLETE);
		}
		return false;
	}
} }

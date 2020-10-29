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
#include "zen_exception.h"

namespace Zen { namespace GL {
	enum class EBufferUsage
	{
		StreamDraw          = GL_STREAM_DRAW,
		StaticDraw          = GL_STATIC_DRAW, 
		DynamicDraw         = GL_DYNAMIC_DRAW,
		
	};
	enum class EBufferType
	{
		ArrayBuffer         = GL_ARRAY_BUFFER,
		ElementArrayBuffer  = GL_ELEMENT_ARRAY_BUFFER,
	};

	class Buffer
	{
	private:
		Buffer(Buffer const &);
		
		void operator = (Buffer const &);
		
	protected:
		inline Buffer(GLenum type);

		inline ~Buffer(void);
	public:

		inline void create(size_t size, void const * data_null, EBufferUsage usage = EBufferUsage::StaticDraw);

		inline void update(size_t offset, size_t size, void const * data);
		
		inline EBufferUsage getUsage() const;
		
		inline uint32_t size() const;
		
		inline bool isValid() const;
		
		inline uint32_t getID() const;

	protected:
		uint32_t mID;
		GLenum mType;
		
	};

	class ArrayBuffer : public Buffer {
	public:
		ArrayBuffer()
		: Buffer(GL_ARRAY_BUFFER)
		{
		}
	};

	class ElementArrayBuffer : public Buffer {
	public:
		ElementArrayBuffer();
	};
	
}}


/// class buffer
namespace Zen { namespace GL {

	inline Buffer::Buffer(GLenum type)
	{
		mType = type;
		mID = 0;
		glGenBuffers(1, &mID);
	}

	inline Buffer::~Buffer(void)
	{
		if(mID != 0) glDeleteBuffers(1, &mID);
	}

	inline void Buffer::create(size_t size, void const * data_null, EBufferUsage usage)
	{
		glBindBuffer((GLenum)mType, mID);
		glBufferData((GLenum)mType, (GLsizeiptr)size, data_null, (GLenum)usage);
#if ZEN_DEBUG
		GLenum eno = glGetError();
		mustsn(eno == GL_NO_ERROR, "set gl buffer data failed", (int)eno);
#endif
	}

	inline void Buffer::update(size_t offset, size_t size, void const * data)
	{
		glBindBuffer((GLenum)mType, mID);
		glBufferSubData((GLenum)mType, (GLintptr)offset, (GLsizeiptr)size, data);
#if ZEN_DEBUG
		GLenum eno = glGetError();
		mustsn(eno == GL_NO_ERROR, "set gl buffer sub data failed", (int)eno);
#endif
	}

	inline EBufferUsage Buffer::getUsage() const
	{
		GLint usage = GL_STATIC_DRAW;
		glBindBuffer((GLenum)mType, mID);
		glGetBufferParameteriv((GLenum)mType, GL_BUFFER_USAGE, &usage);
		return (EBufferUsage)usage;
	}

	inline uint32_t Buffer::size() const
	{
		GLint sz = 0;
		glBindBuffer((GLenum)mType, mID);
		glGetBufferParameteriv((GLenum)mType, GL_BUFFER_SIZE, &sz);
		return (uint32_t)sz;
	}

	inline bool Buffer::isValid() const
	{
		return mID != 0;
	}

	inline uint32_t Buffer::getID() const
	{
		return mID;
	}
}}

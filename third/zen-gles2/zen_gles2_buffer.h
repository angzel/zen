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
	template<GLenum tBufferType>
	class Buffer
	{
	private:
		Buffer(Buffer const &);
		
		void operator = (Buffer const &);
		
	public:
		static const GLenum oType = tBufferType;
		
		inline static void ActiveNone();
		
	public:
		inline Buffer();
		
		inline ~Buffer(void);
		
		inline void create();
		
		inline void release();
		
		inline void bindData(uint32_t size, void const * data, EBufferUsage bindType = EBufferUsage::StaticDraw);
		
		inline void updateData(uint32_t offset, uint32_t size, void const * data);
		
		inline EBufferUsage usage() const;
		
		inline uint32_t size() const;
		
		inline void active() const;
		
		inline bool isValid() const;
		
		inline uint32_t getObject() const;
		
	protected:
		uint32_t mID;
		
	};
	
	typedef Buffer<GL_ARRAY_BUFFER> ArrayBuffer;
	
	typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> ElementArrayBuffer;
	
}}


/// class buffer
namespace Zen { namespace GL {
	
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::ActiveNone()
	{
		glBindBuffer(oType, 0);
	}
	template<GLenum tBufferType>
	inline Buffer<tBufferType>::Buffer()
	{
		mID = 0;
	}
	template<GLenum tBufferType>
	inline Buffer<tBufferType>::~Buffer(void)
	{
		if(mID != 0) glDeleteBuffers(1, &mID);
	}
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::release()
	{
		if(mID == 0) return;
		glDeleteBuffers(1, &mID);
		mID = 0;
	}
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::create()
	{
		if(mID == 0) glGenBuffers(1, &mID);
#if ZEN_DEBUG
		mustsn(mID != 0, "failed to create gl buffer", (int)glGetError());
#endif
	}
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::bindData(uint32_t size, void const * data, EBufferUsage bindType)
	{
		create();
		glBindBuffer(oType, mID);
		glBufferData(oType, size, data, (GLenum)bindType);
#if ZEN_DEBUG
		GLenum eno = glGetError();
		mustsn(eno == GL_NO_ERROR, "set gl buffer data failed", (int)eno);
#endif
	}
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::updateData(uint32_t offset, uint32_t size, void const * data)
	{
		glBindBuffer(oType, mID);
		glBufferSubData(oType, offset, size, data);
#if ZEN_DEBUG
		GLenum eno = glGetError();
		mustsn(eno == GL_NO_ERROR, "set gl buffer sub data failed", (int)eno);
#endif
	}
	template<GLenum tBufferType>
	inline EBufferUsage Buffer<tBufferType>::usage() const
	{
		GLint usage = GL_STATIC_DRAW;
		glBindBuffer(oType, mID);
		glGetBufferParameteriv(oType, GL_BUFFER_USAGE, &usage);
		return (EBufferUsage)usage;
	}
	template<GLenum tBufferType>
	inline uint32_t Buffer<tBufferType>::size() const
	{
		GLint sz = 0;
		glBindBuffer(oType, mID);
		glGetBufferParameteriv(oType, GL_BUFFER_SIZE, &sz);
		return (uint32_t)sz;
	}
	template<GLenum tBufferType>
	inline void Buffer<tBufferType>::active() const
	{
		glBindBuffer(oType, mID);
	}
	template<GLenum tBufferType>
	inline bool Buffer<tBufferType>::isValid() const
	{
		return mID != 0;
	}
	template<GLenum tBufferType>
	inline uint32_t Buffer<tBufferType>::getObject() const
	{
		return mID;
	}
}}

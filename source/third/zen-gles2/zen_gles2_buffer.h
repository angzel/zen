/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_gles2_enum.h"
#include "zen_exception.h"

namespace Zen { namespace GL {
	enum class eBufferUsage
	{
		StreamDraw          = GL_STREAM_DRAW,
		StaticDraw          = GL_STATIC_DRAW, 
		DynamicDraw         = GL_DYNAMIC_DRAW,
		
	};
	enum class eBufferType
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

		inline void create(size_t size, void const * data_null, eBufferUsage usage = eBufferUsage::StaticDraw);

		inline void update(size_t offset, size_t size, void const * data);
		
		inline eBufferUsage getUsage() const;
		
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

	inline void Buffer::create(size_t size, void const * data_null, eBufferUsage usage)
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

	inline eBufferUsage Buffer::getUsage() const
	{
		GLint usage = GL_STATIC_DRAW;
		glBindBuffer((GLenum)mType, mID);
		glGetBufferParameteriv((GLenum)mType, GL_BUFFER_USAGE, &usage);
		return (eBufferUsage)usage;
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

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
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

	class Texture2DBuffer
	{
	private:
		Texture2DBuffer(Texture2DBuffer const &);
		
		void operator=(Texture2DBuffer const &);
		
	public:
		inline Texture2DBuffer();
		
		inline ~Texture2DBuffer();

		inline void create(size_t width, size_t height, eFilter filter, ePixel bytes);

		inline void createWithData(size_t width, size_t height, eFilter filter, ePixel bytes, void const * data);

		inline void fill(int xoff, int yoff, size_t width, size_t height, const void *data);

		inline GLuint getID() const;
		
	protected:
		GLuint mID;
		GLint m_format = 0;
	};
}}

	/// class Texture2DBuffer
namespace Zen { namespace GL {
	inline Texture2DBuffer::Texture2DBuffer()
	{
		glGenTextures (1, &mID);
	}
	inline Texture2DBuffer::~Texture2DBuffer()
	{
		glDeleteTextures(1, &mID);
	}
	inline GLuint Texture2DBuffer::getID() const
	{
		return mID;
	}
	inline void Texture2DBuffer::create(size_t width, size_t height, eFilter filter, ePixel bpp)
	{
		GLint format = ((int)bpp==1?GL_ALPHA:((int)bpp==3?GL_RGB:((int)bpp==4?GL_RGBA:0)));

		musts(format, "invalid format");
		m_format = format;

		glPixelStorei(GL_UNPACK_ALIGNMENT, m_format == GL_RGBA?4:1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, (GLsizei)width, (GLsizei)height,
					 0, (GLenum)format, GL_UNSIGNED_BYTE, nullptr);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to bind data to gl texture", eno);
#endif
	}
	inline void Texture2DBuffer::createWithData(size_t width, size_t height, eFilter filter,  ePixel bpp, void const * data)
	{
		GLint format = ((int)bpp==1?GL_ALPHA:((int)bpp==3?GL_RGB:((int)bpp==4?GL_RGBA:0)));

		musts(format, "invalid format");
		m_format = format;

		glPixelStorei(GL_UNPACK_ALIGNMENT, m_format == GL_RGBA?4:1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, (GLsizei)width, (GLsizei)height,
					 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
	}
	inline void Texture2DBuffer::fill(int xoff, int yoff, size_t width, size_t height, const void *data)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, m_format == GL_RGBA?4:1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)xoff, (GLint)yoff,
						(GLsizei)width, (GLsizei)height, (GLenum)m_format, GL_UNSIGNED_BYTE,  data);
#if ZEN_DEBUG
		auto eno = (int)glGetError();
		mustsn(eno == GL_NO_ERROR, "failed to bind data to gl texture", eno);
#endif
	}
} }

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
	class Texture
	{
	private:
		Texture(Texture const &);
		
		void operator=(Texture const &);
		
	public:
		inline Texture();
		
		inline ~Texture();
		
		inline void create();
		
		inline void release();
		
		inline void bindData(uint32_t width, uint32_t height, ETextureFmt format, void const * data, int level);

		inline GLuint getObject() const;
		
	protected:
		GLuint mID;
	};
	class TextureSrc {
	public:
		Texture texture;
		Zen::Size2 size;
		Zen::Size2 real_size;
		Zen::Size2 gl_size;
		ETextureFmt format;

		TextureSrc() = default;
		TextureSrc(TextureSrc&) = delete;
	};
	typedef std::shared_ptr<TextureSrc const> SharedTexture;

	inline SharedTexture Create(uint32_t width, uint32_t height, ETextureFmt format, void const * data);
}}

	/// class Texture
namespace Zen { namespace GL {
	inline Texture::Texture()
	{
		mID = 0;
	}
	inline Texture::~Texture()
	{
		this->release();
	}
	inline GLuint Texture::getObject() const
	{
		return mID;
	}
	inline void Texture::release()
	{
		if(mID == 0) return;
		glDeleteTextures(1, &mID);
		mID = 0;
	}
	inline void Texture::create()
	{
		if(mID != 0) return;
		glGenTextures (1, &mID);
#if ZEN_DEBUG
		mustsn(mID != 0, "failed to generate gl texture", (int)glGetError());
#endif
	}
	inline void Texture::bindData(uint32_t width, uint32_t height, ETextureFmt format, void const * data, int level)
	{
		this->create();

		switch (format)
		{
			case ETextureFmt::Alpha:
			case ETextureFmt::RGB:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				break;
			case ETextureFmt::RGBA:
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				break;
		}
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
	inline uint32_t GetTextureExtendSize(uint32_t s)
	{
		for (int i = 0; i < 16; ++i) {
			if (s < (1U << i)) return (1U << i);
		}
		return 0;
	}
	inline SharedTexture CreateSharedTexture(uint32_t width, uint32_t height, ETextureFmt format, void const * data)
	{
			//#if ZEN_GL_2D_ANY_SZ
		auto t = new TextureSrc;
		t->format = format;
		t->texture.create();
		t->size = { (float)width, (float)height };
#if 1
		t->texture.bindData(width, height, format, data, 0);
		t->real_size = t->size;
		t->gl_size = { 1.f, 1.f };
#else
		int bpp = (ETextureFmt::Alpha == format?1:(ETextureFmt::RGB == format?3:4));

		auto w = GetTextureExtendSize(width);
		auto h = GetTextureExtendSize(height);

		if(w == width)
		{
			if(h == height)
			{
				t->texture.bindData(width, height, format, data, 0);
			}
			else
			{
				auto src_size = width * height * bpp;
				auto dest_size = w * h * bpp;
				std::vector<char> buf(dest_size, 0);
				::memcpy(buf.data(), data, src_size);
				t->texture.bindData(w, h, format, buf.data(), 0);
			}
		}
		else
		{
			auto dest_stride = bpp * w;
			auto src_stride = bpp * width;
			auto dest_size = dest_stride * h;

			std::vector<char> buf;
			buf.resize(dest_size, 0);

			auto src = (char const *)data;
			auto dest = buf.data();

			for (int i = 0; i < height; ++i) {
				::memcpy(dest, src, src_stride);
				dest += dest_stride;
				src += src_stride;
			}
			t->texture.bindData(w, h, format, buf.data(), 0);
		}
		t->real_size = {(float)w, (float)h};
		t->gl_size = {(float)width /(float)w, (float)height /(float)h};
#endif
		return SharedTexture(t);
	}
} }

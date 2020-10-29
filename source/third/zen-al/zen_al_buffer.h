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
#include "zen_al_platform.h"
#include "zen_exception.h"

namespace Zen { namespace AL {
	enum class EBufferFmt
	{
		Mono8 = AL_FORMAT_MONO8,
		Mono16 = AL_FORMAT_MONO16,
		Stereo8 = AL_FORMAT_STEREO8,
		Stereo16 = AL_FORMAT_STEREO16,
	};
	class Buffer
	{
	protected:
		ALuint mBuffer;
		
		Buffer(Buffer const &);
		
		void operator = (Buffer const &);
		
	public:
		inline Buffer();
		
		inline ~Buffer();
		
		inline void create();
		
		inline void release();
		
		inline void bindData(EBufferFmt fmt, void *data, uint32_t size, uint32_t freq) const;
		
		inline ALuint getALBufferID() const;
	};
}}


namespace Zen { namespace AL {
	inline Buffer::Buffer()
	{
		mBuffer = 0;
	}
	inline Buffer::~Buffer()
	{
		release();
	}
	inline void Buffer::create()
	{
		if(mBuffer != 0) return;
		alGenBuffers(1, &mBuffer);
		musts(mBuffer != 0, "failed to create openal buffer");
	}
	inline void Buffer::release()
	{
		if(mBuffer != 0)
		{
			alDeleteBuffers(1, &mBuffer);
			mBuffer = 0;
		}
	}
	inline void Buffer::bindData(EBufferFmt fmt, void *data, uint32_t size, uint32_t freq) const
	{
		alBufferData(mBuffer, (ALenum)fmt, data, (int)size, (int)freq);
	}
	inline ALuint Buffer::getALBufferID() const
	{
		return mBuffer;
	}
}}

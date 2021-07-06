/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once
#include "zen_al_platform.h"
#include "zen_exception.h"

namespace Zen { namespace AL {
	enum class eTrackType
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
		
		inline void bindData(eTrackType fmt, void *data,size_t size, size_t freq) const;
		
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
	inline void Buffer::bindData(eTrackType fmt, void *data, size_t size, size_t freq) const
	{
		alBufferData(mBuffer, (ALenum)fmt, data, (int)size, (int)freq);
	}
	inline ALuint Buffer::getALBufferID() const
	{
		return mBuffer;
	}
}}

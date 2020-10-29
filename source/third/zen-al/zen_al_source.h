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

namespace Zen { namespace AL {

	enum class EPlayState {
		Inital = AL_INITIAL,
		Playing = AL_PLAYING,
		Paused = AL_PAUSED,
		Stoped = AL_STOPPED,
	};
	struct Position {
		float x, y, z;
	};

	class Source
	{
	protected:
		ALuint  mSource;
		Source(Source const &);
		void operator = (Source const &);
	public:
		Source();
		
		~Source();
		
		void create();
		
		void release();
		
		void setPitch(float value) const;
		
		void setGain(float value) const;
		
		void setPosition(Position) const;
		
		void setVelocity(Position) const;
		
		void setLooping(bool loop) const;
		
		float getPitch() const;
		
		float getGain() const;
		
		Position getPosition() const;
		
		Position getVelocity() const;
		
		bool isLooping() const;
		
		void bindSound(ALuint bufferID) const;
		
		ALuint getALSourceID() const;
		
		EPlayState getPlayState() const;

		bool isFree() const
		{
			auto state = this->getPlayState();
			return (state == EPlayState::Inital || state == EPlayState::Stoped);
		}
	};
}}


namespace Zen { namespace AL {
	inline Source::Source()
	{
		mSource = 0;
	}
	inline Source::~Source()
	{
		release();
	}
	inline void Source::create()
	{
		if(mSource != 0) return;
		alGenSources(1, &mSource);
		musts(mSource != 0, "failed to create openal source");
	}
	inline void Source::release()
	{
		if(mSource != 0)
		{
			alDeleteSources(1, &mSource);
			mSource = 0;
		}
	}
	inline void Source::setPitch(float value) const
	{
		alSourcef(mSource, AL_PITCH, value);
	}
	inline void Source::setGain(float value) const
	{
		alSourcef(mSource, AL_GAIN, value);
	}
	inline void Source::setPosition(Position pos) const
	{
		alSource3f(mSource, AL_POSITION, pos.x, pos.y, pos.z);
	}
	inline void Source::setVelocity(Position motion) const
	{
		alSource3f(mSource, AL_VELOCITY, motion.x, motion.y, motion.z);
	}
	inline void Source::setLooping(bool loop) const
	{
		alSourcei(mSource, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
	}
	inline float Source::getPitch() const
	{
		ALfloat value = 0.f;
		alGetSourcef(mSource, AL_PITCH, &value);
		return value;
	}
	inline float Source::getGain() const
	{
		ALfloat value = 0.f;
		alGetSourcef(mSource, AL_PITCH, &value);
		return value;
	}
	inline Position Source::getPosition() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(mSource, AL_POSITION, &x, &y, &z);
		return Position{x, y, z};
	}
	inline Position Source::getVelocity() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(mSource, AL_VELOCITY, &x, &y, &z);
		return Position{x, y, z};
	}
	inline bool Source::isLooping() const
	{
		ALint value;
		alGetSourcei(mSource, AL_LOOPING, &value);
		return value == AL_TRUE;
	}
	inline void Source::bindSound(ALuint buffer) const
	{
		alSourcei(mSource, AL_BUFFER, (ALint)buffer);
	}
	inline ALuint Source::getALSourceID() const
	{
		return mSource;
	}
	inline EPlayState Source::getPlayState() const
	{
		ALint state;
		alGetSourcei(mSource, AL_SOURCE_STATE, &state);
		return (EPlayState)state;
	}
}}

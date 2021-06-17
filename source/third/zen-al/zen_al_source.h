/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once
#include "zen_al_platform.h"
#include "zen_types.h"

namespace Zen { namespace AL {

	enum class EPlayState {
		Inital = AL_INITIAL,
		Playing = AL_PLAYING,
		Paused = AL_PAUSED,
		Stoped = AL_STOPPED,
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
		
		void setPosition(Point3) const;
		
		void setVelocity(Point3) const;
		
		void setLooping(bool loop) const;
		
		float getPitch() const;
		
		float getGain() const;
		
		Point3 getPosition() const;
		
		Point3 getVelocity() const;
		
		bool isLooping() const;
		
		void bindSound(ALuint bufferID) const;
		
		ALuint getALSourceID() const;
		
		EPlayState getPlayState() const;
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
	inline void Source::setPosition(Point3 pos) const
	{
		alSource3f(mSource, AL_POSITION, pos.x, pos.y, pos.z);
	}
	inline void Source::setVelocity(Point3 motion) const
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
		alGetSourcef(mSource, AL_GAIN, &value);
		return value;
	}
	inline Point3 Source::getPosition() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(mSource, AL_POSITION, &x, &y, &z);
		return Point3{x, y, z};
	}
	inline Point3 Source::getVelocity() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(mSource, AL_VELOCITY, &x, &y, &z);
		return Point3{x, y, z};
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

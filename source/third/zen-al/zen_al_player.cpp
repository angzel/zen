/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_al_player.h"
#include "zen_exception.h"

namespace Zen { namespace AL {
	Player * Player::Get()
	{
		static Player * me = new Player();
		return me;
	}
	Player::Player()
	{
		ALCcontext * context = nullptr;
		ALCdevice * device = alcOpenDevice(NULL); // open default device
		if (device == nullptr) throws("failed to open sound device");
		context = alcCreateContext(device, nullptr); // create context
		if (context == nullptr)
		{
			ALenum eno = alcGetError(device);
			alcCloseDevice(device);
			throwsn("failed to create audio context", eno);
		}
		if(alcMakeContextCurrent(context) != AL_TRUE)
		{
			ALenum eno = alcGetError(device);
			alcDestroyContext(context);
			alcCloseDevice(device);
			throwsn("failed to set audio context", eno);
		}
	}
	Player::~Player()
	{
		ALCdevice * device = getCurrentDevice();
		alcDestroyContext(getCurrentContext());
		alcCloseDevice(device);
	}
}}

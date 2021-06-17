/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once
#include "zen_al_platform.h"

namespace Zen { namespace AL {
	class Player
	{
	protected:
		Player();
	public:
		static Player * GetDefault();

		~Player();

		inline ALCdevice * getCurrentDevice()
		{
			return alcGetContextsDevice(alcGetCurrentContext());
		}

		inline ALCcontext * getCurrentContext()
		{
			return alcGetCurrentContext();
		}

		inline void playSound(ALuint sourceID)
		{
			alSourcePlay(sourceID);
		}

		inline void pauseSound(ALuint sourceID)
		{
			alSourcePause(sourceID);
		}

		inline void stopSound(ALuint sourceID)
		{
			alSourceStop(sourceID);
		}

		inline void playSounds(uint32_t count, ALuint * sources)
		{
			alSourcePlayv((ALsizei)count, sources);
		}

		inline void pauseSounds(uint32_t count, ALuint * sources)
		{
			alSourcePausev((ALsizei)count, sources);
		}

		inline void stopSounds(uint32_t count, ALuint * sources)
		{
			alSourceStopv((ALsizei)count, sources);
		}
	};
}}


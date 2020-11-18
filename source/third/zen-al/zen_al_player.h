/*
 Copyright (c) 2013 MeherTJ G.
 
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


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

#include "zen_al_player.h"
#include "zen_exception.h"

namespace Zen { namespace AL {
	Player * Player::GetDefault()
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

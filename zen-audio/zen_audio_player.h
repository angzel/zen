#pragma once

#include "zen_audio_common.h"

namespace Zen {
	class AudioPlayer : public Zen::Object
	{
	public:
		static AudioPlayer * GetDefault();

		virtual int play(std::string const & file, bool loop, float volume, float pitch) = 0;

		virtual void stop(int) = 0;

		virtual void pauseAll() = 0;

		virtual void resumeAll() = 0;

		virtual void stopAll() = 0;

		virtual void setVolume(float volume) = 0;

		virtual float getVolume() = 0;

		virtual void setEnable(bool) = 0;

		virtual bool isEnable() = 0;
	};
}

#pragma once

#include "zen_audio.h"
#include "zen_types.h"
#include "zen_info.h"
#include "zen_al.h"

namespace Zen {

	typedef int AudioTrackID;

	static const AudioTrackID EMPTY_TRACK_ID = 0;

	class AudioPlayer : public Zen::Object
	{
	public:
		static AudioPlayer * GetDefault();

		virtual void releaseCache() = 0;

		virtual void prepare(std::string const & file) = 0;

		virtual void setAudioDecoder(std::string const & type, std::shared_ptr<AudioDecoder> decoder) = 0;

		virtual std::shared_ptr<AudioDecoder> getAudioDecoder(std::string const & type) = 0;

		virtual void clearAudioDecoders() = 0;

		virtual AudioTrackID play(std::string const & file, bool loop) = 0;

		virtual AudioTrackID play3D(std::string const & file, bool loop,
									float volume, float pitch,
									Zen::Point3 position,
									Zen::Point3 velocity) = 0;

		virtual bool resetVolume(AudioTrackID, float v) = 0;
		virtual bool resetPitch(AudioTrackID, float p) = 0;
		virtual bool resetPosition(AudioTrackID, Zen::Point3) = 0;
		virtual bool resetVelocity(AudioTrackID, Zen::Point3) = 0;

		virtual bool pause(AudioTrackID) = 0;

		virtual bool resume(AudioTrackID) = 0;

		virtual bool stop(AudioTrackID) = 0;

		virtual bool isValid(AudioTrackID) = 0;

		virtual void pausePlayer() = 0;

		virtual void resumePlayer() = 0;

		virtual void stopAllTracks() = 0;

		virtual bool isPlayerPaused() = 0;
	};
}

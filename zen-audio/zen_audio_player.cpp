#include "zen_audio_player.h"
#include "zen_audio_raw.h"
#include "zen_audio_wav.h"
#include "zen_al.h"
#include "zen_system.h"
#include "zen_log.h"
#include <vector>
#include <map>

namespace Zen {
	static const size_t AudioSourceCount = 128;

	static Zen::AudioCoderWav sAudioCoderWavCoder;
	static Zen::AudioCoderRaw sAudioCoderRawCoder;

	struct AudioSource
	{
		std::shared_ptr<Zen::AL::Buffer> buffer;
		Zen::AL::Source * source = nullptr;
		float volume = 1.0;
		bool looping = false;
	};

	class AudioPlayerInner : public AudioPlayer
	{
	protected:
		std::vector<AudioSource> mSources;
		Zen::AL::Player * mEntity;
		size_t mCurrentIndex = 0;

		std::map<std::string, std::shared_ptr<Zen::AL::Buffer> > mAudioCache;
	public:
		AudioPlayerInner()
		{
			mEntity = Zen::AL::Player::GetDefault();
			mSources.resize(AudioSourceCount);
			for(size_t i = 0; i < AudioSourceCount; ++i)
			{
				auto & s = mSources[i];
				s.source = new Zen::AL::Source;
				s.source->create();
				s.source->setPitch(1.f);
				s.source->setPosition({0, 0, 0});
				s.source->setVelocity({0, 0, 0});
			}
		}
		~AudioPlayerInner()
		{
			for(auto & i : mSources)
			{
				delete i.source;
			}
		}
		static AudioPlayerInner * GetDefault()
		{
			static auto me = new AudioPlayerInner;
			return me;
		}
		std::shared_ptr<Zen::AL::Buffer> _getAudio(std::string const & file)
		{
			auto iter = mAudioCache.find(file);
			if(iter != mAudioCache.end()) return iter->second;

			auto data = Zen::System::LoadResourceContent(file);
			if(data.size() == 0)
			{
				data = Zen::System::LoadDocumentContent(file);
				if(data.empty()) return nullptr;
			}

			Zen::AudioData audio;
			int type = 0;
			if(file.size() > 4)
			{
				auto tail = file.substr(file.size()-4);
				if(tail == ".raw") type = 1;
				else if(tail == ".wav") type = 2;
			}
			do {
				if(type == 1 || type == 0)
				{
					try {
						sAudioCoderRawCoder.decode(audio, data);
					} catch(...) {}
					if(audio.count) break;
				}
				if(type == 2 || type == 0)
				{
					try {
						sAudioCoderWavCoder.decode(audio, data);
					} catch(...) {}
					if(audio.count) break;
				}
				return nullptr;
			} while(false);
			if(audio.format == Zen::EAudioFormat::None) return nullptr;

			auto buffer = new Zen::AL::Buffer;
			buffer->create();
			Zen::AL::EBufferFmt format;
			switch (audio.format) {
				case Zen::EAudioFormat::Mono8:
					format = Zen::AL::EBufferFmt::Mono8;
					break;
				case Zen::EAudioFormat::Stereo8:
					format = Zen::AL::EBufferFmt::Stereo8;
					break;
				case Zen::EAudioFormat::Mono16:
					format = Zen::AL::EBufferFmt::Mono16;
					break;
				case Zen::EAudioFormat::Stereo16:
					format = Zen::AL::EBufferFmt::Stereo16;
					break;

				default:
					return nullptr;
					break;
			}
			buffer->bindData(format, audio.buffer.data(), (uint32_t)audio.buffer.size(), audio.frequency);

			return (mAudioCache[file] = std::shared_ptr<Zen::AL::Buffer>(buffer));
		}
		int _getNextFreeIndex()
		{
			for(auto i = mCurrentIndex + 1; i < AudioSourceCount; ++i)
			{
				if(mSources[i].looping) continue;
				if(mSources[i].source->isFree()) return (int)i;
			}
			for(auto i = 0; i < mCurrentIndex; ++i)
			{
				if(mSources[i].looping) continue;
				if(mSources[i].source->isFree()) return (int)i;
			}
			return -1;
		}
		virtual int play(std::string const & file, bool loop, float volume, float pitch) override
		{
			if(!mOn && !loop) return -1;
			if(mIsPaused && !loop) return -1;

			auto index = _getNextFreeIndex();
			if(index < 0)
			{
				LogW("%s", "too many sound is playing!");
				return -1;
			}
			auto audio = _getAudio(file);
			if(!audio)
			{
				LogW("cannot load audio [%s]", file.data());
				return -1;
			}

			mCurrentIndex = index;

			auto & item = mSources[index];
			item.buffer = audio;
			item.volume = volume;
			item.looping = loop;
			auto s = mSources[index].source;
			s->bindSound(audio->getALBufferID());
			s->setLooping(loop);
			s->setPitch(pitch);
			s->setGain(volume * mVolume);
			if(mOn && !mIsPaused) mEntity->playSound(s->getALSourceID());
			return index;
		}

		virtual void stop(int index) override
		{
			if(index < 0) return;

			auto & item = mSources[index];
			mEntity->stopSound(item.source->getALSourceID());
			item.looping = false;
			item.buffer = nullptr;
		}

		virtual void pauseAll() override
		{
			if(mIsPaused) return;
			mIsPaused = true;

			for(auto & i : mSources)
			{
				if(i.source->getPlayState() == Zen::AL::EPlayState::Playing)
				{
					mEntity->pauseSound(i.source->getALSourceID());
				}
			}
		}

		virtual void resumeAll() override
		{
			if(!mIsPaused) return;
			mIsPaused = false;

			_onReplay();
		}

		virtual void stopAll() override
		{
			for(auto & i : mSources)
			{
				auto state = i.source->getPlayState();
				if(state == Zen::AL::EPlayState::Playing ||
				   state == Zen::AL::EPlayState::Paused)
				{
					mEntity->stopSound(i.source->getALSourceID());
				}
				i.looping = false;
			}
		}

		virtual void setVolume(float volume) override
		{
			mVolume = volume;
		}

		virtual float getVolume() override
		{
			return mVolume;
		}

		virtual void setEnable(bool on) override
		{
			if(mOn == on) return;
			mOn = on;

			if(!on)
			{
				for(auto & i : mSources)
				{
					if(!i.source->isFree())
					{
						mEntity->stopSound(i.source->getALSourceID());
					}
				}
			}
			else _onReplay();
		}

		void _onReplay()
		{
			if(mOn && !mIsPaused)
			{
				for(auto & i : mSources)
				{
					auto state = i.source->getPlayState();

					if(state == Zen::AL::EPlayState::Paused)
					{
						mEntity->playSound(i.source->getALSourceID());
					}
					else if(i.looping)
					{
						mEntity->playSound(i.source->getALSourceID());
					}
				}
			}
		}

		virtual bool isEnable() override
		{
			return mOn;
		}

	protected:
		bool mOn = true;
		bool mIsPaused = false;
		float mVolume = 1.f;
	};

	AudioPlayer * AudioPlayer::GetDefault()
	{
		return AudioPlayerInner::GetDefault();
	}
}

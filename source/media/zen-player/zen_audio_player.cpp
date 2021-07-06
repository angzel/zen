#include "zen_audio_player.h"
#include "zen_audio_raw.h"
#include "zen_audio_wav.h"
#include "zen_log.h"
#include "zen_file.h"
#include <vector>
#include <map>

namespace Zen {
#if defined(ZEN_P_AUDIO_TRACK_LIMIT) && \
ZEN_P_AUDIO_TRACK_LIMIT >= 8 && \
ZEN_P_AUDIO_TRACK_LIMIT <= 1024
	static const size_t AudioTrackLimit = ZEN_P_AUDIO_TRACK_LIMIT;
#else
	static const size_t AudioTrackLimit = 128;
#endif

#define EMPTY_TRACK_ID 0

	struct AudioTrack
	{
		Zen::AL::Source * source = nullptr;

		std::shared_ptr<Zen::AL::Buffer> buffer;
		bool looped = false;
		bool paused = false;
		AudioTrackID track_id = EMPTY_TRACK_ID;

		void reset()
		{
			buffer = nullptr;
			looped = false;
			paused = false;
			track_id = EMPTY_TRACK_ID;
		}
	};

	class AudioPlayerInner : public AudioPlayer
	{
	protected:
		std::vector<AudioTrack*> mTracks;
		Zen::AL::Player * mEntity;

		std::map<std::string, std::shared_ptr<Zen::AL::Buffer> > mAudioCache;
		int mCurrentTrackID = 1;

		std::map<std::string, std::shared_ptr<AudioDecoder> > mAudioDecoders;
	public:
		AudioPlayerInner()
		{
			mEntity = Zen::AL::Player::Get();
			mTracks.resize(AudioTrackLimit);

			for(size_t i = 0; i < mTracks.size(); ++i)
			{
				mTracks[i] = new AudioTrack;
				auto source = new Zen::AL::Source;
				source->create();
				source->setPitch(1.f);
				source->setGain(1.f);
				source->setPosition({0, 0, 0});
				source->setVelocity({0, 0, 0});
				mTracks[i]->source = source;
			}
		}
		~AudioPlayerInner()
		{
			for(auto i : mTracks)
			{
				delete i->source;
				delete i;
			}
		}

		static AudioPlayerInner * Get()
		{
			static auto me = new AudioPlayerInner;
			return me;
		}

		virtual void prepare(const std::string &file) override {
			this->_getAudio(file);
		}

		void releaseCache() override
		{
			for(auto t : mTracks)
			{
				if(t->buffer == nullptr) continue;

			}
		}

		std::shared_ptr<Zen::AL::Buffer> _getAudio(std::string const & file)
		{
			auto iter = mAudioCache.find(file);
			if(iter != mAudioCache.end()) return iter->second;

			auto data = Zen::Utils::Get()->loadResource(file);
			if(data.size() == 0)
			{
				data = Zen::Utils::Get()->loadDocument(file);
				if(data.empty()) return nullptr;
			}

			auto ext = GetFilePathExtention(file);
			auto decoder = this->getAudioDecoder(ext);
			if(!decoder)
			{
				decoder = this->getAudioDecoder("");
			}
			if(!decoder)
			{
				Zen::LogW("no valid decoder for audio:%s", file.data());
				return nullptr;
			}

			auto audio = decoder->decode(data);

			auto buffer = new Zen::AL::Buffer;
			buffer->create();
			Zen::AL::eTrackType format;

			auto c = audio->channel();
			auto s = audio->sampleSize();
			if(c == 1 && s == 1) format = Zen::AL::eTrackType::Mono8;
			else if(c == 1 && s == 2) format = Zen::AL::eTrackType::Mono16;
			else if(c == 2 && s == 2) format = Zen::AL::eTrackType::Stereo8;
			else if(c == 2 && s == 4) format = Zen::AL::eTrackType::Stereo16;
			else return nullptr;

			buffer->bindData(format, audio->data(), (uint32_t)audio->size(), audio->frequency());

			return (mAudioCache[file] = std::shared_ptr<Zen::AL::Buffer>(buffer));
		}

		AudioTrack * _getTrackWithID(AudioTrackID tid)
		{
			for(auto t : mTracks)
			{
				if(t->track_id == tid) return t;
			}
			return nullptr;
		}

		AudioTrack * _getFreeTrack()
		{
			for(auto t : mTracks)
			{
				if(_freeTrack(t)) return t;
			}
			return nullptr;
		}

		bool _freeTrack(AudioTrack * track)
		{
			if(!track->buffer) return true;
			if(!track->looped && track->source->getPlayState() == Zen::AL::EPlayState::Stoped)
			{
				track->reset();
				return true;
			}
			return false;
		}

		virtual bool stop(AudioTrackID tid) override
		{
			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;
			mEntity->stopSound(track->source->getALSourceID());
			track->reset();
			return true;
		}

		bool pause(Zen::AudioTrackID tid) override {

			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;

			if(_freeTrack(track)) return false;

			track->paused = true;
			mEntity->pauseSound(track->source->getALSourceID());
			return true;
		}

		bool resume(Zen::AudioTrackID tid) override
		{

			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;
			if(_freeTrack(track)) return false;

			track->paused = false;
			if(!mIsPaused)
			{
				mEntity->playSound(track->source->getALSourceID());
			}
			return true;
		}

		bool isValid(Zen::AudioTrackID tid) override
		{

			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;
			if(_freeTrack(track)) return false;
			return true;
		}

		void pausePlayer() override
		{
			if(mIsPaused) return;
			mIsPaused = true;

			for(auto t : mTracks)
			{
				if(_freeTrack(t)) continue;
				mEntity->pauseSound(t->source->getALSourceID());
			}
		}

		void resumePlayer() override
		{
			if(!mIsPaused) return;
			mIsPaused = false;

			for(auto t : mTracks)
			{
				if(_freeTrack(t)) continue;

				if(!t->paused)
				{
					mEntity->playSound(t->source->getALSourceID());
				}
			}
		}

		void stopAllTracks() override
		{
			for(auto t : mTracks)
			{
				if(!t->buffer) continue;
				mEntity->stopSound(t->source->getALSourceID());
				t->reset();
			}
		}

		bool isPlayerPaused() override
		{
			return mIsPaused;
		}

		void setAudioDecoder(const std::string &type, std::shared_ptr<AudioDecoder> decoder) override {
			if(!decoder) mAudioDecoders.erase(type);
			else mAudioDecoders[type] = decoder;
		}

		std::shared_ptr<AudioDecoder> getAudioDecoder(const std::string &type) override {
			auto iter = mAudioDecoders.find(type);
			if(iter != mAudioDecoders.end()) return iter->second;
			return nullptr;
		}

		void clearAudioDecoders() override {
			mAudioDecoders.clear();
		}

//		virtual AudioTrackID play(std::string const & file, bool loop) override
//		{
//			return this->play3D(file, loop, 1.f, 1.f, {0, 0, 0}, {0, 0, 0});
//		}

		Zen::AudioTrackID play(const std::string &file, bool loop, float volume, float pitch, Zen::Point3 position, Zen::Point3 velocity) override
		{
			if(mIsPaused && !loop) return EMPTY_TRACK_ID;

			auto track = _getFreeTrack();
			if(!track)
			{
				LogW("%s", "too many sounds playing!");
				return EMPTY_TRACK_ID;
			}

			auto audio = _getAudio(file);
			if(!audio)
			{
				LogW("cannot load audio [%s]", file.data());
				return EMPTY_TRACK_ID;
			}

			track->buffer = audio;
			track->looped = loop;
			auto s = track->source;
			s->bindSound(audio->getALBufferID());
			s->setLooping(loop);
			s->setGain(volume);
			s->setPitch(pitch);
			s->setPosition(position);
			s->setVelocity(velocity);
			if(!mIsPaused) mEntity->playSound(s->getALSourceID());

			return (track->track_id = ++mCurrentTrackID);
		}

		bool resetVolume(Zen::AudioTrackID tid, float v) override {
			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;

			if(_freeTrack(track)) return false;
			track->source->setGain(v);
			return true;
		}

		bool resetPitch(Zen::AudioTrackID tid, float p) override {
			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;

			if(_freeTrack(track)) return false;
			track->source->setPitch(p);
			return true;
		}

		bool resetPosition(Zen::AudioTrackID tid, Zen::Point3 pos) override {
			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;

			if(_freeTrack(track)) return false;
			track->source->setPosition(pos);
			return true;
		}

		bool resetVelocity(Zen::AudioTrackID tid, Zen::Point3 v) override {
			if(tid == EMPTY_TRACK_ID) return false;
			auto track = _getTrackWithID(tid);
			if(!track) return false;

			if(_freeTrack(track)) return false;
			track->source->setVelocity(v);
			return true;
		}

	protected:
		bool mIsPaused = false;
	};

	AudioPlayer * AudioPlayer::Get()
	{
		return AudioPlayerInner::Get();
	}
}

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_audio_raw.h"
#include "zen_file.h"
#include "zen_exception.h"
#include "zen_endian.h"

namespace Zen {
	struct AudioRawHead
	{
#define Version Byte4('j','a','i','w').value
		
		// donnt change the order of these members.
		uint32_t sign;
		uint16_t channel;
		uint16_t sample_size;
		uint32_t frequency;
		uint32_t sample_count;
		
		AudioRawHead()
		{
		}
		void set(size_t channel, size_t sample_size, size_t frequency, size_t sample_count)
		{
			sign = Version;
			this->channel = HostNet16((uint16_t)channel);
			this->sample_size = HostNet16((uint16_t)sample_size);
			this->frequency = HostNet32((uint32_t)frequency);
			this->sample_count = HostNet32((uint32_t)sample_count);
		}
	};

	std::shared_ptr<Audio> AudioRawDecoder::decode(const std::vector<uint8_t> &data)
	{
//		Zen::BufferReader reader(&data);

		AudioRawHead head;

		musts(data.size() >= sizeof(head), "failed to read header");
		::memcpy(&head, data.data(), sizeof(head));

		auto channel = HostNet16(head.channel);
		auto sample_size = HostNet16(head.sample_size);
		auto frequency = HostNet32(head.frequency);
		auto sample_count = HostNet32(head.sample_count);

		uint32_t size = sample_size * sample_count;

		musts(size > 0, "invalid file format");

		musts(data.size() >= sizeof(head) + size, "invalid file content");

		auto audio = Audio::Create(channel, sample_size, frequency, sample_count);
		must(size == audio->size());
		::memcpy(audio->data(), data.data() + sizeof(head), size);
		return audio;
	}
	std::vector<uint8_t> AudioRawEncoder::encode(const Zen::Audio & audio)
	{
		AudioRawHead head;
		head.set(audio.channel(), audio.sampleSize(), audio.frequency(), audio.sampleCount());
		std::vector<uint8_t> data;
		data.reserve(sizeof(head) + audio.size());

		auto head_buf = reinterpret_cast<char const *>(&head);
		data.assign(head_buf, head_buf + sizeof(head));
		data.insert(data.end(), audio.data(), audio.data() + audio.size());
		return data;
	}
}


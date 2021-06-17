/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_audio.h"

namespace Zen {

	std::shared_ptr<Audio> Audio::Create(size_t channel, size_t sample_size, size_t frequency, size_t sample_count)
	{
		auto audio = std::shared_ptr<Audio>(new Audio);
		audio->m_sample_size = (uint16_t)sample_size;
		audio->m_channel = (uint16_t)channel;
		audio->m_frequency = (uint32_t)frequency;
		audio->m_sample_count = (uint32_t)sample_count;

		audio->m_buffer.resize(0);
		audio->m_buffer.resize(sample_count * sample_size, 0);
		return audio;
	}

	std::shared_ptr<Audio> Audio::CreateSine(size_t channel, size_t sample_size, size_t frequency, size_t sample_count, float tune)
	{
		auto audio = Create(channel, sample_size, frequency, sample_count);
		auto value_size = sample_size / channel;
		if(value_size == 1)
		{

		}
		return audio;
	}
}

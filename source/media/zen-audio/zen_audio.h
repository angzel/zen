/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_object.h"

#include <memory>
#include <vector>

namespace Zen {
	class Audio {
	public:
		static std::shared_ptr<Audio> Create(size_t channel, size_t sample_size, size_t frequency, size_t sample_count);

		static std::shared_ptr<Audio> CreateSine(size_t channel, size_t sample_size, size_t frequency, size_t sample_count, float tune);

		size_t channel() const { return m_channel; }
		size_t sampleSize() const { return m_sample_size; }
		size_t sampleCount() const { return m_sample_count; }
		size_t frequency() const { return m_frequency; }
		size_t size() const { return m_buffer.size(); }
		uint8_t * data() { return m_buffer.data(); }
		uint8_t const * data() const { return m_buffer.data(); }
	protected:
		Audio() = default;
		Audio(Audio&) = default;

		std::vector<uint8_t> m_buffer;
		uint16_t m_channel = 1;
		uint16_t m_sample_size = 1;
		uint32_t m_frequency = 0;
		uint32_t m_sample_count = 0;
	};

	class AudioDecoder
	{
	public:
		virtual std::shared_ptr<Audio> decode(std::vector<uint8_t> const & data) = 0;
		virtual ~AudioDecoder() = default;
	};

	class AudioEncoder
	{
	public:
		virtual std::vector<uint8_t> encode(Audio const &) = 0;
		virtual ~AudioEncoder() = default;
	};
}

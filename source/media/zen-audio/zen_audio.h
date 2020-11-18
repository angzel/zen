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

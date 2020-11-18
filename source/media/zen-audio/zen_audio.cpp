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

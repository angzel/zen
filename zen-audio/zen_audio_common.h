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

#pragma once

#include <vector>
#include "zen_object.h"

namespace Zen {
	enum class EAudioFormat
	{
		None,
		Mono8,
		Mono16,
		Stereo8,
		Stereo16,
	};
	inline int GetBytesOfAudioFormat(EAudioFormat format)
	{
		switch (format) {
			case EAudioFormat::Mono8: return 1;
			case EAudioFormat::Mono16: return 2;
			case EAudioFormat::Stereo8: return 2;
			case EAudioFormat::Stereo16: return 4;
			default: return 0;
		}
	}
	inline int GetChannelsOfAudioFormat(EAudioFormat format)
	{
		switch (format) {
			case EAudioFormat::Mono8: return 1;
			case EAudioFormat::Mono16: return 1;
			case EAudioFormat::Stereo8: return 2;
			case EAudioFormat::Stereo16: return 2;
			default: return 0;
		}
	}
	struct AudioData
	{
		std::vector<uint8_t> buffer;
		EAudioFormat format = EAudioFormat::None;
		uint32_t frequency = 0;
		uint32_t count = 0;
	};

	class AudioCoder : public virtual Zen::Object
	{
	public:
		virtual void load(AudioData & wave, std::string const & file) = 0;
		virtual void save(AudioData const & wave, std::string const & file) = 0;
		virtual void decode(AudioData & wave, std::vector<uint8_t> const & data) = 0;
		virtual std::vector<uint8_t> encode(AudioData const & wave) = 0;
	};

	inline void AudioMake(AudioData & data, EAudioFormat format, uint32_t frequency, uint32_t count)
	{
		data.format = format;
		data.frequency = frequency;
		data.count = count;
		data.buffer.resize(count * GetBytesOfAudioFormat(format));
	}
}

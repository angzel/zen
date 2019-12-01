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

#include "zen_audio_raw.h"
#include "zen_file.h"
#include "zen_buffer.h"
#include "zen_exception.h"
#include "zen_endian.h"

namespace Zen {
	struct AudioRawHead
	{
#define Version Byte4('j','a','i','w').value
		
		// donnt change the order of these members.
		uint32_t sign;
		uint32_t length;
		uint32_t frequence;
		uint16_t format;
		uint16_t keep_;
		
		AudioRawHead()
		{
		}
		void set(uint32_t len, uint32_t freq,  uint32_t fmt)
		{
			sign = Version;
			length = HostNet32((uint32_t)len);
			frequence = HostNet32((uint32_t)freq);
			format = HostNet16((uint16_t)fmt);
			keep_ = 0;
		}
	};
	
	void AudioRaw::load(AudioData & wave, std::string const & file)
	{
		wave.format = EAudioFormat::None;
		
		auto data = Zen::ReadWholeFileToBuffer(file);
		musts(data.size(), "read file error");
		
		this->decode(wave, data);
	}
	
	void AudioRaw::save(AudioData const & wave, std::string const & file)
	{
		std::fstream outs;
		outs.open(file, std::ios::out | std::ios::binary);
		musts(outs.good(), "open file error");
		
		AudioRawHead head;
		head.set(wave.count, wave.frequency, (uint32_t)wave.format);
		auto head_buf = reinterpret_cast<char const *>(&head);
		
		outs.write((const char*)head_buf, sizeof(head));
		outs.write((const char*)wave.buffer.data(), wave.buffer.size());
		
		musts(outs.good(), "write file error");
	}
	
	void AudioRaw::decode(AudioData & wave, std::vector<uint8_t> const & data)
	{
		wave.format = EAudioFormat::None;
		
		Zen::BufferReader reader(&data);
		
		AudioRawHead head;
		head.set(wave.count, wave.frequency, (uint32_t)wave.format);
		
		musts(reader.read(head), "failed to read header");
		
		auto format = (EAudioFormat)HostNet16(head.format);
		uint32_t length = HostNet32(head.length);
		uint32_t frequence = HostNet32(head.frequence);
		uint32_t size = GetBytesOfAudioFormat(format) * length;
		
		musts(size > 0, "invalid file format");
		
		musts(reader.getReadPointer() + size <= data.size(), "invalid file content");
		
		wave.buffer.resize(size);
		
		reader.read(wave.buffer.data(), wave.buffer.size());
		
		wave.count = length;
		wave.format = format;
		wave.frequency = frequence;
	}
	
	std::vector<uint8_t> AudioRaw::encode(AudioData const & wave)
	{
		AudioRawHead head;
		head.set(wave.count, wave.frequency, (uint32_t)wave.format);
		
		auto head_buf = reinterpret_cast<char const *>(&head);
		
		std::vector<uint8_t> data;
		data.reserve(sizeof(head) + wave.buffer.size());
		data.assign(head_buf, head_buf + sizeof(head));
		data.insert(data.end(), wave.buffer.begin(), wave.buffer.end());
		
		return data;
	}
}


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

#include "zen_audio_wav.h"
#include "zen_buffer.h"
#include "zen_file.h"
#include "zen_exception.h"
#include "zen_endian.h"
#include <iostream>

namespace Zen {
	struct AudioWavHead
	{
		Byte4 fiRiffID; // "RIFF"
		uint32_t fiRiffSize;
		Byte4 fiAudioType; // "WAVE"
		Byte4 fiFmtID; // "fmt "
		uint32_t fiFmtSize;
		uint16_t fiFormatTag;
		uint16_t fiChannels;
		uint32_t fiSPS;
		uint32_t fiBPS;
		uint16_t fiBlockAlign;
		uint16_t fiBPSample;
		
		void set(uint32_t data_size, uint32_t len, uint32_t freq, int channel, int byteps)
		{
			this->fiRiffID.set('R','I','F','F');
			this->fiRiffSize = (uint32_t)data_size + (44 - 8);
			this->fiAudioType.set('W','A','V','E');
			this->fiFmtID.set('f','m','t',' ');
			this->fiFmtSize = 16;
			this->fiFormatTag = 1;
			this->fiChannels = (uint16_t)channel;
			this->fiBlockAlign = (uint16_t)byteps;
			this->fiBPSample = (uint16_t)byteps * 8;
			this->fiSPS = (uint32_t)freq;
			this->fiBPS = (uint32_t)(freq * this->fiBlockAlign);
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
			this->fiRiffSize = EndianSwap32(this->fiRiffSize);
			this->fiFmtSize = EndianSwap32(this->fiFmtSize);
			this->fiFormatTag = EndianSwap16(this->fiFormatTag);
			this->fiChannels = EndianSwap16(this->fiChannels);
			this->fiSPS = EndianSwap32(this->fiSPS);
			this->fiBPS = EndianSwap32(this->fiBPS);
			this->fiBlockAlign = EndianSwap16(this->fiBlockAlign);
			this->fiBPSample = EndianSwap16(this->fiBPSample);
#endif
		}
	};
	
	void AudioWav::load(AudioData & wave, std::string const & file)
	{
		wave.format = EAudioFormat::None;
		
		auto data = Zen::ReadWholeFileToBuffer(file);
		musts(data.size(), "read file error");
		
		this->decode(wave, data);
	}
	
	void AudioWav::save(AudioData const & wave, std::string const & file) 	{
		std::fstream outs;
		outs.open(file, std::ios::out | std::ios::binary);
		musts(outs.good(), "open file error");
		
		uint32_t fiDataSize = (uint32_t)wave.buffer.size();
		Byte4 fiDataID('d','a','t','a');
		
		AudioWavHead head;

		auto channel = GetChannelsOfAudioFormat(wave.format);
		auto bytes = GetBytesOfAudioFormat(wave.format);
		head.set(fiDataSize, wave.count, wave.frequency, channel, bytes);

		outs.write((char const *)&head, sizeof(head));
		outs.write((char const *)&fiDataID, sizeof(fiDataID));
		outs.write((char const *)&fiDataSize, sizeof(fiDataSize));
		outs.write((char const *)wave.buffer.data(), wave.buffer.size());
		
		musts(outs.good(), "write wav file error");
	}
	
	void AudioWav::decode(AudioData & wave, std::vector<uint8_t> const & data)
	{
		wave.format = EAudioFormat::None;

		Zen::BufferReader reader(&data);
		
		AudioWavHead head;
		
		musts(reader.read(head), "failed to read file data");
		
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
		head.fiRiffSize = EndianSwap32(head.fiRiffSize);
		head.fiFmtSize = EndianSwap32(head.fiFmtSize);
		head.fiFormatTag = EndianSwap16(head.fiFormatTag);
		head.fiChannels = EndianSwap16(head.fiChannels);
		head.fiSPS = EndianSwap32(head.fiSPS);
		head.fiBPS = EndianSwap32(head.fiBPS);
		head.fiBlockAlign = EndianSwap32(head.fiBPSample);
#endif
		
		uint16_t fiCbSize = 0;
		
		if(head.fiFmtSize == 18)
		{
			musts(reader.read(fiCbSize), "error wav format");
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
			fiCbSize = EndianSwap16(fiCbSize);
#endif
			reader.jumpReadPointer(fiCbSize);
		}
		else
		{
			musts(head.fiFmtSize == 16, "invalid wave format size");
		}
		
		musts(head.fiFormatTag == 1, "not PCM wav format");
		
		musts(head.fiRiffID == Byte4('R','I','F','F') &&
			  head.fiAudioType == Byte4('W','A','V','E') &&
			  head.fiFmtID == Byte4('f','m','t','\x20'), "invalid wav file format");
		
		auto format = EAudioFormat::None;
		if(head.fiChannels == 1)
		{
			if(head.fiBPSample == 8) format = EAudioFormat::Mono8;
			else if(head.fiBPSample == 16) format = EAudioFormat::Mono16;
		}
		else if(head.fiChannels == 2)
		{
			if(head.fiBPSample == 8) format = EAudioFormat::Stereo8;
			else if(head.fiBPSample == 16) format = EAudioFormat::Stereo16;
		}
		musts(format != EAudioFormat::None, "unsupported format");
		musts(GetBytesOfAudioFormat(format) == (uint32_t)head.fiBlockAlign, "invalid channel and sample info");
		
		Byte4 fiDataID;
		uint32_t fiDataSize = 0;

		musts((reader.read(fiDataID) && reader.read(fiDataSize)), "failed to read data info");
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
		fiDataSize = EndianSwap32(fiDataSize);
#endif
		if(fiDataID != Byte4('d','a','t','a'))
		{
			musts(reader.getReadPointer() + fiDataSize <= data.size(), "invalid fact data");
			reader.jumpReadPointer(fiDataSize);
			
			musts((reader.read(fiDataID) && reader.read(fiDataSize)), "failed to read data info");
#if ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN
			fiDataSize = EndianSwap32(fiDataSize);
#endif
		}
		
		musts(fiDataID == Byte4('d', 'a', 't', 'a'), "invalid data format");
		
		uint32_t length = (uint32_t)(fiDataSize / head.fiBlockAlign);
		uint32_t size = length * (uint32_t)head.fiBlockAlign;
		
		musts(reader.getReadPointer() + size <= data.size(), "invalid data size");
		
		wave.buffer.resize(size);

		reader.read(wave.buffer.data(), wave.buffer.size());
		
		wave.count = length;
		wave.format = format;
		wave.frequency = (uint32_t)head.fiSPS;
	}
	
	std::vector<uint8_t> AudioWav::encode(AudioData const & wave)
	{
		std::vector<uint8_t> data;

		Zen::BufferWriter writer(&data);

		uint32_t fiDataSize = (uint32_t)wave.buffer.size();
		Byte4 fiDataID('d','a','t','a');

		AudioWavHead head;
		auto channel = GetChannelsOfAudioFormat(wave.format);
		auto bytes = GetBytesOfAudioFormat(wave.format);
		head.set(fiDataSize, wave.count, wave.frequency, channel, bytes);

		writer.append(head);
		writer.append(fiDataID);
		writer.append(fiDataSize);
		writer.append(wave.buffer.data(), wave.buffer.size());

		return data;
	}
}



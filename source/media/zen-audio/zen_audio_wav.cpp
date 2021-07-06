/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_audio_wav.h"
#include "zen_file.h"
#include "zen_buffer.h"
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
		
		void set(size_t channel, size_t sample_size, size_t frequency, size_t sample_count, size_t data_size)
		{
			this->fiRiffID.set('R','I','F','F');
			this->fiRiffSize = (uint32_t)data_size + (44 - 8);
			this->fiAudioType.set('W','A','V','E');
			this->fiFmtID.set('f','m','t',' ');
			this->fiFmtSize = 16;
			this->fiFormatTag = 1;
			this->fiChannels = (uint16_t)channel;
			this->fiBlockAlign = (uint16_t)sample_size;
			this->fiBPSample = (uint16_t)sample_size * 8;
			this->fiSPS = (uint32_t)frequency;
			this->fiBPS = (uint32_t)(frequency * sample_size);
#if ZEN_ENDIAN == ZEN_BE_VALUE
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

	std::shared_ptr<Audio> AudioWavDecoder::decode(std::vector<uint8_t> const & data)
	{
		Zen::BufferReader reader(&data);

		AudioWavHead head;

		musts(reader.read(head), "invalid wav data");

#if ZEN_ENDIAN == ZEN_BE_VALUE
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
#if ZEN_ENDIAN == ZEN_BE_VALUE
			fiCbSize = EndianSwap16(fiCbSize);
#endif
			reader.forward(fiCbSize);
		}
		else
		{
			musts(head.fiFmtSize == 16, "invalid wave format size");
		}
		
		musts(head.fiFormatTag == 1, "not PCM wav format");
		
		musts(head.fiRiffID == Byte4('R','I','F','F') &&
			  head.fiAudioType == Byte4('W','A','V','E') &&
			  head.fiFmtID == Byte4('f','m','t','\x20'), "invalid wav format");

		Byte4 fiDataID;
		uint32_t fiDataSize = 0;

		musts(reader.read(fiDataID) && reader.read(fiDataSize), "invalid wav data info");
#if ZEN_ENDIAN == ZEN_BE_VALUE
		fiDataSize = EndianSwap32(fiDataSize);
#endif
		if(fiDataID != Byte4('d','a','t','a'))
		{
			musts(reader.position() + fiDataSize <= data.size(), "invalid fact data");
			reader.forward(fiDataSize);
			
			musts(reader.read(fiDataID) && reader.read(fiDataSize), "invalid wav data info");

#if ZEN_ENDIAN == ZEN_BE_VALUE
			fiDataSize = EndianSwap32(fiDataSize);
#endif
		}
		
		musts(fiDataID == Byte4('d', 'a', 't', 'a'), "invalid data format");
		
		uint32_t length = (uint32_t)(fiDataSize / head.fiBlockAlign);
		uint32_t size = length * (uint32_t)head.fiBlockAlign;
		
		musts(reader.position() + size <= data.size(), "invalid data size");

		auto audio = Audio::Create(head.fiChannels, head.fiBlockAlign, head.fiSPS, length);

		must(audio->size() == size);

		reader.read(audio->data(), audio->size());

		return audio;
	}
	
	std::vector<uint8_t> AudioWavEncoder::encode(Audio const & audio)
	{
		Byte4 fiDataID('d','a','t','a');
		Byte4 fiDataSize((uint32_t)audio.size());

		AudioWavHead head;
		head.set(audio.channel(), audio.sampleSize(), audio.frequency(), audio.sampleCount(), audio.size());

		std::vector<uint8_t> data;
		data.reserve(sizeof(head) + audio.size() + 8);
		auto head_buf = reinterpret_cast<char const *>(&head);
		data.assign(head_buf, head_buf + sizeof(head));
		data.insert(data.end(), fiDataID.bytes, fiDataID.bytes + 4);
		data.insert(data.end(), fiDataSize.bytes, fiDataSize.bytes + 4);
		data.insert(data.end(), audio.data(), audio.data() + audio.size());

		return data;
	}
}



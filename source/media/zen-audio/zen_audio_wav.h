/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_audio.h"

namespace Zen {
	class AudioWavDecoder : public AudioDecoder
	{
	public:
		std::shared_ptr<Audio> decode(const std::vector<uint8_t> &data) override;

	};

	class AudioWavEncoder : public AudioEncoder
	{
	public:
		std::vector<uint8_t> encode(const Zen::Audio &) override;
	};

	class AudioWavCoder : public AudioWavDecoder, public AudioWavEncoder
	{

	};
}

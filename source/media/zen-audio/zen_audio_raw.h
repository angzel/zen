/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#include "zen_audio.h"

namespace Zen {
	class AudioRawDecoder : public AudioDecoder
	{
	public:
		std::shared_ptr<Audio> decode(const std::vector<uint8_t> &data) override;

	};

	class AudioRawEncoder : public AudioEncoder
	{
	public:
		std::vector<uint8_t> encode(const Zen::Audio &) override;
	};
	
	class AudioRawCoder : public AudioRawEncoder, public AudioRawDecoder
	{

	};
}

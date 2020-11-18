	//
	//  main.cpp
	//  zena_dev
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_audio.h"
#include "zen_audio_wav.h"
#include "zen_file.h"
#include "zen_audio_raw.h"
#include "zen_audio_player.h"

using namespace std;
using namespace Zen;

int main(int argc, const char * argv[])
{
	auto player = Zen::AudioPlayer::GetDefault();
	player->setAudioDecoder("wav", std::shared_ptr<AudioDecoder>(new AudioWavCoder));

	player->play("bee.wav", 0);
	int a = 0;
	Zen::AudioTrackID loopId = 0;
	while((a = cin.get()))
	{
		if(a <= 0)
		{
			fflush(stdin);
			cin.clear();
		}
		if(a == 'a')
		{
			player->stopAllTracks();
		}
		else if(a == 'b')
		{
			player->play("bee.wav", 0);
		}
		else if(a == 'e')
		{
			loopId = player->play("bee.wav", 1);
		}
		else if(a == 'c')
		{
			player->pausePlayer();
		}
		else if(a == 'd')
		{
			player->resumePlayer();
		}
		else if(a == 'p')
		{
			player->pause(loopId);
		}
		else if(a == 'r')
		{
			player->resume(loopId);
		}
		else if(a == 's')
		{
			player->stop(loopId);
		}
	}
//	if(argc != 3) {
//		exit(-1);
//	}
//	std::string src = argv[1];
//	std::string dst = argv[2];
//	auto data = Zen::LoadFileToBuffer(src);
//	Zen::AudioRawCoder raw;
//	Zen::AudioWavCoder wav;
//	auto audio = raw.decode(data);
//	data = wav.encode(*audio);
//	Zen::WriteBufferToFile(dst, data);
	return 0;
}

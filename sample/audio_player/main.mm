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
	
	string a;
	Zen::AudioTrackID loopId = 0;
	while(cin>>a)
	{
		if(a == "stop")
		{
			player->stopAllTracks();
		}
		else if(a == "play")
		{
			player->play("bee.wav", 0);
		}
		else if(a == "loop")
		{
			loopId = player->play("bee.wav", 1);
		}
		else if(a == "pause")
		{
			player->pausePlayer();
		}
		else if(a == "resume")
		{
			player->resumePlayer();
		}
		else if(a == "ploop")
		{
			player->pause(loopId);
		}
		else if(a == "rloop")
		{
			player->resume(loopId);
		}
		else if(a == "stop-loop")
		{
			player->stop(loopId);
		}
		else {
			cout << "command list: \n play/stop/loop/pause/resume/ploop/rloop/sloop" << endl;
		}
	}

	return 0;
}

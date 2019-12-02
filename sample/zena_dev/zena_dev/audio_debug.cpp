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
#include "zen_system.h"


using namespace std;
int main(int argc, const char * argv[]) {
	auto player = Zen::AudioPlayer::GetDefault();
	player->play("a.wav", false, 1, 1);
	cout << Zen::System::GetDeviceUniqueID() << endl;
	char a;
	cin >> a;
	return 0;
}

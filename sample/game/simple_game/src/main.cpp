	//
	//  main.cpp
	//  gles_debug
	//
	//  Created by lover on 2019/12/4.
	//  Copyright © 2019 lover. All rights reserved.
	//
#include "zen_app_config.h"

#include <stdio.h>
#include <vector>
#include "MyMainDelegate.h"

MyConfig gConfig;

int ZenAppMain(int argc, const char ** argv)
{
	auto root = Zen::Vap::Root::Get();
	root->run(std::shared_ptr<RootDelegate>(new MainDelegate));
	Zen::App::Get()->setRotatable(true);
	Zen::App::Get()->setBGUpdate(true);

	return 0;
}


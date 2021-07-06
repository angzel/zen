	//
	//  main.cpp
	//  debug_zen
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_exception.h"
#include "zen_csv.h"
#include "zen_localization.h"

int main(int argc, const char * argv[]) {

	/// src是文本化表格格式文件（.csv）内容
	std::string src = "key,en,cn\n"
	"hello,Hello! I love You.,你好啊！朋友！\n"
	"move '1',This is your first step. Move any chess on light.,移动任何棋子。";

	Zen::CSVLoader csv_loader;
	csv_loader.decode(src);

	auto & data = csv_loader.getContent();
	for(auto & v : data.rows)
	{
		for(auto & i : v)
		{
			std::cout << i << std::endl;
		}
	}
	std::cout << std::string(12, '-') << std::endl;
	Zen::Localization local;
	local.initWithCSVContent(data);
	local.setLanguageIndex(0);
	std::cout << local.getText("hello") << std::endl;
	local.setLanguageIndex(1);
	std::cout << local.getText("hello") << std::endl;
	return 0;
}

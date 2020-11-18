	//
	//  main.cpp
	//  debug_zen
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_image.h"
#include "zen_image_png.h"
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_freetype.h"
#include "zen_file.h"
#include "zen_utf8.h"

using namespace std;
/**
	make text to the image.
	usage
 bin <text>
 */
int main(int argc, const char * argv[]) {
	std::string text_utf8 = "BeLoved God~";
	if(argc >= 2) text_utf8 = argv[1];

	Zen::ImagePNGCoder coder;

	auto buffer = Zen::LoadFileToBuffer("/System/Library/Fonts/Noteworthy.ttc");
	Zen::FontConfig config = { 32, 32, 0, 0, 0 };
	auto font = Zen::FontLibrary::S()->createFont(buffer);
	auto brush = Zen::FontBrush::Create(font, config);
	auto text = Zen::UTF8ToUnicode(text_utf8);
	auto render = Zen::FontRender::Create(brush, text, 0, 0, 0, 0);
	auto image = render->render(0, 0, 0, 0, 0);

	auto dest = coder.encode(*image);
	if(dest.size())
	{
		Zen::WriteBufferToFile("ft.png", dest);
	}
	return 0;
}

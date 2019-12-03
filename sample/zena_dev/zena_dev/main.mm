		//
		//  main.cpp
		//  zena_dev
		//
		//  Created by lover on 2019/11/29.
		//  Copyright © 2019 lover. All rights reserved.
		//

	#include <iostream>
	#include <string>
	#include "zen_image.h"
	#include "zen_image.h"
	#include "zen_image_jpg.h"
	#include "zen_system.h"
	#include "zen_freetype.h"
	#include "zen_file.h"
	#include "zen_utf8.h"

	using namespace std;
	int main(int argc, const char * argv[]) {
		Zen::ImageData data;

		Zen::FontConfig fc;
		fc.charW = 32;
		fc.charH = 32;
		auto font_src = Zen::ReadWholeFileToBuffer("/System/Library/Fonts/Noteworthy.ttc");
		auto lib = Zen::FontLibrary::GetDefault();
		auto font = lib->loadFont(font_src);
		auto render = Zen::FontRender::Create(font, fc, Zen::UTF8ToUnicode("hello,yaya"), 0);
		auto w = render->getOutputWidth();
		auto h = render->getOutputHeight();
		render->renderToImage(data, w, h+10, 0, 0);

		Zen::ImageCoderJPG jpg;
		jpg.save(data, "2.jpg");
		return 0;
	}

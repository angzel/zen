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
#include "zen_image_png.h"
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_file.h"
#include "zen_utf8.h"

using namespace std;

class ImageConvert {
	std::vector<std::string> mArgs;
	Zen::ImageCoderJPG jpg;
	Zen::ImageCoderPNG png;
	Zen::ImageCoderRaw raw;
	Zen::ImageData image;
public:
	void help()
	{
		auto bin = mArgs[0];
		auto pos = bin.find_last_of('/');
		if(pos != bin.npos)
		{
			bin = bin.substr(pos + 1);
		}
		cout << "usage: " << bin << " <source-file> <dest-files ...>\n";
		exit(0);
	}
	void run(int argc, const char ** argv)
	{
		if(argc < 1) exit(-1);
		for(int i = 0; i < argc; ++i)
		{
			mArgs.push_back(argv[i]);
		}
		if(mArgs.size() < 3)
		{
			help();
		}
		if(!load())
		{
			cout << "cannot load image:" << mArgs[1] << endl;
			exit(-1);
		}
		cout << "load image bpp:" << Zen::GetBytesOfImageFormat(image.format) << endl;
		cout << "width:" << image.width << " height:" << image.height << endl;
		for(int i = 2; i < argc; ++i)
		{
			if(saveTo(mArgs[i]))
			{
				cout << "save ok:" << mArgs[i] << endl;
			}
			else
			{
				cout << "save failed:" << mArgs[i] << endl;
			}
		}
	}
	std::string getType(std::string const & path)
	{
		auto pos = path.find_last_of(".");
		if(pos == path.npos) return "";
		return path.substr(pos+1);
	}
	bool load()
	{
		try {
			auto type = getType(mArgs[1]);
			auto data = Zen::LoadFileToBuffer(mArgs[1]);
			if(data.empty()) return false;
			if(type == "jpg" || type == "jpeg")
			{
				jpg.decode(image, data);
			}
			else if(type == "png")
			{
				png.decode(image, data);
			}
			else
			{
				raw.decode(image, data);
			}
		} catch(std::exception & e)
		{
			cout << "exception: " << e.what() << endl;
			return false;
		}
		return true;
	}
	bool saveTo(std::string const & path)
	{
		auto type = getType(path);
//		Zen::ImageData colors;
//		Zen::ImageSeparateColor(colors, image);
//		image = colors;
		try {
			std::vector<uint8_t> data;
			if(type == "jpg" || type == "jpeg")
			{
				jpg.setQuality(100);

				data = jpg.encode(image);
			}
			else if(type == "png")
			{
				data = png.encode(image);
			}
			else
			{
				data = raw.encode(image);
			}
			return Zen::WriteBufferToFile(path, data);
		} catch(std::exception & e)
		{
			cout << "exception: " << e.what() << endl;
			return false;
		}
	}
};
int main(int argc, const char * argv[]) {
//	Zen::ImageCoderPNG png;
//	Zen::ImageData image;
//	png.load(image, "png2.png");
//	png.load(image, "png2c.png");
//	return 0;
//	char const * args[8][3] = {
//		{ "", "png1.png", "png1c.png" },
//		{ "", "png1.png", "png1c.jpg" },
//		{ "", "png2.png", "png2c.png" },
//		{ "", "png2.png", "png2c.jpg" },
//		{ "", "jpg1.jpg", "jpg1c.png" },
//		{ "", "jpg1.jpg", "jpg1c.jpg" },
//		{ "", "jpg2.jpg", "jpg2c.png" },
//		{ "", "jpg2.jpg", "jpg2c.jpg" },
//	};	for(int i = 0; i < 8; ++i)
//	{
//		ImageConvert convert;
//		convert.run(3, args[i]);
//	}

	ImageConvert convert;
	convert.run(argc, argv);
	return 0;
}

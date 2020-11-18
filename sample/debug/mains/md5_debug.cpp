//
//  main.cpp
//  debug_zen
//
//  Created by lover on 2019/11/29.
//  Copyright © 2019 lover. All rights reserved.
//

#include <iostream>
#include <string>
#include "zen_md5.h"

struct A {
	char c1, c2;
};
int main(int argc, const char * argv[]) {
	Zen::MD5Util md5;
	md5.start();
	md5.update("hello", 5);
	std::cout << md5.finish() << std::endl;
	std::cout << md5.finish(1) << std::endl;
	std::cout << Zen::MD5("hello", 5, 1) << std::endl;
	return 0;
}

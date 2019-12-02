//
//  main.cpp
//  zena_dev
//
//  Created by lover on 2019/11/29.
//  Copyright © 2019 lover. All rights reserved.
//

#include <iostream>
#include <string>
#include "zen-string/zen_utf8.h"

struct A {
	char c1, c2;
};
int main(int argc, const char * argv[]) {
	std::string u = "ABCD甲乙丙丁\n";
	std::cout << u << "has " << u.size() << " bytes\n";
	auto u32 = Zen::UTF8ToUnicode(u);
	std::cout << "with " << u32.size() << " words\n";

	return 0;
}

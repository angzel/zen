//
//  main.cpp
//  zena_dev
//
//  Created by lover on 2019/11/29.
//  Copyright © 2019 lover. All rights reserved.
//

#include <iostream>
#include <string>
#include "zen-core/zen_exception.h"
#include "zen-string/zen_buffer.h"
#include "zen-string/zen_cast.h"

struct A {
	char c1, c2;
};
int main(int argc, const char * argv[]) {
	Zen::Buffer buffer;
	buffer.write(0, "hello", 5);
	std::cout << buffer.size() << std::endl;
	buffer.write(2, "world", 5);
	std::cout << buffer.size() << std::endl;
	std::cout << Zen::ToString(buffer.data()) << std::endl;
	buffer.append("lalalal", 6);
	std::cout << buffer.size() << std::endl;
	std::cout << Zen::ToString(buffer.data()) << std::endl;
	A a;
	buffer.read(a);
	buffer.jumpReadPointer(3);
	std::cout << a.c1 << a.c2 << std::endl;
	std::cout << buffer.getReadPointer() << std::endl;
	return 0;
}

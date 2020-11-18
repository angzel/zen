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
#include "zen_buffer.h"
#include "zen_cast.h"

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
	buffer.forward(3);
	std::cout << a.c1 << a.c2 << std::endl;
	std::cout << buffer.position() << std::endl;
	return 0;
}

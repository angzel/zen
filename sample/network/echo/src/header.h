#pragma once

#include "zen_socket.h"
#include <iostream>
#include <string>

using namespace std;
#define SIZE 1024

using namespace Zen::Socket;

char buf[SIZE];
inline void error(string const & s)
{
	cout << s << endl;
}
//auto a = AddressMake("47.114.72.150", 12886, Family::inet);
auto a = AddressMake("0.0.0.0", 10886, Family::inet);


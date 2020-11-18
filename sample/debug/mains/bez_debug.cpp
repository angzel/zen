	//
	//  main.cpp
	//  debug_zen
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_bezier.h"
#include "zen_fraction.h"
#include "zen_ticker.h"

using namespace std;

int main(int argc, const char * argv[]) {

	Zen::Bezier2_4 b;
	b.controls[0] = {0, 0};
	b.controls[1] = {0, 1};
	b.controls[2] = {1, 1};
	b.controls[3] = {2, 2};

	for(float i = 0; i < 1.0f; i+= 0.005f)
	{
		auto p1 = Zen::BezierGetPoint(b, i);
		auto pair = Zen::BezierGetPiar(b, i);
		auto p2 = Zen::Vector2Lerp(pair.first, pair.second, i);
		cout << p1 << endl;
		cout << p2 << endl;
		cout << "direct:" << (pair.second - pair.first) << endl;
		cout << string(12, '-') << endl;
	}

	return 0;
}

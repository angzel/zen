	//
	//  main.cpp
	//  zena_dev
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
	Zen::Fraction f;
	f.set(2, 3);
	cout << f << endl;
	cout << f.value() << endl;

	Zen::Ticker ticker;
	ticker.restart();

	Zen::Vector2 v{0, 0};
	{
		Zen::Bezier2_4 b;
		b.controls[0] = {0, 0};
		b.controls[1] = {0, 1};
		b.controls[2] = {1, 1};
		b.controls[3] = {2, 2};
		for(float i = 0; i < 1.0f; i+= 0.00005f)
		{
			auto p = Zen::BezierGetPoint(b, i);
			v = v + p;
		}
	}
	ticker.pause();
	auto sec = ticker.getRunningDuration();
	cout << Zen::ToSeconds(sec) << endl;
	cout << v << endl;
	return 0;
}

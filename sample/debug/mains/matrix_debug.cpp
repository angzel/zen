	//
	//  main.cpp
	//  debug_zen
	//
	//  Created by lover on 2019/11/29.
	//  Copyright © 2019 lover. All rights reserved.
	//

#include <iostream>
#include <string>
#include "zen_matrix.h"
#include "zen_meta.h"

using namespace std;
int main(int argc, const char * argv[]) {
	auto q1 = Zen::QuaternionMakeWithAngleAndAxis(12, 3, 4, 5);
	auto q2 = Zen::QuaternionMakeWithAngleAndVector3Axis(12, Zen::Vector3Make(3, 4, 5));
	auto m1 = Zen::Matrix4MakeWithQuaternion(q1);
	auto m2 = Zen::Matrix4MakeWithQuaternion(q2);
	auto m3 = Zen::Matrix4MakeRotation(12, 3, 4, 5);
	cout << (q1 == q2) << endl;
	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	auto v = Zen::Vector4Make(1, 3, .43, 3);
	cout << v << endl;
	cout << v.s.z << endl;
	return 0;
}

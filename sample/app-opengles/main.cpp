//
//  main.cpp
//  gles_debug
//
//  Created by lover on 2019/12/4.
//  Copyright © 2019 lover. All rights reserved.
//

#include <stdio.h>
#include "zen_app_runtime.h"
#include "zen_color.h"
#include "zen_random.h"
#include <OpenGLES/ES2/gl.h>

class ScreenColorDelegate : public Zen::AppRuntimeDelegate {
	Zen::Color color1, color2;
	Zen::Random r;
	float v = 0;
public:
	virtual void onLaunch(Zen::Size2 size) override {
		r.reset((uint32_t)::time(0));
		color2 = r.next();
		v = 1.f;
	}
	virtual void onDraw() override {
		v += 0.004f;
		if(v > 1)
		{
			color1 = color2;
			color2 = r.next();
			v = 0;
		}
		auto c = Zen::Color4fLerp(color1, color2, v);
		glClearColor(c.red, c.green, c.blue, c.alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}
};
int ZenAppMain(int argc, const char ** argv)
{
	auto runtime = Zen::AppRuntime::S();
	runtime->setRuntimeDelegate(std::shared_ptr<Zen::AppRuntimeDelegate>(new ScreenColorDelegate));
	return 0;
}

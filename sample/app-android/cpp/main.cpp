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
#include "zen_ticker.h"
#include "zen_log.h"
#include <GLES2/gl2.h>

class ScreenColorDelegate : public Zen::AppRuntimeDelegate {
	Zen::ColorGradient grad;
	Zen::Random r;
	float v = 0;
public:
	virtual void onLaunch(Zen::Size2 const & size) override {
		r.reset((uint32_t)::time(0));
		Zen::Color4 c1, c2;
		c1.set(r.next());
		c2.set(r.next());
		grad.set(c1, c2);
		ticker.restart();
	}
	virtual void onDraw() override {
		v += 0.004f;
		if(v > 1)
		{
			grad.set(grad.get1(), Zen::Color4(r.next()));
			v = 0;
		}
		auto c = grad.get(v);
		glClearColor(c.red, c.green, c.blue, c.alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	virtual void onTouchDown(Zen::AppTouch const & touch) override
	{
		Zen::AppRuntime::GetDefault()->setFramesPerSecond(40);
	}

	int count = 0;
	Zen::Ticker ticker;
	virtual void onUpdate() override
	{
		++count;
		auto t = Zen::ToSeconds(ticker.getRunningDuration());
		if(t > 5)
		{
			Zen::LogV("fps:%.2f\n", count/t);
			count = 0;
			ticker.restart();
		}
	}
};
int ZenAppMain(int argc, const char ** argv)
{
	auto runtime = Zen::AppRuntime::GetDefault();
	runtime->setRuntimeDelegate(new ScreenColorDelegate);
	return 0;
}

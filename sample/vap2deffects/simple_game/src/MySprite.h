#pragma once

#include <iostream>
#include <vector>
#include "zen_vap2d_config.h"
#include "zen_vap2d.h"
#include "zen_image_raw.h"
#include "zen_utils.h"
#include "zen_log.h"
#include "zen_ticker.h"

class MySprite : public Zen::Vap2d::Sprite {
public:
	virtual Node * onTouchDown(Zen::AppTouch const & touch) override
	{
		Zen::LogV("touch sprite: %.2f %.2f", touch.x, touch.y);
		return this;
	}
	virtual Node * onTouchMove(Zen::AppTouch const & touch) override
	{
		Zen::LogV("touch move sprite: %.2f %.2f", touch.x, touch.y);
		return this;
	}
	virtual Node * onTouchUp(Zen::AppTouch const & touch) override
	{
		Zen::LogV("touch up sprite: %.2f %.2f", touch.x, touch.y);
		return this;
	}
};

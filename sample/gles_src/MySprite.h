#pragma once

#include <iostream>
#include <vector>
#include "zen_gles2.h"
#include "zen_vap2d.h"
#include "zen_image_raw.h"
#include "zen_system.h"
#include "zen_ticker.h"

class MySprite : public Zen::Vap2d::Sprite {
public:
	virtual Node * onTouchDown(Zen::AppTouch const & touch) override
	{
		Zen::LogV("touch: %.2f %.2f", touch.x, touch.y);
		return this;
	}
	virtual Node * onTouchUp(Zen::AppTouch const & touch) override
	{
//		if(touch.y < 100) Zen::Vap2d::Root::GetDefault()->replaceRootNode(nullptr);
//		else if(touch.y < 400)
//		{
//			auto root = Zen::Vap2d::Root::GetDefault()->getRootNode();
//			root->setVisible(!root->isVisible());
//		}
		return this;
	}

};

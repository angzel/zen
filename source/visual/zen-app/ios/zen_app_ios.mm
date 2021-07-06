/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_app_ios.h"

#if ZEN_OS_APPLE_IOS
#include "GLESAppDelegate.h"
#include "GLESViewController.h"
#include "zen_log.h"

AppIOS * AppIOS::_me = new AppIOS();

Zen::App * Zen::App::Get()
{
	return AppIOS::Get();
}


void AppIOS::update()
{
	mDelegate->onUpdate();
}

void AppIOS::draw()
{
	mDelegate->onDraw();
}

void AppIOS::launch(Zen::Size2 view_size)
{
	Zen::LogV("---\napp launch ok\nscreen_size:%.2f %.2f\n"
			  "rotatable:%d\nfps:%.2f\n---",
			  view_size.w, view_size.h,
			  (int)isRotatable(),
			  (float)getFramesPerSecond()
			  );
	
	mDelegate->onLaunch(view_size);
	mViewSize = view_size;
}

void AppIOS::resize(Zen::Size2 view_size)
{
	mDelegate->onResize(view_size);
	mViewSize = view_size;
}

void AppIOS::exit()
{
	mDelegate->onExit();
}

void AppIOS::pause()
{
	if(m_is_paused) return;
	m_is_paused = true;
	mDelegate->onPause();
}

void AppIOS::resume()
{
	if(!m_is_paused) return;
	mDelegate->onResume();
	m_is_paused = false;
}

void AppIOS::touchDown(Zen::AppTouch touch)
{
	mDelegate->onTouchDown(touch);
}

void AppIOS::touchUp(Zen::AppTouch touch)
{
	mDelegate->onTouchUp(touch);
}

void AppIOS::touchCancel(Zen::AppTouch touch)
{
	mDelegate->onTouchCancel(touch);
}

void AppIOS::touchMove(Zen::AppTouch touch)
{
	mDelegate->onTouchMove(touch);
}
#endif

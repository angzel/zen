/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#include "zen_app_mac.h"

#if ZEN_OS_APPLE_MAC
#include "AppMetalDelegate.h"
#include "zen_log.h"

AppMac * AppMac::_me = new AppMac();

Zen::App * Zen::App::Get()
{
    return AppMac::Get();
}


void AppMac::update()
{
    mDelegate->onUpdate();
}

void AppMac::draw()
{
    mDelegate->onDraw();
}

void AppMac::launch(Zen::Size2 view_size)
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

void AppMac::resize(Zen::Size2 view_size)
{
	Zen::LogV("---\napp resize\nscreen_size:%.2f %.2f\n"
		   "rotatable:%d\nfps:%.2f\n---",
		   view_size.w, view_size.h,
		   (int)isRotatable(),
		   (float)getFramesPerSecond()
		   );
    mDelegate->onResize(view_size);
    mViewSize = view_size;
}

void AppMac::exit()
{
    mDelegate->onExit();
}

void AppMac::pause()
{
    if(m_is_paused) return;
    m_is_paused = true;
    mDelegate->onPause();
}

void AppMac::resume()
{
    if(!m_is_paused) return;
    mDelegate->onResume();
    m_is_paused = false;
}

void AppMac::touchDown(Zen::AppTouch touch)
{
    mDelegate->onTouchDown(touch);
}

void AppMac::touchUp(Zen::AppTouch touch)
{
    mDelegate->onTouchUp(touch);
}

void AppMac::touchCancel(Zen::AppTouch touch)
{
    mDelegate->onTouchCancel(touch);
}

void AppMac::touchMove(Zen::AppTouch touch)
{
    mDelegate->onTouchMove(touch);
}
#endif

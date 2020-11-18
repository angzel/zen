/*
 Copyright (c) 2013 MeherTJ G.

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "AppRuntimeIOS.h"

#if defined(ZEN_OS_IOS)
#include "GLESAppDelegate.h"
#include "GLESViewController.h"

AppRuntimeIOS * AppRuntimeIOS::_me = new AppRuntimeIOS();

Zen::App * Zen::App::S()
{
	return AppRuntimeIOS::S();
}


void AppRuntimeIOS::update()
{
	mDelegate->onUpdate();
}

void AppRuntimeIOS::draw()
{
	mDelegate->onDraw();
}

void AppRuntimeIOS::launch(Zen::Size2 view_size)
{
	mDelegate->onLaunch(view_size);
	mViewSize = view_size;
}

void AppRuntimeIOS::resize(Zen::Size2 view_size)
{
	mDelegate->onResize(view_size);
	mViewSize = view_size;
}

void AppRuntimeIOS::exit()
{
	mDelegate->onExit();
}

void AppRuntimeIOS::pause()
{
	if(m_is_paused) return;
	m_is_paused = true;
	mDelegate->onPause();
}

void AppRuntimeIOS::resume()
{
	if(!m_is_paused) return;
	mDelegate->onResume();
	m_is_paused = false;
}

void AppRuntimeIOS::touchDown(Zen::AppTouch touch)
{
	mDelegate->onTouchDown(touch);
}

void AppRuntimeIOS::touchUp(Zen::AppTouch touch)
{
	mDelegate->onTouchUp(touch);
}

void AppRuntimeIOS::touchCancel(Zen::AppTouch touch)
{
	mDelegate->onTouchCancel(touch);
}

void AppRuntimeIOS::touchMove(Zen::AppTouch touch)
{
	mDelegate->onTouchMove(touch);
}
#endif

/*
 Copyright (c) 2013 ClearSky G.

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

#import "AppRuntimeIOS.h"
#import "AppDelegate.h"
#import "ViewController.h"


AppRuntimeIOS * AppRuntimeIOS::_me = new AppRuntimeIOS();

Zen::AppRuntime * Zen::AppRuntime::GetDefault()
{
	return AppRuntimeIOS::GetDefault();
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
	mDelegate->onPause();
}

void AppRuntimeIOS::resume()
{
	mDelegate->onResume();
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

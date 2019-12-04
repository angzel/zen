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

#import "AppRuntime.h"
#import "AppDelegate.h"
#import "ViewController.h"


AppRuntime * AppRuntime::_me = new AppRuntime();

Zen::App::Runtime * Zen::App::Runtime::GetDefault()
{
	return AppRuntime::GetDefault();
}


void AppRuntime::update()
{
	mDelegate->onUpdate();
}

void AppRuntime::draw()
{
	mDelegate->onDraw();
}

void AppRuntime::launch(Zen::Size2 const & view_size)
{
	mDelegate->onLaunch(view_size);
	mViewSize = view_size;
}

void AppRuntime::resize(Zen::Size2 const & view_size)
{
	mDelegate->onResize(mViewSize, view_size);
	mViewSize = view_size;
}

void AppRuntime::exit()
{
	mDelegate->onExit();
}

void AppRuntime::pause()
{
	mDelegate->onPause();
}

void AppRuntime::resume()
{
	mDelegate->onResume();
}


void AppRuntime::touchDown(Zen::App::Touch touch)
{
	mDelegate->onTouchDown(touch);
}

void AppRuntime::touchUp(Zen::App::Touch touch)
{
	mDelegate->onTouchUp(touch);
}

void AppRuntime::touchCancel(Zen::App::Touch touch)
{
	mDelegate->onTouchCancel(touch);
}

void AppRuntime::touchMove(Zen::App::Touch touch)
{
	mDelegate->onTouchMove(touch);
}

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

#if defined(__ANDROID__)

#include "AppRuntimeAndroid.h"

namespace Zen {

	AppRuntimeAndroid * AppRuntimeAndroid::_me = new AppRuntimeAndroid();

	AppRuntime * AppRuntime::GetDefault()
	{
		return AppRuntimeAndroid::GetDefault();
	}

	void AppRuntimeAndroid::update()
	{
		mDelegate->onUpdate();
	}

	void AppRuntimeAndroid::draw()
	{
		mDelegate->onDraw();
	}

	void AppRuntimeAndroid::launch(Zen::Size2 const & view_size)
	{
		mDelegate->onLaunch(view_size);
		mViewSize = view_size;
	}

	void AppRuntimeAndroid::resize(Zen::Size2 const & view_size)
	{
		mDelegate->onResize(mViewSize, view_size);
		mViewSize = view_size;
	}

	void AppRuntimeAndroid::exit()
	{
		mDelegate->onExit();
	}

	void AppRuntimeAndroid::pause()
	{
		mDelegate->onPause();
	}

	void AppRuntimeAndroid::resume()
	{
		mDelegate->onResume();
	}

	void AppRuntimeAndroid::back()
	{
		mDelegate->onBack();
	}

	void AppRuntimeAndroid::touchDown(Zen::AppTouch touch)
	{
		mDelegate->onTouchDown(touch);
	}

	void AppRuntimeAndroid::touchUp(Zen::AppTouch touch)
	{
		mDelegate->onTouchUp(touch);
	}

	void AppRuntimeAndroid::touchCancel(Zen::AppTouch touch)
	{
		mDelegate->onTouchCancel(touch);
	}

	void AppRuntimeAndroid::touchMove(Zen::AppTouch touch)
	{
		mDelegate->onTouchMove(touch);
	}

}

#endif

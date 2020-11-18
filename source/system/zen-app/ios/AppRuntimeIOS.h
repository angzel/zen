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

#pragma once

#include "zen_app_config.h"

#if defined(ZEN_OS_IOS)

#if ZEN_APP_DRAW_API_METAL
#import "MetalAppDelegate.h"
#import "MetalViewController.h"
#elif ZEN_APP_DRAW_API_GLES
#import "GLESAppDelegate.h"
#import "GLESViewController.h"
#else
#error "no valid DRAW API select"
#endif

#include "zen_app_delegate.h"
#include "zen_app.h"

class AppRuntimeIOS : public Zen::App
{
protected:
	std::shared_ptr<Zen::AppRuntimeDelegate> mDelegate = nullptr;

	Zen::Size2 mViewSize;

	bool m_rotatable = false;

	bool m_status_visible = false;

	bool m_is_paused = false;
	
	static AppRuntimeIOS * _me;

public:
	static AppRuntimeIOS * S()
	{
		return _me;
	}

	virtual void setRuntimeDelegate(std::shared_ptr<Zen::AppRuntimeDelegate> delegate)
	{
		mDelegate = delegate;
	}

	virtual void onUpdate()
	{

	}
	virtual void onDraw()
	{

	}

	virtual std::shared_ptr<Zen::AppRuntimeDelegate>
	getRuntimeDelegate()
	{
		return mDelegate;
	}

	virtual float getFramesPerSecond()
	{
		return [G_view_controller getFPS];
	}
	virtual void setFramesPerSecond(float frames)
	{
		[G_view_controller setFPS:(int)frames];
	}

	virtual bool isMultiTouch()
	{
		return G_view_controller.view.isMultipleTouchEnabled;
	}
	virtual void setMultiTouch(bool use)
	{
		G_view_controller.view.multipleTouchEnabled = use;
	}

	virtual bool isRotatable()
	{
		return m_rotatable;
	}
	virtual void setRotatable(bool r)
	{
		m_rotatable = r;
	}

	virtual bool isStatusVisible()
	{
		return m_status_visible;
	}
	virtual void setStatusVisible(bool show)
	{
		m_status_visible = show;
	}

public:
	void update();

	void draw();

	void launch(Zen::Size2 view_size);

	void resize(Zen::Size2 view_size);

	void exit();

	void pause();

	void resume();

	void touchDown(Zen::AppTouch);

	void touchUp(Zen::AppTouch);
	
	void touchCancel(Zen::AppTouch);

	void touchMove(Zen::AppTouch);

protected:
	AppRuntimeIOS()
	{
		mDelegate = nullptr;
	}
};

#endif

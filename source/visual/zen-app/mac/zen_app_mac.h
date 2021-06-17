/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_app_config.h"

#if ZEN_OS_APPLE_MAC

#import "AppMetalViewController.h"
#import "AppMetalRenderer.h"

#include "zen_app_delegate.h"
#include "zen_app.h"

class AppMac : public Zen::App
{
protected:
	std::shared_ptr<Zen::AppRuntimeDelegate> mDelegate = nullptr;
	
	Zen::Size2 mViewSize;
	
	bool m_rotatable = false;
	
	bool m_status_visible = false;
	
	bool m_is_paused = false;
	
	bool m_is_bg_render = false;

	static AppMac * _me;
	
public:
	static AppMac * Get()
	{
		return _me;
	}
	
	virtual void setRuntimeDelegate
	(std::shared_ptr<Zen::AppRuntimeDelegate> delegate)
	override
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
	getRuntimeDelegate() override
	{
		return mDelegate;
	}
	
	virtual float getFramesPerSecond() override
	{
		return [gViewController getFPS];
	}
	virtual void setFramesPerSecond(float frames) override
	{
		[gViewController setFPS:(int)frames];
	}
	
	virtual bool isMultiTouch() override
	{
		return false;
	}
	virtual void setMultiTouch(bool use) override
	{
	}
	
	virtual bool isRotatable() override
	{
		return m_rotatable;
	}
	virtual void setRotatable(bool r) override
	{
		m_rotatable = r;
	}
	
	virtual bool isStatusVisible() override
	{
		return m_status_visible;
	}
	virtual void setStatusVisible(bool show) override
	{
		m_status_visible = show;
	}
	
	virtual bool isBGUpdate() override
	{
		return m_is_bg_render;
	}
	
	virtual void setBGUpdate(bool v) override
	{
		m_is_bg_render = v;
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
	AppMac()
	{
		mDelegate = nullptr;
	}
};

#endif

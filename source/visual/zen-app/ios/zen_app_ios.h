/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_app_config.h"

#if ZEN_OS_APPLE_IOS

#if ZEN_DRAW_METAL
#import "MetalAppDelegate.h"
#import "MetalViewController.h"
#elif ZEN_DRAW_GLES
#import "GLESAppDelegate.h"
#import "GLESViewController.h"
#else
#error "no valid DRAW API select"
#endif

#include "zen_app_delegate.h"
#include "zen_app.h"
#include "zen_log.h"

class AppIOS : public Zen::App
{
protected:
	std::shared_ptr<Zen::AppRuntimeDelegate> mDelegate = nullptr;
	
	Zen::Size2 mViewSize;
	
	bool m_rotatable = false;
	
	bool m_status_visible = false;
	
	bool m_is_paused = false;
	
	static AppIOS * _me;
	
public:
	static AppIOS * Get()
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
		return [G_view_controller getFPS];
	}
	virtual void setFramesPerSecond(float frames) override
	{
		[G_view_controller setFPS:(int)frames];
	}
	
	virtual bool isMultiTouch() override
	{
		return G_view_controller.view.isMultipleTouchEnabled;
	}
	virtual void setMultiTouch(bool use) override
	{
		G_view_controller.view.multipleTouchEnabled = use;
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
	
	bool isBGUpdate() override {
		return false;
	}
	
	void setBGUpdate(bool v) override {
		if(v) Zen::LogV("BGUpdate not supported on iOS");
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
	AppIOS()
	{
		mDelegate = nullptr;
	}
};

#endif

/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_app_delegate.h"
#include "zen_app_config.h"

/**
 once import 'zen-app', use ZenAppMain as main function.
 in ZenAppMain, call Zen::App::Get()->
 */
extern int ZenAppMain(int argc, char const ** argv);

namespace Zen {

	class App
	{
	protected:
		App() = default;
		virtual ~App() = default;
	public:
		static App * Get();

		virtual void setRuntimeDelegate(std::shared_ptr<AppRuntimeDelegate> delegate) = 0;

		virtual std::shared_ptr<AppRuntimeDelegate> getRuntimeDelegate() = 0;

		virtual bool isMultiTouch() = 0;

		virtual void setMultiTouch(bool) = 0;

		virtual bool isRotatable() = 0;

		virtual void setRotatable(bool) = 0;

		virtual float getFramesPerSecond() = 0;

		virtual void  setFramesPerSecond(float) = 0;

		virtual bool isStatusVisible() = 0;

		virtual void setStatusVisible(bool) = 0;
		
		virtual bool isBGUpdate() = 0;
		
		virtual void setBGUpdate(bool) = 0;
	};
}

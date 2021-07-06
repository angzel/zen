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

#pragma once

#if defined(__ANDROID__)

#include "zen_app.h"
#include "zen_log.h"
#include "zen_vector.h"
#include "AppNative.h"

#include <jni.h>
#include <thread>
#include <string>

namespace Zen {
	class AppRuntimeAndroid : public App
	{
	protected:
		std::shared_ptr<Zen::AppRuntimeDelegate> mDelegate = nullptr;

		Zen::Size2 mViewSize;
	public:
		bool m_multi_touch = false;
		bool m_rotatable = false;
		float mFramePerSecond = 60;

		static AppRuntimeAndroid * Get();

		virtual void setRuntimeDelegate(std::shared_ptr<Zen::AppRuntimeDelegate> delegate) override
		{
			mDelegate = delegate;
		}

		virtual std::shared_ptr<Zen::AppRuntimeDelegate>
		getRuntimeDelegate() override
		{
			return mDelegate;
		}

		virtual float getFramesPerSecond() override
		{
			return mFramePerSecond;
		}
		virtual void setFramesPerSecond(float frames) override
		{
			if(frames > 60) frames = 60;
			if(frames < 1) frames = 1;
			mFramePerSecond = frames;
			Zen::Android::SetFramePerSecond(mFramePerSecond);
		}

		virtual bool isMultiTouch() override
		{
			return m_multi_touch;
		}
		virtual void setMultiTouch(bool use) override
		{
			m_multi_touch = use;
		}

		virtual bool isRotatable() override
		{
			return m_rotatable;
		}
		virtual void setRotatable(bool ena) override
		{
			m_rotatable = ena;
		}

		virtual bool isStatusVisible() override
		{
			return false;
		}
		virtual void setStatusVisible(bool show) override
		{
			Zen::LogE("config status visible in your project");
		}
		bool isBGUpdate() override {
			return false;
		}
		
		void setBGUpdate(bool v) override {
			if(v) Zen::LogV("BGUpdate not supported on android");
		}
		
	public:
		void update();

		void draw();

		void launch(Zen::Size2 const & view_size);

		void resize(Zen::Size2 const & view_size);

		void exit();

		void pause();

		void resume();

		void back(); // android only

		void touchDown(Zen::AppTouch);

		void touchUp(Zen::AppTouch);

		void touchCancel(Zen::AppTouch);

		void touchMove(Zen::AppTouch);

	protected:
		AppRuntimeAndroid()
		{
		}
	};
}

#endif

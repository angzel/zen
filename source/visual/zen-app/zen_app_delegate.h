/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_vector.h"

namespace Zen {
	struct AppKey
	{
		int type;
		uint32_t value;
	};

	struct AppTouch
	{
		uint32_t no;
		float x, y;
	};

	struct AppMouse
	{
		uint32_t key_status;
		float x, y;
	};

	class AppRuntimeDelegate
	{
	public:
		virtual ~AppRuntimeDelegate() = default;
		
		virtual void onLaunch(Zen::Size2 view_size) {}

		virtual void onResize(Zen::Size2 view_size) {}

		virtual void onExit() {}

		virtual void onPause() {}

		virtual void onResume() {}

		virtual void onUpdate() {}

		virtual void onDraw() {}

		virtual void onEvent(std::string event) {}

		virtual void onTouchDown(AppTouch touch) {}

		virtual void onTouchMove(AppTouch touch) {}

		virtual void onTouchUp(AppTouch touch) {}

		virtual void onTouchCancel(AppTouch touch) {}

		virtual void onMouseMove(AppMouse mouse) {}

		virtual void onMouseKeyDown(AppMouse mouse, int key_index) {}

		virtual void onMouseKeyUp(AppMouse mouse, int key_index) {}

		virtual void onKeyDown(AppKey key) {}

		virtual void onKeyUp(AppKey key) {}
	};
}

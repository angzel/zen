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

#include "zen_object.h"
#include "zen_vector.h"

namespace Zen { namespace App {
	struct Key
	{
		int type;
		int value;
	};
	struct Touch
	{
		uint32_t no;
		float x, y;
	};

	class RuntimeDelegate
	: public virtual Zen::Object
	{
	public:
		virtual void onLaunch(Zen::Size2 const & view_size) {}

		virtual void onResize(Zen::Size2 const & old_size,
		Zen::Size2 const & new_size) {}

		virtual void onExit() {}

		virtual void onPause() {}

		virtual void onResume() {}

		virtual void onUpdate() {}

		virtual void onDraw() {}

		/**
		@function onBack
		 Android back action
		 */
		virtual void onBack() {}
		/**
		@function onBack
		 Windows <alt+f4> action
		 */
		virtual void onClose() {}

		virtual void onTouchDown(Touch const & touch) {}

		virtual void onTouchMove(Touch const & touch) {}

		virtual void onTouchUp(Touch const & touch) {}

		virtual void onTouchCancel(Touch const & touch) {}

		virtual void onMouseMove(Touch const & touch) {}

		virtual void onKeyDown(Key const & key) {}

		virtual void onKeyUp(Key const & key) {}
	};
}}

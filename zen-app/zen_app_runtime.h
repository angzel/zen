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
#include "zen_app_delegate.h"

extern int ZenAppMain(int argc, char const ** argv);

namespace Zen { namespace App {

	class Runtime : public virtual Zen::Object
	{
	protected:
		Runtime() = default;
	public:
		static Runtime * GetDefault();

		virtual void setRuntimeDelegate(Zen::App::RuntimeDelegate* delegate) = 0;

		virtual RuntimeDelegate * getRuntimeDelegate() = 0;

		virtual bool isMultiTouch() = 0;

		virtual void setMultiTouch(bool) = 0;

		virtual bool isRotatable() = 0;

		virtual void setRotatable(bool) = 0;

		virtual float getFramesPerSecond() = 0;

		virtual void  setFramesPerSecond(float) = 0;

		virtual bool isStatusVisible() = 0;

		virtual void setStatusVisible(bool) = 0;
	};
}}

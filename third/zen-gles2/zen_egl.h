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
#include "zen_gles2_enum.h"
#include "zen_os.h"
#if defined(ZEN_OS_WIN)
#include <EGL/egl.h>
namespace Zen { namespace GL { 
	class Window 
	{
	protected:
		EGLDisplay mEglDisplay;
		
		EGLContext mEglContext;
		
		EGLSurface mEglSurface;
		
		Window(Window const &);
		
		void operator=(Window const &);
		
	public:
		Window(EGLNativeWindowType window);
		
		~Window();
		
	public:
		inline void swapBuffer();
		
	public:
		inline void EnableVSync(bool);
		
		inline int32_t getWidth() const;
		
		inline int32_t getHeight() const;
		
		inline EGLDisplay _getDisplay();
		
		inline EGLContext _getContext();
		
		inline EGLSurface _getSurface();
		
		inline void setSwapInterval(int interval);
		
	};
} }



// class Window
namespace Zen {
	namespace GL {
		inline void Window::swapBuffer()
		{
#if ZEN_DEBUG
			EGLBoolean res = eglSwapBuffers(mEglDisplay, mEglSurface);
			musts(res == EGL_TRUE, "failed to swap buffers of egl");
#else
			eglSwapBuffers(mEglDisplay, mEglSurface);
#endif
		}
		inline void Window::setSwapInterval(int interval)
		{
			eglSwapInterval(mEglDisplay, interval);
		}
		inline int32_t Window::getWidth() const
		{
			EGLint value;
			eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &value);
			return (int32_t)value;
		}
		inline int32_t Window::getHeight() const
		{
			EGLint value;
			eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &value);
			return (int32_t)value;
		}
		inline EGLDisplay Window::_getDisplay()
		{
			return mEglDisplay;
		}
		inline EGLContext Window::_getContext()
		{
			return mEglContext;
		}
		inline EGLSurface Window::_getSurface()
		{
			return mEglSurface;
		}
	}
}
#else
#endif

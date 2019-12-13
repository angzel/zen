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
		EGLDisplay m_display;
		
		EGLContext m_context;
		
		EGLSurface m_surface;
		
		Window(Window const &) = delete;
		
		void operator=(Window const &) = delete;
		
	public:
		inline Window(EGLNativeWindowType window);
		inline ~Window();

	public:
		inline void swapBuffer();
		
	public:
		inline void enableVSync(bool);
		
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
			EGLBoolean res = eglSwapBuffers(m_display, m_surface);
			musts(res == EGL_TRUE, "failed to swap buffers of egl");
#else
			eglSwapBuffers(m_display, m_surface);
#endif
		}
		inline void Window::setSwapInterval(int interval)
		{
			eglSwapInterval(m_display, interval);
		}
		inline int32_t Window::getWidth() const
		{
			EGLint value;
			eglQuerySurface(m_display, m_surface, EGL_WIDTH, &value);
			return (int32_t)value;
		}
		inline int32_t Window::getHeight() const
		{
			EGLint value;
			eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &value);
			return (int32_t)value;
		}
		inline EGLDisplay Window::_getDisplay()
		{
			return m_display;
		}
		inline EGLContext Window::_getContext()
		{
			return m_context;
		}
		inline EGLSurface Window::_getSurface()
		{
			return m_surface;
		}
	}

	inline Window::Window(EGLNativeWindowType window)
	{
			//EGL init.
		static EGLDisplay sEglDisplay = EGL_NO_DISPLAY;
		EGLContext eglContext = 0;
		EGLSurface eglSurface = 0;

		/*Note we use OpenGLES 2 versions in the attributes below just to foolcurrent version of EGL */
		EGLint attributes[]= {
			EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE,8,
			EGL_BLUE_SIZE,8,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE, EGL_NONE
		};

		EGLint ctx_attributes[]= {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE, EGL_NONE
		};

		EGLConfig eglConfig;
		EGLint nConfigs;

		if(sEglDisplay == EGL_NO_DISPLAY)
		{
			sEglDisplay= eglGetDisplay(EGL_DEFAULT_DISPLAY);
		}

		EGLBoolean res = EGL_FALSE;
		res = eglInitialize(sEglDisplay,nullptr, nullptr);
		musts(res == EGL_TRUE, "failed to init EGL");

		res = eglChooseConfig(sEglDisplay, attributes, &eglConfig, 1, &nConfigs);
		musts(res == EGL_TRUE && nConfigs > 0, "no valid EGL config");

		eglSurface= eglCreateWindowSurface(sEglDisplay, eglConfig, window, nullptr);
		eglContext= eglCreateContext(sEglDisplay, eglConfig, EGL_NO_CONTEXT, ctx_attributes);

		res = eglMakeCurrent(sEglDisplay, eglSurface, eglSurface, eglContext);
		musts(res == EGL_TRUE, "failed to make current surface for egl");

		m_context = eglContext;
		m_display = sEglDisplay;
		m_surface = eglSurface;
	}

	inline Window::~Window()
	{
		eglMakeCurrent(m_display,EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(m_display, m_context);
		eglDestroySurface(m_display, m_surface);
		eglTerminate(m_display);
	}
}}
#else
#endif

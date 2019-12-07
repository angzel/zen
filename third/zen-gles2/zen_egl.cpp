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

#include "zen_egl.h"

#if defined(ZEN_OS_WIN) 
namespace Zen { namespace GL {
	Window::~Window()
	{
		eglMakeCurrent(mEglDisplay,EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(mEglDisplay, mEglContext);
		eglDestroySurface(mEglDisplay, mEglSurface);
		eglTerminate(mEglDisplay);
	}
	Window::Window(EGLNativeWindowType window)
	{
		//EGL init.
		static EGLDisplay sEglDisplay = EGL_NO_DISPLAY;
		EGLContext eglContext = 0;
		EGLSurface eglSurface = 0;
		
		/*Note we use OpenGLES 2 versions in the attributes below just to foolcurrent version of EGL */
		EGLint aEGLAttributes[]= {
			//			EGL_BUFFER_SIZE, 32, 
			EGL_RED_SIZE, 4,
			EGL_GREEN_SIZE,4,
			EGL_BLUE_SIZE,4,
			//			EGL_DEPTH_SIZE,16,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE, EGL_NONE
		};
		
		EGLint aEGLContextAttributes[]= {
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
		res = eglInitialize(sEglDisplay,NULL, NULL);
		musts(res == EGL_TRUE, "failed to init EGL");
		
		res = eglChooseConfig(sEglDisplay, aEGLAttributes, &eglConfig, 
													1, &nConfigs);
		musts(res == EGL_TRUE && nConfigs > 0, "no valid EGL config");
		
		eglSurface= eglCreateWindowSurface(sEglDisplay, eglConfig, window, nullptr);
		eglContext= eglCreateContext(sEglDisplay, eglConfig, EGL_NO_CONTEXT,aEGLContextAttributes);
		
		res = eglMakeCurrent(sEglDisplay, eglSurface, eglSurface, eglContext);
		musts(res == EGL_TRUE, "failed to make current surface for egl");
		
		mEglContext = eglContext;
		mEglDisplay = sEglDisplay;
		mEglSurface = eglSurface;
	}
}}
#else
#endif

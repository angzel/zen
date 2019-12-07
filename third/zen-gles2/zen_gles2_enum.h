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

#include "zen_os.h"
#include "zen_exception.h"
#include "zen_log.h"

#if defined(ZEN_OS_MAC)
// glTexImage2D alow size not 2^N?
#   define ZEN_GL_2D_ANY_SZ 1
#   include <OpenGL/OpenGL.h>
#   include <OpenGL/gl3.h>
#   include <OpenGL/glext.h>

#elif defined(ZEN_OS_IOS)
#   define ZEN_GL_2D_ANY_SZ 1
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>

#elif defined(ZEN_OS_ANDROID)
#   define ZEN_GL_2D_ANY_SZ 0
#   include "GLES2/gl2.h"
#   include "GLES2/gl2ext.h"
#   include "GLES2/gl2platform.h"

#elif defined(ZEN_OS_WIN32)
#	include "EGL/egl.h"
#	include "GLES2/gl2.h"
#	include "GLES2/gl2ext.h"
#else
#   error unknow platform
#endif

namespace Zen { namespace GL {
	enum class EType
	{
		Byte                = GL_BYTE,
		UByte               = GL_UNSIGNED_BYTE,
		Short               = GL_SHORT,
		UShort              = GL_UNSIGNED_SHORT,
		Int                 = GL_INT,
		UInt                = GL_UNSIGNED_INT,
		Float               = GL_FLOAT,
		Fixed               = GL_FIXED,
	};
	enum class EDrawMode
	{
		Points              = GL_POINTS,
		Lines               = GL_LINES,
		LineLoop            = GL_LINE_LOOP,
		LineStrip           = GL_LINE_STRIP,
		Triangles           = GL_TRIANGLES,
		TriangleStrip       = GL_TRIANGLE_STRIP,
		TriangleFan         = GL_TRIANGLE_FAN,
	};
	
	// SrcBlend * srcColor + DstBlend * destColor
	enum class EBlendSrc
	{
		Zero                = GL_ZERO,//0.f
		One                 = GL_ONE,//1.f
		DstColor            = GL_DST_COLOR,//bg.color
		OneMinusDstColor    = GL_ONE_MINUS_DST_COLOR,//1-bg.color
		SrcAlphaSaturate    = GL_SRC_ALPHA_SATURATE,//
		SrcAlpha            = GL_SRC_ALPHA,//1-fg.alpha
		OneMinusSrcAlpha    = GL_ONE_MINUS_SRC_ALPHA,//1-fg.alpha
		DstAlpha            = GL_DST_ALPHA,//bg.alpha
		OneMinusDstAlpha    = GL_ONE_MINUS_DST_ALPHA,//1-bg.alpha
	};
	enum class EBlendDst
	{
		Zero                = GL_ZERO,
		One                 = GL_ONE,
		SrcColor            = GL_SRC_COLOR  ,
		OneMinusSrcColor    = GL_ONE_MINUS_SRC_COLOR,
		SrcAlpha            = GL_SRC_ALPHA  ,
		OneMinusSrcAlpha    = GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha            = GL_DST_ALPHA  ,
		OneMinusDstAlpha    = GL_ONE_MINUS_DST_ALPHA,
	};
	enum class EDepthFunc
	{
		Never               = GL_NEVER,
		Less                = GL_LESS,
		Equal               = GL_EQUAL,
		Lequal              = GL_LEQUAL,
		Greater             = GL_GREATER,
		Notequal            = GL_NOTEQUAL,
		Gequal              = GL_GEQUAL,
		Always              = GL_ALWAYS,
	};
	enum class EFace
	{
		None = 0,
		ClockWise,
		CounterClockWise,
		Both,
	};
	
	enum class EFilter
	{
		Nearest             = GL_NEAREST,
		Linear              = GL_LINEAR,
		/* TextureMinFilter only below */
#if 1
		NearestMipmapNearst = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear  = GL_LINEAR_MIPMAP_LINEAR,
#endif
	};
	enum class EWarp
	{
		Repeat              = GL_REPEAT,
		MirrorRepeat        = GL_MIRRORED_REPEAT,
		ClampToEdge         = GL_CLAMP_TO_EDGE,
	};
}}

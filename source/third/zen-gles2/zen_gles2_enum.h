/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_macro.h"
#include "zen_exception.h"
#include "zen_log.h"

#if defined(ZEN_OS_APPLE_MAC)
// glTexImage2D alow size with 2^N value?
#   define ZEN_GL_2D_ANY_SZ 1
#   include <OpenGL/OpenGL.h>
#   include <OpenGL/gl3.h>
#   include <OpenGL/glext.h>

#elif ZEN_OS_APPLE_IOS
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
	enum class eType
	{
		Byte   = GL_BYTE,
		UByte  = GL_UNSIGNED_BYTE,
		Short  = GL_SHORT,
		UShort = GL_UNSIGNED_SHORT,
		Int    = GL_INT,
		UInt   = GL_UNSIGNED_INT,
		Float  = GL_FLOAT,
		Fixed  = GL_FIXED,
	};

#if 0
	enum class ePixel
	{
		Alpha     = GL_ALPHA,
		RGB       = GL_RGB,
		RGBA      = GL_RGBA,
		Depth     = GL_DEPTH_COMPONENT,
		Lumi      = GL_LUMINANCE,
		LumiAlpha = GL_LUMINANCE_ALPHA,
	};
#endif

	enum class eVertexMode
	{
		Point         = GL_POINTS,
		Line          = GL_LINES,
		LineLoop      = GL_LINE_LOOP,
		LineStrip     = GL_LINE_STRIP,
		Triangle      = GL_TRIANGLES,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan   = GL_TRIANGLE_FAN,
	};
	
	// SrcBlend * srcColor + DstBlend * destColor
	enum class eSrcBlend
	{
		Zero             = GL_ZERO,//0.f
		One              = GL_ONE,//1.f
		DstColor         = GL_DST_COLOR,//bg.color
		OneMinusDstColor = GL_ONE_MINUS_DST_COLOR,//1-bg.color
		SrcAlphaSaturate = GL_SRC_ALPHA_SATURATE,//
		SrcAlpha         = GL_SRC_ALPHA,//1-fg.alpha
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,//1-fg.alpha
		DstAlpha         = GL_DST_ALPHA,//bg.alpha
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,//1-bg.alpha
	};
	enum class eDstBlend
	{
		Zero             = GL_ZERO,
		One              = GL_ONE,
		SrcColor         = GL_SRC_COLOR  ,
		OneMinusSrcColor = GL_ONE_MINUS_SRC_COLOR,
		SrcAlpha         = GL_SRC_ALPHA  ,
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DstAlpha         = GL_DST_ALPHA  ,
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA,
	};
	enum class eDepthFunc
	{
		Never    = GL_NEVER,
		Less     = GL_LESS,
		Equal    = GL_EQUAL,
		Lequal   = GL_LEQUAL,
		Greater  = GL_GREATER,
		Notequal = GL_NOTEQUAL,
		Gequal   = GL_GEQUAL,
		Always   = GL_ALWAYS,
	};
	
	enum class eFilter
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
	enum class eWarp
	{
		Repeat       = GL_REPEAT,
		MirrorRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge  = GL_CLAMP_TO_EDGE,
	};
}}

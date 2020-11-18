/*
 Copyright (c) 2013 MeherTJ G.
 
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

// define platform
#if defined(_WIN32)
#	define ZEN_OS_WIN
#	define ZEN_OS_WIN32

#elif defined(__ANDROID__)
#include "jni.h"
#	define ZEN_OS_LINUX
#	define ZEN_OS_ANDROID

#elif defined(__APPLE__)
#include <TargetConditionals.h>
#	define ZEN_OS_APPLE
#	if __has_feature(objc_arc)
#		define ZEN_OC_ARC = 1
#	else
#		define ZEN_OC_ARC = 0
#	endif
#	if  !TARGET_OS_IPHONE
#		define ZEN_OS_MAC
#	else
#		define ZEN_OS_IOS
#	endif
#else
#	if defined(__unix__)
#		define ZEN_OS_UNIX
#	endif
#	if defined(__posix__)
#		define ZEN_OS_POSIX
#	endif
#	if defined(__linux__)
#		define ZEN_OS_LINUX
#	endif
#endif


#define ZEN_BIG_ENDIAN 0x00010203
#define ZEN_LITTLE_ENDIAN 0x03020100
#undef ZEN_BYTE_ORDER

#ifdef __linux__
#	include <endian.h>
#	if __BYTE_ORDER == __BIG_ENDIAN
#		define
#		define ZEN_BYTE_ORDER   ZEN_BIG_ENDIAN
#	elif __BYTE_ORDER == __LITTLE_ENDIAN
#		define ZEN_BYTE_ORDER   ZEN_LITTLE_ENDIAN
#	else
#		error unsupported system
#	endif
#else
#	if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || \
defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
defined(__sparc__)
#		define ZEN_BYTE_ORDER   ZEN_BIG_ENDIAN
#	else
#		define ZEN_BYTE_ORDER   ZEN_LITTLE_ENDIAN
#	endif
#endif

#define ZEN_IS_BIG_ENDIAN (ZEN_BYTE_ORDER == ZEN_BIG_ENDIAN)
#define ZEN_IS_LITTLE_ENDIAN (ZEN_BYTE_ORDER == ZEN_LITTLE_ENDIAN)

#include <cstdint>
#include <cstdarg>

#if !defined(ZEN_DEBUG)
#	if !defined(NO_DEBUG) && (defined(_DEBUG) || defined(DEBUG))
#		define ZEN_DEBUG 1
#	else
#		define ZEN_DEBUG 0
#	endif
#endif

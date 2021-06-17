/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

/// @ZEN_OS_WIN
/// @ZEN_OS_ANDROID
/// @ZEN_OS_APPLE
/// @@ ZEN_OS_APPLE_MAC
/// @@ ZEN_OS_APPLE_IOS
/// @ZEN_OS_UNIX

	// system macro
	// - windows
#if defined(_WIN32)
#	define ZEN_OS_WIN 1

	// - android
#elif defined(__ANDROID__)
#	include "jni.h"
#	define ZEN_OS_ANDROID 1

	// - apple
#elif defined(__APPLE__)
#	include <TargetConditionals.h>
#	define ZEN_OS_APPLE 1
#	if __has_feature(objc_arc)
#		define ZEN_OC_ARC = 1
#	else
#		define ZEN_OC_ARC = 0
#	endif
	// -- apple - mac
#	if  TARGET_OS_OSX
#		define ZEN_OS_APPLE_MAC 1
	// -- apple - ios
#	elif TARGET_OS_IOS
#		define ZEN_OS_APPLE_IOS 1
#	endif
	// - unixlike
#elif defined(__unix__) || defined(__posix__) || defined(__linux__)
#	define ZEN_OS_UNIX 1
#else
#	error "unknow system"
#endif

/// @ZEN_IS_BE
/// @ZEN_IS_LE

#define ZEN_BE_VALUE 0x00010203
#define ZEN_LE_VALUE 0x03020100

#undef ZEN_ENDIAN

#ifdef __linux__
#	include <endian.h>
#	if __BYTE_ORDER == __BIG_ENDIAN
#		define ZEN_ENDIAN   ZEN_BE_VALUE
#	elif __BYTE_ORDER == __LITTLE_ENDIAN
#		define ZEN_ENDIAN   ZEN_LE_VALUE
#	else
#		error "unsupported system"
#	endif
#else
#	if defined(__hppa__) || defined(__m68k__) || defined(mc68000) || \
defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
defined(__sparc__)
#		define ZEN_ENDIAN   ZEN_BE_VALUE
#	else
#		define ZEN_ENDIAN   ZEN_LE_VALUE
#	endif
#endif

#define ZEN_IS_BE (ZEN_ENDIAN == ZEN_BE_VALUE)
#define ZEN_IS_LE (ZEN_ENDIAN == ZEN_LE_VALUE)

#include <cstdint>
#include <cstdarg>

/// @ ZEN_DEBUG 1/0(undef)

#if !defined(ZEN_DEBUG)
#	if !defined(NO_DEBUG) && (defined(_DEBUG) || defined(DEBUG))
#		define ZEN_DEBUG 1
#	else
#		define ZEN_DEBUG 0
#	endif
#elif ZEN_DEBUG != 0
#	define ZEN_DEBUG 1
#endif

/// @ ZEN_RUNTIME_VISUAL 1/0

#if defined(ZEN_P_RUNTIME_VISUAL)
#	if ZEN_P_RUNTIME_VISUAL == 1
#		define ZEN_RUNTIME_VISUAL 1
#	else
#		define ZEN_RUNTIME_VISUAL 0
#	endif
#
#else
#	if ZEN_OS_APPLE_IOS || ZEN_OS_ANDROID
// ios or android default 1
#		define ZEN_RUNTIME_VISUAL 1
#	else
// other platform default 0
#		define ZEN_RUNTIME_VISUAL 0
#	endif
#endif



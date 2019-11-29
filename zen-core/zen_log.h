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

#include <ctime>
#include <cstdarg>
#include <cstdio>
#include <sstream>

#ifdef ZEN_LOG_MTH
#include <thread>
#endif

#ifdef ZEN_OS_ANDROID
#include "android/log.h"
#define ZEN_LOG_TYPE      android_LogPriority
#define ZEN_LOG_ERROR     ANDROID_LOG_ERROR
#define ZEN_LOG_WARN      ANDROID_LOG_WARN
#define ZEN_LOG_INFO      ANDROID_LOG_INFO
#define ZEN_LOG_DEBUG     ANDROID_LOG_DEBUG
#define ZEN_LOG_VERBOSE   ANDROID_LOG_VERBOSE
#else
#define ZEN_LOG_TYPE      char const
#define ZEN_LOG_ERROR     'E'
#define ZEN_LOG_WARN      'W'
#define ZEN_LOG_INFO      'I'
#define ZEN_LOG_DEBUG     'D'
#define ZEN_LOG_VERBOSE   'V'
#endif

#if defined(ZEN_P_LOGS_TAG)
#define ZEN_LOGS_TAG ""#ZEN_P_LOGS_TAG
#else
#define ZEN_LOGS_TAG "ZEN-LOG"
#endif

#if defined(ZEN_P_LOGS_PRIORITY)
#define ZEN_LOGS_PRIORITY ZEN_P_LOGS_PRIORITY
#else
#define ZEN_LOGS_PRIORITY 0xff
#endif

#if defined(ZEN_LOGS_STYLE)
#define ZEN_LOGS_STYLE ZEN_P_LOGS_STYLE
#else
#define ZEN_LOGS_STYLE 0
#endif

namespace Zen {
	inline void _PrintLog(ZEN_LOG_TYPE type, char const * fmt, va_list va)
	{
#ifdef ZEN_OS_ANDROID
		__android_log_vprint(type, ZEN_LOGS_TAG, fmt, va);
#else
		
#	if ZEN_LOGS_STYLE == 2
		time_t ti = ::time(0);
		tm * timeStruct = localtime(&ti);
		fprintf(stdout, "%c/%s[%04d-%02d-%02d %02d:%02d:%02d]:",
				type, ZEN_LOGS_TAG,
				timeStruct->tm_year + 1900,
				timeStruct->tm_mon + 1,
				timeStruct->tm_mday,
				timeStruct->tm_hour,
				timeStruct->tm_min,
				timeStruct->tm_sec);
#	elif ZEN_LOGS_STYLE == 1
		time_t ti = ::time(0);
		tm * timeStruct = localtime(&ti);
		fprintf(stdout, "%c/%s[%02d:%02d:%02d]:",
				type, ZEN_LOGS_TAG,
				timeStruct->tm_hour,
				timeStruct->tm_min,
				timeStruct->tm_sec);
#	else
		fprintf(stdout, "%c:", type);
#	endif
		vfprintf(stdout, fmt, va);
		fprintf(stdout, "\n");
		fflush(stdout);
#endif
	}
	inline void LogE(char const * fmt, ...)
	{
#if ZEN_LOGS_PRIORITY & 1
		va_list va;
		va_start(va, fmt);
		_PrintLog(ZEN_LOG_ERROR, fmt, va);
		va_end(va);
#endif
	}
	inline void LogW(char const * fmt, ...)
	{
#if ZEN_LOGS_PRIORITY & 2
		va_list va;
		va_start(va, fmt);
		_PrintLog(ZEN_LOG_WARN, fmt, va);
		va_end(va);
#endif
	}
	inline void LogI(char const * fmt, ...)
	{
#if ZEN_LOGS_PRIORITY & 4
		va_list va;
		va_start(va, fmt);
		_PrintLog(ZEN_LOG_INFO, fmt, va);
		va_end(va);
#endif
	}
	inline void LogD(char const * fmt, ...)
	{
#if ZEN_LOGS_PRIORITY & 8
		va_list va;
		va_start(va, fmt);
		_PrintLog(ZEN_LOG_DEBUG, fmt, va);
		va_end(va);
#endif
	}
	inline void LogV(char const * fmt, ...)
	{
#if ZEN_LOGS_PRIORITY & 16
		va_list va;
		va_start(va, fmt);
		_PrintLog(ZEN_LOG_VERBOSE, fmt, va);
		va_end(va);
#endif
	}
}



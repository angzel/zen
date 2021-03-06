/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include "zen_macro.h"

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

namespace Zen {
	inline void _PrintLog(ZEN_LOG_TYPE type, char const * fmt, va_list va)
	{
#ifdef ZEN_OS_ANDROID
		__android_log_vprint(type, ZEN_LOGS_TAG, fmt, va);
#else
		std::string fmts;
		fmts.push_back(type);
		fmts.push_back(':');
		fmts.append(fmt);
		fmts.append("\n");
		vfprintf(stdout, fmts.c_str(), va);
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



#pragma once

#include "zen_os.h"

#if defined(ZEN_OS_IOS) // ios : metal/gles
#	if ZEN_P_USE_METAL
#	define ZEN_APP_DRAW_API_METAL 1
#	elif ZEN_P_USE_GLES
#	define ZEN_APP_DRAW_API_GLES 1
#	else
#	warning "no render api selected, use GLES for default"
#	define ZEN_APP_DRAW_API_GLES 1
#	endif

#elif defined(ZEN_OS_MAC) // mac : metal
#	define ZEN_APP_DRAW_API_METAL 1

#else // android windows : gles
#	define ZEN_APP_DRAW_API_GLES 1
#endif


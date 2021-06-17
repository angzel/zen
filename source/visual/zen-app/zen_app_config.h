#pragma once

#include "zen_macro.h"

#if ! ZEN_RUNTIME_VISUAL
#	error "zen_app can only apply to a visual system"
#endif

#if ZEN_OS_APPLE_IOS
// ios : metal/gles
#	if ZEN_P_DRAW_METAL
#		define ZEN_DRAW_METAL 1
#	elif ZEN_P_DRAW_GLES
#		define ZEN_DRAW_GLES  1
#	else
#		define ZEN_DRAW_METAL 1
#	endif
#elif defined(ZEN_OS_APPLE_MAC)
// mac : metal
#	define ZEN_DRAW_METAL 1
#else
// android windows : gles
#	define ZEN_DRAW_GLES  1
#endif

// clean
#if ZEN_DRAW_METAL
#	undef ZEN_DRAW_GLES
#	define ZEN_DRAW_GLES 0
#endif

#if ZEN_DRAW_GLES
#	undef ZEN_DRAW_METAL
#	define ZEN_DRAW_METAL 0
#endif

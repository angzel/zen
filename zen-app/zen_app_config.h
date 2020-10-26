#pragma once

#include "zen_os.h"

#if defined(ZEN_OS_IOS)

#if ZEN_P_USE_METAL
#define ZEN_APP_DRAW_API_METAL 1
#elif ZEN_P_USE_OPENGLES
#define ZEN_APP_DRAW_API_OPENGLES 1
#else
#warning "no render api selected, use opengles for default"
#define ZEN_APP_DRAW_API_OPENGLES 1
#endif

#else
#define ZEN_APP_DRAW_API_OPENGLES 1
#endif

//#define ZEN_APP_DRAW_API_METAL 1

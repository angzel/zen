#pragma once

#if ZEN_P_USE_METAL || 1
#define ZEN_APP_DRAW_API_METAL 1
#elif ZEN_P_USE_OPENGLES || 1
#define ZEN_APP_DRAW_API_OPENGLES 1
#endif

//#define ZEN_APP_DRAW_API_METAL 1

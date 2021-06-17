#pragma once

#include "zen_macro.h"

#if defined(ZEN_OS_ANDROID)
#include "AL/al.h"
#include "AL/alc.h"
#elif ZEN_OS_APPLE_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif defined(ZEN_OS_APPLE_MAC)
#include <OpenAL/OpenAL.h>
#elif defined(ZEN_OS_WIN)
#include "AL/al.h"
#include "AL/alc.h"
#else
#error unknow platform
#endif

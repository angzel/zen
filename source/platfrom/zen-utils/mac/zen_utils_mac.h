#pragma once
#include "zen_macro.h"

#if ZEN_RUNTIME_VISUAL && ZEN_OS_APPLE_MAC

#include "zen_utils.h"
#import <Foundation/Foundation.h>

namespace Zen {
	class UtilsMac : public Utils
	{
	public:
		virtual NSString* DocumentPath(NSString*) = 0;

		virtual NSString * ResourcePath(NSString*) = 0;

		virtual NSString * toString(std::string const & s) = 0;
		
		virtual std::string toString(NSString * s) = 0;

	public:
		static UtilsMac * Get();
	};
}

#endif

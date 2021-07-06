#pragma once

#include "zen_macro.h"

#if ZEN_OS_APPLE_IOS
#include "zen_utils.h"

#import <UIKit/UIKit.h>

namespace Zen {
	class UtilsIOS : public Utils
	{
	public:

		virtual NSString* DocumentPath(NSString*) = 0;

		virtual NSString * ResourcePath(NSString*) = 0;

		virtual NSString * toString(std::string const & s) = 0;
		
		virtual std::string toString(NSString * s) = 0;
	public:
		static UtilsIOS * Get();
	protected:
		UtilsIOS() = default;
	};
}

#endif

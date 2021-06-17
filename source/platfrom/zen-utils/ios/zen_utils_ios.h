#pragma once

#include "zen_macro.h"

#if ZEN_OS_APPLE_IOS
#include "zen_utils.h"

#import <UIKit/UIKit.h>

namespace Zen {
	class UtilsIOS : public Utils
	{
	public:

		NSString* _DocumentPath(std::string const & path);

		NSString * _ResourcePath(std::string const & path);

	public:
		static UtilsIOS * Get();
	};
}

#endif

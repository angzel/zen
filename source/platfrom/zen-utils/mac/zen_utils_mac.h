#pragma once
#include "zen_macro.h"

#if defined(ZEN_OS_APPLE_MAC)

#include "zen_utils.h"
#import <Foundation/Foundation.h>

namespace Zen {
	class UtilsMac : public Utils
	{
	public:
		NSString* _DocumentPath(std::string const & path);

		NSString * _ResourcePath(std::string const & path);

	public:
		static UtilsMac * Get();
	};
}

#endif

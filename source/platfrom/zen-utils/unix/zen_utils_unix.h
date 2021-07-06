#pragma once
#include "zen_macro.h"

#if !ZEN_RUNTIME_VISUAL

#include "zen_utils.h"

namespace Zen {
	class UtilsUnix : public Utils
	{
	public:
		static UtilsUnix * Get();
	};
}

#endif

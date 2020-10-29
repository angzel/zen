#include "zen_os.h"

#if defined(ZEN_OS_ANDROID)
#include "zen_android.h"

namespace Zen {
	namespace Android {

		static JNIEnv * sEnv = nullptr;

		JNIEnv * GetEnv()
		{
			return sEnv;
		}

		void SetEnv(JNIEnv * env)
		{
			sEnv = env;
		}

	}
}

#endif

